#include "BitmapQuads.hpp"
#include "SingleQuad.hpp"
#include "PermutationMRRR.hpp"
#include "BitSequence375.h"
#include "SingleQuad.hpp"
#include "Iterator.hpp"
#include "BitmapQuadIteratorWrapper.hpp"
#include "BitmapQuadIteratorSingle.hpp"
#include "TripleTranslatorIteratorWrapper.hpp"

using namespace std;

namespace hdt{

void BitmapQuads::load(ModifiableTriples &triples, ProgressListener *listener/*=NULL*/) 
{
	triples.sort(order);

	IteratorTripleID *it = triples.searchAll();

	bitmapY = new BitSequence375(triples.getNumberOfElements()/2);
	bitmapZ = new BitSequence375(triples.getNumberOfElements());
	bitmapG = new BitSequence375(triples.getNumberOfElements());

	//LogSequence2* permId_tmp = new LogSequence2(bits(triples.getNumberOfElements()),triples.getNumberOfElements());
	std::vector<size_t> permId_tmp;
	permId_tmp.reserve(triples.getNumberOfElements()/2); //pre-allocate the vector to at least the estimate number of graphs

	LogSequence2 *vectorY = new LogSequence2(bits(triples.getNumberOfElements()));
	LogSequence2 *vectorZ = new LogSequence2(bits(triples.getNumberOfElements()),triples.getNumberOfElements());

	unsigned int lastX, lastY, lastZ;
	unsigned int x, y, z, gr;

	unsigned int numTriples=0;

	while(it->hasNext()) {
		TripleID *triple = it->next();

		swapComponentOrder(triple, SPO, order);

		x = toRoleID(triple->getSubject(), SUBJECT);
		y = toRoleID(triple->getPredicate(), PREDICATE);
		z = toRoleID(triple->getObject(), OBJECT);
		QuadID qid(triple->to_QuadID());
		if (qid.hasGraph())
			gr = toRoleID(qid.getGraph(), GRAPH); // returns 0 if triple is a TripleID
		else
			gr = 0;


		if(x==0 || y==0 || z==0) {
			cerr << "ERROR: Triple with at least one component zero." << endl;
			continue;
		}

		if(numTriples==0){
			vectorY->push_back(y);
			vectorZ->push_back(z);


		} else if(x!=lastX) {
            if(x!=lastX+1) {
                throw std::runtime_error("Error: The subjects must be correlative.");
            }
			bitmapY->append(true);
			vectorY->push_back(y);

			bitmapZ->append(true);
			vectorZ->push_back(z);
		} else if(y!=lastY) {
            if(y<lastY) {
                throw std::runtime_error("Error: The predicates must be in increasing order.");
            }
            bitmapY->append(false);
			vectorY->push_back(y);

			bitmapZ->append(true);
			vectorZ->push_back(z);
		} else {
            if(z<=lastZ) {
                throw std::runtime_error("Error, The objects must be in increasing order.");
            }
            bitmapZ->append(false);
			vectorZ->push_back(z);
		}
		
		if (gr == 0)
			bitmapG->append(false);
		else
		{
			bitmapG->append(true);
			permId_tmp.push_back(gr);
		}

		lastX = x;
		lastY = y;
		lastZ = z;



        NOTIFYCOND(listener, "Converting to BitmapTriples", numTriples, triples.getNumberOfElements());
		numTriples++;
	}

	bitmapY->append(true);
	bitmapZ->append(true);

	delete it;

	vectorY->reduceBits();

	delete arrayY;
	arrayY = vectorY;

	delete arrayZ;
	arrayZ = vectorZ;


	permutation = new PermutationMRRR(permId_tmp, 8);



#if 0
	AdjacencyList adjY(arrayY, bitmapY);
	AdjacencyList adjZ(arrayZ, bitmapZ);
	adjY.dump();
	adjZ.dump();
#endif
}




void BitmapQuads::save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener/*=NULL*/)
{
	CHECK_BITMAPTRIPLES_INITIALIZED

	controlInformation.setFormat(getType());
	controlInformation.setUint("order", order);
	controlInformation.save(output);

	IntermediateListener iListener(listener);
	iListener.setRange(0,5);
	iListener.notifyProgress(0, "BitmapQuads saving Bitmap Y");
	bitmapY->save(output);

	iListener.setRange(5,15);
	iListener.notifyProgress(0, "BitmapQuads saving Bitmap Z");
	bitmapZ->save(output);

	iListener.setRange(15,30);
	iListener.notifyProgress(0, "BitmapQuads saving Stream Y");
	arrayY->save(output);

	iListener.setRange(30,70);
	iListener.notifyProgress(0, "BitmapQuads saving Stream Z");
	arrayZ->save(output);

	iListener.setRange(70,80);
	iListener.notifyProgress(0, "BitmapQuads saving Bitmap Id");
	bitmapG->save(output);
	
	iListener.setRange(80,100);
	iListener.notifyProgress(0, "BitmapQuads saving Permutation Id");
	permutation->save(output);

}
void BitmapQuads::load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener/*=NULL*/)
{
	std::string format = controlInformation.getFormat();
	if(format!=getType()) {
		throw std::runtime_error("Trying to read a BitmapQuads but the data is not BitmapQuads");
	}

	order = (TripleComponentOrder) controlInformation.getUint("order");

	IntermediateListener iListener(listener);

	iListener.setRange(0,5);
	iListener.notifyProgress(0, "BitmapQuads loading Bitmap Y");
	bitmapY = BitSequence375::load(input);
	if(bitmapY==NULL){
		throw std::runtime_error("Could not read bitmapY.");
	}

	iListener.setRange(5,10);
	iListener.notifyProgress(0, "BitmapQuads loading Bitmap Z");
	bitmapZ = BitSequence375::load(input);
	if(bitmapZ==NULL){
		throw std::runtime_error("Could not read bitmapZ.");
	}

	iListener.setRange(10,20);
	iListener.notifyProgress(0, "BitmapQuads loading Array Y");
	delete arrayY;
	arrayY = IntSequence::getArray(input);
	arrayY->load(input);

	iListener.setRange(20,35);
	iListener.notifyProgress(0, "BitmapQuads loading Array Z");
	delete arrayZ;
	arrayZ = IntSequence::getArray(input);
    arrayZ->load(input);

	iListener.setRange(35,40);
	iListener.notifyProgress(0, "BitmapTriples loading Bitmap Id");
	delete bitmapG;
	bitmapG = BitSequence375::load(input);
	if(bitmapG==NULL){
		throw std::runtime_error("Could not read Bitmap Id.");
	}


	iListener.setRange(40,50);
	iListener.notifyProgress(0, "BitmapTriples loading Permutation");
	delete permutation;
	permutation->load(input);
	if(permutation==NULL){
		throw std::runtime_error("Could not read permutation.");
	}

}


IteratorTripleID *BitmapQuads::search(TripleID& pattern)
{
	IteratorTripleID* tid_it;
	QuadID patt = pattern.to_QuadID();
	if(!patt.hasGraph())
		tid_it = BitmapTriples::search(pattern);
	else
	{
		if(patt.getGraph()==0)
		{
			tid_it = new BitmapQuadIteratorWrapper(this, BitmapTriples::search(pattern));
		}
		else
		{
			tid_it = new BitmapQuadIteratorSingle(this, patt); 
		}
	}
	return new TripleTranslatorIteratorWrapper(this, tid_it);
}

//equivalent of get method in java
void BitmapQuads::initTripleIDFromPos(TripleID* tid_ptr, const unsigned int pos)const{
	const unsigned int g = bitmapG->access(pos) ? permutation->pi(bitmapG->rank1(pos)) : 0;
	TripleID* tid_ptr_tmp;
	BitmapTriples::initTripleIDFromPos(tid_ptr_tmp,pos);
	if(tid_ptr)
		delete tid_ptr;
	if(g==0)
		tid_ptr = new TripleID(*tid_ptr_tmp);
	else
		tid_ptr = new QuadID(*tid_ptr_tmp,g);
	delete tid_ptr_tmp;
}


}

