#include "TriplesQuadsList.hpp"
#include <HDTSpecification.hpp>


using namespace hdt;
using namespace std;


TriplesQuadsList::TriplesQuadsList(HDTSpecification &spec):
	tList(spec), qList(spec){}


TriplesQuadsList::TriplesQuadsList(const TriplesList& triplesList, const QuadsList& quadsList):
	tList(triplesList), qList(quadsList){}

TriplesQuadsList::TriplesQuadsList(const std::vector<TripleID>& tripleArray, const std::vector<QuadID>& quadArray, const TripleComponentOrder& ord):
	tList(TriplesList(tripleArray, ord)),qList(QuadsList(quadArray, ord)){}




// From Triples
IteratorTripleID* TriplesQuadsList::search(TripleID &pattern)
{
	string patternString = pattern.getPatternString();
	if(patternString=="???" || patternString=="????") {
		return new TriplesQuadsListIterator(&tList, &qList, pattern);
	} else {
		return new SequentialSearchIteratorTripleID(pattern, new TriplesQuadsListIterator(&tList, &qList, pattern));
	}
}

size_t TriplesQuadsList::getNumberOfElements() const
{return tList.getNumberOfElements() + qList.getNumberOfElements();}


size_t TriplesQuadsList::size() const
{return tList.size() + qList.size();}

void TriplesQuadsList::save(std::ostream &output, ControlInformation &ctrlInfo, ProgressListener *listener/*=NULL*/)
{
	tList.save(output, ctrlInfo, listener);
	qList.save(output, ctrlInfo, listener);
}

void TriplesQuadsList::load(std::istream &input, ControlInformation &ctrlInfo, ProgressListener *listener){
	tList.load(input, ctrlInfo, listener);
	qList.load(input, ctrlInfo, listener);
}

size_t TriplesQuadsList::load(unsigned char *char_ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/){
	size_t count = 0;
	count += tList.load(char_ptr+count, ptrMax, listener);
	count += qList.load(char_ptr+count, ptrMax, listener);

	return count;
}

void TriplesQuadsList::load(ModifiableTriples &input, ProgressListener *listener/*=NULL*/){
	tList.load(input, listener);
	qList.load(input, listener);
}


void TriplesQuadsList::populateHeader(Header &header, string rootNode){
	tList.populateHeader(header, rootNode);
	qList.populateHeader(header, rootNode);
}

string TriplesQuadsList::getType() const
{return "";}





// From ModifiableTriples
void TriplesQuadsList::insert(TripleID &triple){
    if (triple.hasGraph())
	    qList.insert(triple);
    else
	{
		TripleID tid = triple.to_TripleID();
	    tList.insert(tid);
	}
}

void TriplesQuadsList::insert(IteratorTripleID *triples){
	    
	vector<TripleID> triple_array_tmp;
	vector<QuadID> quad_array_tmp;

	size_t numres = triples->estimatedNumResults();
	triple_array_tmp.reserve(numres);
	quad_array_tmp.reserve(numres);


	while( triples->hasNext() ) {
		TripleID* tid_tmp = triples->next();
		if(tid_tmp->hasGraph())
			quad_array_tmp.push_back(tid_tmp->to_QuadID());
		else
			triple_array_tmp.push_back(tid_tmp->to_TripleID());
	}

	TripleID patt_tmp(0,0,0);
	TriplesList* trListTmp = new TriplesList(triple_array_tmp, Unknown);
	TriplesListIterator* trListTmpIt = new TriplesListIterator(trListTmp, patt_tmp);
	tList.insert(trListTmpIt);
	delete trListTmp; trListTmp=NULL;
	delete trListTmpIt; trListTmpIt=NULL;

	QuadsList* qListTmp = new QuadsList(quad_array_tmp, Unknown);
	QuadsListIterator* qListTmpIt = new QuadsListIterator(qListTmp, patt_tmp);
	qList.insert(qListTmpIt);
	delete qListTmp; qListTmp=NULL;
	delete qListTmpIt; qListTmpIt=NULL;
}

bool TriplesQuadsList::remove(TripleID &pattern){
	bool rem = qList.remove(pattern);
	if(!pattern.hasGraph())
		rem = rem || tList.remove(pattern) ;
	return rem;
}

