#include "BitmapQuadIteratorSingle.hpp"
#include "TripleOrderConvert.hpp"

namespace hdt{

BitmapQuadIteratorSingle::BitmapQuadIteratorSingle(BitmapQuads* bmq, QuadID patt):
	quads(bmq),
	perm(bmq->getPermutationPtr()),
	bitmapPerm(bmq->getBitmapPermPtr()),
	pattern(patt),
	subj(0), pred(0), obj(0),
	has_next(false), has_previous(false)
	{goToStart();}



TripleID* BitmapQuadIteratorSingle::next(){
	if(!has_next)
		return NULL;
	TripleID* qid = new QuadID(subj, pred, obj, pattern.getGraph());
	swapComponentOrder(qid, quads->getOrder(), SPO);
	has_next = false;
	has_previous = true;
	return qid;
}

TripleID* BitmapQuadIteratorSingle::previous(){
	if(!has_previous)
		return NULL;
	TripleID* qid = new QuadID(subj, pred, obj, pattern.getGraph());
	swapComponentOrder(qid, quads->getOrder(), SPO);
	has_next = true;
	has_previous = false;
	return qid;
}

void BitmapQuadIteratorSingle::goToStart(){
	const int patX = pattern.getSubject();
	const int patY = pattern.getPredicate();
	const int patZ = pattern.getObject();
	const int patG = pattern.getGraph();

	const AdjencyList adjY = AdjencyList();

	const unsigned int numReifiedTriple = perm->revpi(patG);
	const unsigned int posZ = bitmapPerm->select1(numReifiedTriple+1);
	obj = quads->getAdjZ().get(posZ);

	if(patZ !=0 && patZ != obj){
		subj = pred = obj = 0;
		has_next = false;
	}else{
		unsigned int posY = quads->getAdjZ().findListIndex(posZ);
		pred = quads->getAdjY().get(posY);
		if(patY !=0 && patY != pred){
			subj = pred = obj = 0;
			has_next = false;
		}
		else
			has_next = true;
	}
}

void BitmapQuadIteratorSingle::goTo(unsigned int pos){
	if (!canGoTo())
		throw std::runtime_error("Cannot goto on this bitmaptriples pattern");
	if(pos!=1)
		throw std::runtime_error("Cannot goTo beyond last triple");
	goToStart();
}



}
