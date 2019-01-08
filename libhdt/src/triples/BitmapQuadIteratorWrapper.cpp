#include "BitmapQuadIteratorWrapper.hpp"
#include "BitmapQuads.hpp"
#include "Permutation.hpp"
#include "BitSequence375.h"
#include "SingleQuad.hpp"

namespace hdt{

BitmapQuadIteratorWrapper::BitmapQuadIteratorWrapper(BitmapQuads* bmq, IteratorTripleID* it_tid, unsigned int patternG):
	it(it_tid),
	quads(bmq),
	perm(bmq->getPermutationPtr()),
	bitmapPerm(bmq->getBitmapPermPtr()),
	previousTriple(NULL),
	returnTriple(NULL),
	pattG(patternG),
	hasNext_called(false),
	hasPrevious_called(false){}

BitmapQuadIteratorWrapper::~BitmapQuadIteratorWrapper(){
	if (previousTriple)
	{
		delete previousTriple;
		previousTriple = NULL;
	}
	if (returnTriple)
	{
		delete returnTriple;
		returnTriple = NULL;
	}

}

void BitmapQuadIteratorWrapper::goToStart()
{
	if (previousTriple)
	{
		delete previousTriple;
		previousTriple = NULL;
	}
	if(returnTriple){
		delete returnTriple;
		returnTriple = NULL;
	}

	hasNext_called = false;
	hasPrevious_called = false;
	it->goToStart();
}

bool BitmapQuadIteratorWrapper::BitmapQuadIteratorWrapper::hasNext()
{
	if(returnTriple){
		delete returnTriple;
		returnTriple = NULL;
	}

	if (it->hasNext()){
		// tid should always be a non-QuadID TripleID
		const TripleID* tid(it->next());
		const unsigned int gr = getGraphID(*tid);
		TripleID* tid_ptr=NULL;
		
		if(gr!=pattG && pattG!=0){
			returnTriple = NULL;
			return false;
		}

		if (gr)
			tid_ptr = new QuadID(*tid, gr);
		else
			tid_ptr = new TripleID(*tid);
	
		returnTriple = tid_ptr;
		hasNext_called = true;
		return true;
	}
	else
	{
		returnTriple = NULL;
		return false;
	}
}

TripleID* BitmapQuadIteratorWrapper::next(){
	if(!hasNext_called){
		throw std::runtime_error("Unable to call BitmapQuadIteratorWrapper::next() : the hasNext() method has not been called or has returned 'false'");
	}
	hasNext_called = false;
	return returnTriple;

}


bool BitmapQuadIteratorWrapper::hasPrevious()
{
	if(returnTriple){
		delete returnTriple;
		returnTriple = NULL;
	}

	if (it->hasPrevious()){
		// tid should always be a non-QuadID TripleID
		const TripleID* tid(it->previous());
		const unsigned int gr = getGraphID(*tid);
		TripleID* tid_ptr=NULL;
		
		if(gr!=pattG && pattG!=0){
			returnTriple = NULL;
			return false;
		}

		if (gr)
			tid_ptr = new QuadID(*tid, gr);
		else
			tid_ptr = new TripleID(*tid);
	
		returnTriple = tid_ptr;
		hasPrevious_called = true;
		return true;
	}
	else
	{
		returnTriple = NULL;
		return false;
	}
}

TripleID* BitmapQuadIteratorWrapper::previous(){
	if(!hasPrevious_called){
		throw std::runtime_error("Unable to call BitmapQuadIteratorWrapper::previous() : the hasPrevious() method has not been called or has returned 'false'");
	}
	hasPrevious_called = false;
	return returnTriple;
}


unsigned int BitmapQuadIteratorWrapper::getGraphID(const TripleID& tid){
	unsigned int gr;
	
	if(previousTriple && tid.isBasedOn(*previousTriple))
	{
		return getGraphIDNextTriple(tid);
	}
	else
	{
		return getGraphIDNewTriple(tid);
	}
	
	
	//return (previousTriple && tid.isBasedOn(*previousTriple)) ? getGraphIDNextTriple(tid) : getGraphIDNewTriple(tid);
}

unsigned int BitmapQuadIteratorWrapper::getGraphIDNextTriple(const TripleID& tid){
	previousPosition++;

if(bitmapPerm->access(previousPosition)!=0)
{
	const unsigned int val = perm->pi(bitmapPerm->rank1(previousPosition));
	return val;
}
else
{
	return 0;
}


	//return bitmapPerm->access(previousPosition) ? perm->pi(bitmapPerm->rank1(previousPosition)) : 0;
}

unsigned int BitmapQuadIteratorWrapper::getGraphIDNewTriple(const TripleID& tid){
	unsigned int predPos, objPos;
	unsigned int gr = 0;
	const unsigned int subj = tid.getSubject();
	const unsigned int pred = tid.getPredicate();
	const unsigned int obj = tid.getObject();

	predPos = quads->getAdjY().find(subj-1, pred);
	objPos = quads->getAdjZ().find(predPos, obj);


	if(objPos > 0){
		TripleID* tid_ptr=NULL;
		do{
			quads->initTripleIDFromPos(tid_ptr,--objPos);
		}while(tid_ptr->isBasedOn(tid) && objPos>=0);
		objPos++;
	}
		
	if(previousTriple)
		delete previousTriple;
	previousTriple = new TripleID(tid);
	previousPosition = objPos;

	if(bitmapPerm->access(objPos)!=0)
		gr = perm->pi(bitmapPerm->rank1(objPos));
		
	return gr;
}


}
