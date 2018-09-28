#include "BitmapQuadIteratorWrapper.hpp"

namespace hdt{

BitmapQuadIteratorWrapper::BitmapQuadIteratorWrapper(BitmapQuads* bmq, IteratorTripleID* it_tid):
	it(it_tid),
	quads(bmq),
	adjY(bmq->adjY),
	adjZ(bmq->adjZ),
	perm(bmq->permutation),
	bitmapPerm(bmq->bitmapG),
	previousTriple(NULL);

BitmapQuadIteratorWrapper::~BitmapQuadIteratorWrapper(){
	if (previousTriple)
	{
		delete previousTriple;
		previousTriple = NULL;
	}

}

TripleID* BitmapQuadIteratorWrapper::next(){
	TripleID tid(it->next());
	return new QuadID(tid, getGraphID(tid));
}
TripleID* BitmapQuadIteratorWrapper::previous(){
	TripleID tid(it->previous());
	return new QuadID(tid, getGraphID(tid));
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

	predPos = adjY->find(subj-1, pred);
	objPos = adjZ->find(predPos, obj);

	TripleID tid_prev;
	if(objPos != 0){
		while(bitmap.get(--objPos)!=tid);
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