bool TriplesQuadsList::remove(IteratorTripleID *pattern){
	bool removed = false;
	vector<TripleID> tidpat_array;
	vector<QuadID> qidpat_array;
	TripleID* tid_tmp;

	while(pattern->hasNext()) {
		tid_tmp = pattern->next();
		if(!tid_tmp->hasGraph())
			tidpat_array.push_back(tid_tmp->to_TripleID());
		else
			qidpat_array.push_back(tid_tmp->to_QuadID());
	}

	TripleID patt_tmp(0,0,0);
	TriplesList* trListTmp = new TriplesList(tidpat_array, Unknown);
	TriplesListIterator* trListTmpIt = new TriplesListIterator(trListTmp, patt_tmp);
	removed = tList.remove(trListTmpIt);
	delete trListTmp; trListTmp=NULL;
	delete trListTmpIt; trListTmpIt=NULL;

	QuadsList* qListTmp = new QuadsList(qidpat_array, Unknown);
	QuadsListIterator* qListTmpIt = new QuadsListIterator(qListTmp, patt_tmp);
	removed = removed || qList.remove(qListTmpIt);
	delete qListTmp; qListTmp=NULL;
	delete qListTmpIt; qListTmpIt=NULL;

	return removed;	
}

void TriplesQuadsList::sort(TripleComponentOrder ord, ProgressListener *listener/*=NULL*/){
	tList.sort(ord, listener);
	qList.sort(ord, listener);
}

void TriplesQuadsList::removeDuplicates(ProgressListener *listener/*=NULL*/){
	tList.removeDuplicates(listener);
	qList.removeDuplicates(listener);
}

TripleComponentOrder TriplesQuadsList::getOrder() const{
	if ( tList.getOrder() != qList.getOrder() )
		throw std::logic_error("Order of TriplesList and QuadsList are different");
	return Unknown;

}

void TriplesQuadsList::setOrder(TripleComponentOrder ord)
{
	tList.setOrder(ord);
	qList.setOrder(ord);
}





TriplesQuadsListIterator::TriplesQuadsListIterator() :
	tListIt(NULL),
	qListIt(NULL),
	futureNextTid(NULL),
	futurePreviousTid(NULL),
	futureNext(TQLI_FUTURELOCATION_NONE),
	futurePrevious(TQLI_FUTURELOCATION_NONE),
	posTriples(0),
	posQuads(0){}

/*TriplesQuadsListIterator::TriplesQuadsListIterator(TriplesListIterator* tLIt, QuadsListIterator* qLIt) :
	tListIt(tLIt),
	qListIt(qLIt),
	futureNextTid(NULL),
	futurePreviousTid(NULL),
	futureNext(TQLI_FUTURELOCATION_NONE),
	futurePrevious(TQLI_FUTURELOCATION_NONE),
	posTriples(0),
	posQuads(0){}*/

TriplesQuadsListIterator::TriplesQuadsListIterator(TriplesList *triple_list, QuadsList *quad_list, TripleID& patt) :
	tListIt(new TriplesListIterator(triple_list, patt)),
	qListIt(new QuadsListIterator(quad_list, patt)),
	futureNextTid(NULL),
	futurePreviousTid(NULL),
	futureNext(TQLI_FUTURELOCATION_NONE),
	futurePrevious(TQLI_FUTURELOCATION_NONE),
	posTriples(0),
	posQuads(0){
	}

TriplesQuadsListIterator::TriplesQuadsListIterator(TriplesQuadsList *tq_list, TripleID& patt) :
	tListIt(new TriplesListIterator(tq_list->getTriplesList(), patt)),
	qListIt(new QuadsListIterator(tq_list->getQuadsList(), patt)),
	futureNextTid(NULL),
	futurePreviousTid(NULL),
	futureNext(TQLI_FUTURELOCATION_NONE),
	futurePrevious(TQLI_FUTURELOCATION_NONE),
	posTriples(0),
	posQuads(0){}

TriplesQuadsListIterator::~TriplesQuadsListIterator()
{
	delete tListIt; tListIt=NULL;
	delete qListIt; qListIt=NULL;

}

bool TriplesQuadsListIterator::hasNext()
{
	bool triplesHasNext = posTriples < tListIt->getNumberOfElements();

	bool quadsHasNext = posQuads < qListIt->getNumberOfElements();
	return  triplesHasNext || quadsHasNext;
}


