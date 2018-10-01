#include "BitmapQuadIteratorWrapper.hpp"
#include "BitmapQuads.hpp"
#include "Permutation.hpp"
#include "BitSequence375.h"
#include "SingleQuad.hpp"

namespace hdt{

BitmapQuadIteratorWrapper::BitmapQuadIteratorWrapper(BitmapQuads* bmq, IteratorTripleID* it_tid):
	it(it_tid),
	quads(bmq),
	perm(bmq->getPermutationPtr()),
	bitmapPerm(bmq->getBitmapPermPtr()),
	previousTriple(NULL){}

BitmapQuadIteratorWrapper::~BitmapQuadIteratorWrapper(){
	if (previousTriple)
	{
		delete previousTriple;
		previousTriple = NULL;
	}

}

TripleID* BitmapQuadIteratorWrapper::next(){
	TripleID* tid(it->next());
	return new QuadID(*tid, getGraphID(*tid));
}
TripleID* BitmapQuadIteratorWrapper::previous(){
	TripleID* tid(it->previous());
	return new QuadID(*tid, getGraphID(*tid));
}


unsigned int BitmapQuadIteratorWrapper::getGraphID(const TripleID& tid){
	unsigned int gr;
	return (previousTriple && *previousTriple == tid) ? getGraphIDNextTriple(tid) : getGraphIDNewTriple(tid);
}

unsigned int BitmapQuadIteratorWrapper::getGraphIDNextTriple(const TripleID& tid){
	previousPosition++;
	return bitmapPerm->access(previousPosition) ? perm->pi(bitmapPerm->rank1(previousPosition)) : 0;
}

unsigned int BitmapQuadIteratorWrapper::getGraphIDNewTriple(const TripleID& tid){
	unsigned int predPos, objPos;
	unsigned int gr = 0;
	const unsigned int subj = tid.getSubject();
	const unsigned int pred = tid.getPredicate();
	const unsigned int obj = tid.getObject();

	predPos = quads->getAdjY().find(subj-1, pred);
	objPos = quads->getAdjZ().find(predPos, obj);

	if(objPos != 0){
		TripleID* tid_ptr;
		do{
			quads->initTripleIDFromPos(tid_ptr,--objPos);
		}while(*tid_ptr != tid);
		objPos++;
	}
	if(bitmapPerm->access(objPos))
		gr = perm->pi(bitmapPerm->rank1(objPos));
		
	if(previousTriple)
		delete previousTriple;
	previousTriple = new TripleID(tid);
	previousPosition = objPos;

	return gr;
}


}
