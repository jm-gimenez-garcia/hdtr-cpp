#include "QuadsList.hpp"
#include "SingleQuad.hpp"
#include "HDTVocabulary.hpp"
#include "TriplesComparator.hpp"
#include "../util/StopWatch.hpp"
#include <algorithm>

using namespace std;
namespace hdt {

QuadsList::QuadsList() : ptrQ(NULL), orderQ(Unknown), numValidQuads(0){}

QuadsList::QuadsList(const vector<QuadID>& quadArray, const TripleComponentOrder& ord){	
	arrayOfQuads.reserve(quadArray.size());	
	for (unsigned int i=0 ; i<quadArray.size() ; i++)
		if(quadArray[i].isValid())
			arrayOfQuads.push_back(quadArray[i]);

	ptrQ = &arrayOfQuads[0];
	numValidQuads = arrayOfQuads.size();
	orderQ = ord;
}

QuadsList::QuadsList(HDTSpecification &specification) : ptrQ(NULL), orderQ(Unknown), numValidQuads(0){}

QuadsList::~QuadsList(){}

// From Triples
IteratorTripleID* QuadsList::search(TripleID &pattern)
{
	string patternString = pattern.getPatternString();
	if(patternString=="???" || patternString=="????") {
		return new QuadsListIterator(this, pattern);
	} else {
		return new SequentialSearchIteratorTripleID(pattern, new QuadsListIterator(this,pattern));
	}
}

size_t QuadsList::getNumberOfElements() const
{
	return numValidQuads;}

size_t QuadsList::size() const
{
	return getNumberOfElements()*sizeof(QuadID);}

void QuadsList::save(ostream &output, ControlInformation &controlInfo, ProgressListener *listener/*=NULL*/){
	controlInfo.clear();
	controlInfo.setUint("numQuads", numValidQuads);
	controlInfo.setFormat(HDTVocabulary::TRIPLES_TYPE_QUADSLIST);
	controlInfo.setUint("order", orderQ);
	controlInfo.save(output);

	for( unsigned int i = 0; i < arrayOfQuads.size(); i++ ) {
		if ( arrayOfQuads[i].isValid() ) {
			output.write((char *)&arrayOfQuads[i], sizeof(QuadID));
			NOTIFYCOND(listener, "QuadsList saving", i, arrayOfQuads.size())
		}
	}
}

void QuadsList::load(istream &input, ControlInformation &controlInfo, ProgressListener *listener){

	string format = controlInfo.getFormat();
	if(format!=getType()) {
		throw runtime_error("Trying to read a QuadsList but the data is not QuadsList");
	}

	orderQ = (TripleComponentOrder) controlInfo.getUint("order");
	unsigned int totalQuads = controlInfo.getUint("numQuads");

	unsigned int numRead=0;
	QuadID readQuad;

	while(input.good() && numRead<totalQuads) {
		input.read((char *)&readQuad, sizeof(QuadID));
		arrayOfQuads.push_back(readQuad);
        ptrQ = &arrayOfQuads[0];
		numRead++;
		numValidQuads++;
		NOTIFYCOND(listener, "QuadsList loading", numRead, totalQuads)
    }
}


#define CHECKPTR(base, max, size) if(((base)+(size))>(max)) throw runtime_error("Could not read completely the HDT from the file.");

size_t QuadsList::load(unsigned char *char_ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/){

   size_t count=0;

    ControlInformation controlInfo;
    count += controlInfo.load(&char_ptr[count], ptrMax);

    string format = controlInfo.getFormat();
    if(format!=getType()) {
        throw runtime_error("Trying to read a QuadsList but the data is not QuadsList");
    }

    orderQ = (TripleComponentOrder) controlInfo.getUint("order");
    unsigned long long totalQuads = controlInfo.getUint("numQuads");

	numValidQuads = 100000000;

    //CHECKPTR(&char_ptr[count],ptrMax,numValidTriples*12);

    ptrQ = (QuadID*)&char_ptr[count];
	arrayOfQuads.clear();
	arrayOfQuads.assign(ptrQ,ptrQ+totalQuads);

	count += size();

    return count;

}
#undef CHECKPTR

void QuadsList::load(ModifiableTriples &input, ProgressListener *listener/*=NULL*/){
}

void QuadsList::populateHeader(Header &header, string rootNode){
	header.insert(rootNode, HDTVocabulary::TRIPLES_TYPE, HDTVocabulary::TRIPLES_TYPE_QUADSLIST);
	header.insert(rootNode, HDTVocabulary::TRIPLES_NUM_QUADS, getNumberOfElements() );
	header.insert(rootNode, HDTVocabulary::TRIPLES_ORDER, getOrderStr(orderQ) );
}

string QuadsList::getType() const
{return HDTVocabulary::TRIPLES_TYPE_QUADSLIST;}

TripleComponentOrder QuadsList::getOrder() const
{return orderQ;}



// From ModifiableTriples
void QuadsList::insert(TripleID &triple){
	// Add the quad
	orderQ = Unknown;
	arrayOfQuads.push_back(triple.to_QuadID());
    ptrQ = (QuadID*)&arrayOfQuads[0];
	numValidQuads++;
	 
}

void QuadsList::insert(IteratorTripleID *triples){
	    
	size_t numres = triples->estimatedNumResults();
	arrayOfQuads.reserve(numres);

    numValidQuads=0;

	while( triples->hasNext() ) {
		arrayOfQuads[numValidQuads++] = triples->next()->to_QuadID();
		ptrQ = (QuadID*)&arrayOfQuads[0];
	}
	orderQ = Unknown;
}


bool QuadsList::remove(TripleID& pattern){
	QuadID qid_patt = pattern.to_QuadID_pattern();
	bool removedQ=false;
	for(vector<QuadID>::iterator it = arrayOfQuads.begin(); it != arrayOfQuads.end(); ++it) {
		if (it->match(qid_patt)) {
			it->clear(); // why don't we remove it from the vector ? Must the number of TripleIDs in the vector remain constant 
			numValidQuads--;
			removedQ=true;
		}
	}
	return removedQ ;
}

bool QuadsList::remove(IteratorTripleID *pattern){
	bool removed = false;
	vector<QuadID> qidpat_array;

	while(pattern->hasNext()) {
		qidpat_array.push_back(pattern->next()->to_QuadID_pattern());
	}

	for(vector<QuadID>::iterator it_aOQ = arrayOfQuads.begin(); it_aOQ != arrayOfQuads.end(); ++it_aOQ) {
        for(vector<QuadID>::iterator it_patt = qidpat_array.begin(); it_patt != qidpat_array.end(); ++it_patt) {
			if (it_aOQ->match(*it_patt)) {
				it_aOQ->clear();
				numValidQuads--;
				removed = true;
				break;
			}
		}
	}

	return removed;	
}

void QuadsList::sort(TripleComponentOrder ord, ProgressListener *listener/*=NULL*/){
	if(orderQ != ord) {
		//StopWatch st;
		NOTIFY(listener, "Sorting triples", 0, 100);
		std::sort(arrayOfQuads.begin(), arrayOfQuads.end(), TriplesComparator(ord));
		//cout << "Sorted in " << st << endl;
		orderQ = ord;
	}
}

void QuadsList::removeDuplicates(ProgressListener *listener/*=NULL*/){

	if(arrayOfQuads.size()<=1)
		return;
	unsigned int j = 0;
	StopWatch st;

	for(unsigned int i=1; i<arrayOfQuads.size(); i++) {
        if(!arrayOfQuads[i].isValid()) {
            cerr << "WARNING: Quad with null component: " << arrayOfQuads[i] << endl;
        }
		// We don't check if  arrayOfQuads[0] is valid ?
        if(arrayOfQuads[i] != arrayOfQuads[j] && arrayOfQuads[i].isValid()) {
			j++;
			arrayOfQuads[j] = arrayOfQuads[i];
		}
		NOTIFYCOND(listener, "Removing duplicate quads", i, arrayOfQuads.size());
	}

	//cout << "Removed "<< arrayOfQuads.size()-j-1 << " duplicates in " << st << endl;

	arrayOfQuads.resize(j+1);
	numValidQuads = j+1;

}

void QuadsList::setOrder(TripleComponentOrder ord)
{orderQ = ord;}




QuadID* QuadsList::getQuadID(unsigned int i){
	return &ptrQ[i];
}





// ITERATOR

QuadsListIterator::QuadsListIterator(QuadsList* ql, TripleID & patt) :
         pattern(patt.to_QuadID_pattern()), quads(ql), pos(0){}

bool QuadsListIterator::hasNext()
{
	return pos < quads->getNumberOfElements();
}

TripleID *QuadsListIterator::next()
{
	returnQuad = *quads->getQuadID(pos++);
	return &returnQuad;
}

bool QuadsListIterator::hasPrevious()
{
	return pos>0;
}

TripleID *QuadsListIterator::previous()
{
	returnQuad = *quads->getQuadID(--pos);
	return &returnQuad;
}

void QuadsListIterator::goToStart()
{
	pos=0;
}

size_t QuadsListIterator::getNumberOfElements()const
{return quads->getNumberOfElements();}

}