TripleID* TriplesQuadsListIterator::next(){
	TripleID* tid_triples;
	TripleID* tid_quads;
	TripleID* tid_return;

	if(futureNext != TQLI_FUTURELOCATION_IN_TRIPLESLIST)
	{
		if(tListIt->hasNext())
		{
			tid_triples = tListIt->next();
		}
		else
		{
			tid_triples = NULL;
		}
	}
	else
	{
		tid_triples = futureNextTid;
	}

	if (futureNext != TQLI_FUTURELOCATION_IN_QUADSLIST)
	{
		if(qListIt->hasNext())
		{
			tid_quads = qListIt->next();
		}
		else
		{
			tid_quads = NULL;
		}
	}
	else
	{
		tid_quads = futureNextTid;
	}


	if(tid_triples)
	{
		if(tid_quads)
		{
			if(tid_quads->compare(*tid_triples) <= 0)
			{
				posQuads++;
				futureNext = TQLI_FUTURELOCATION_IN_TRIPLESLIST;
				futureNextTid = tid_triples;
				tid_return = tid_quads;
			}
			else
			{
				posTriples++;
				futureNext = TQLI_FUTURELOCATION_IN_QUADSLIST;
				futureNextTid = tid_quads;
				tid_return = tid_triples;
			}
		}
		else
		{
			posTriples++;
			futureNext = TQLI_FUTURELOCATION_NONE;
			futureNextTid = NULL;
			tid_return = tid_triples;
		}
	}
	else
	{
		if(tid_quads)
		{
			posQuads++;
			futureNext = TQLI_FUTURELOCATION_NONE;
			futureNextTid = NULL;
			tid_return = tid_quads;
		}
		else
		{
			futureNext = TQLI_FUTURELOCATION_NONE;
			futureNextTid = NULL;
			tid_return = NULL;
			throw runtime_error("Error : Unable to increment TriplesQuadsListIterator");
		}
	}
	cout << "*tid_return  = " << *tid_return << endl;
	return tid_return;
}


TripleID* TriplesQuadsListIterator::previous(){

	TripleID* tid_triples;
	TripleID* tid_quads;
	TripleID* tid_return;

	if(futurePrevious != TQLI_FUTURELOCATION_IN_TRIPLESLIST)
	{
		if(tListIt->hasPrevious())
			tid_triples = tListIt->previous();
		else
			tid_triples = NULL;
	}
	else
		tid_triples = futurePreviousTid;

	if (futurePrevious != TQLI_FUTURELOCATION_IN_QUADSLIST)
	{
		if(qListIt->hasPrevious())
			tid_quads = qListIt->previous();
		else
			tid_quads = NULL;
	}
	else
		tid_quads = futurePreviousTid;


	if(tid_triples)
	{
		if(tid_quads)
		{
			if(tid_quads->compare(*tid_triples) > 0)
			{
				posQuads--;
				futurePrevious = TQLI_FUTURELOCATION_IN_TRIPLESLIST;
				futurePreviousTid = tid_triples;
				tid_return = tid_quads;
			}
			else
			{
				posTriples--;
				futurePrevious = TQLI_FUTURELOCATION_IN_QUADSLIST;
				futurePreviousTid = tid_quads;
				tid_return = tid_triples;
			}
		}
		else
		{
			posTriples--;
			futurePrevious = TQLI_FUTURELOCATION_NONE;
			futurePreviousTid = NULL;
			tid_return = tid_triples;
		}
	}
	else
	{
		if(tid_quads)
		{
			posQuads--;
			futurePrevious = TQLI_FUTURELOCATION_NONE;
			futurePreviousTid = NULL;
			tid_return = tid_quads;
		}
		else
		{
			futurePrevious = TQLI_FUTURELOCATION_NONE;
			futurePreviousTid = NULL;
			tid_return = NULL;
			throw runtime_error("Error : Unable to increment TriplesQuadsListIterator");
		}
	}
	return tid_return;
}


bool TriplesQuadsListIterator::hasPrevious()
{return posTriples>0 || posQuads>0;}




void TriplesQuadsListIterator::goToStart(){
	tListIt->goToStart();
	qListIt->goToStart();
	futureNextTid = NULL;
	futurePreviousTid = NULL;
	futureNext = TQLI_FUTURELOCATION_NONE;
	futurePrevious = TQLI_FUTURELOCATION_NONE;
	posTriples = 0;
	posQuads = 0;
}



