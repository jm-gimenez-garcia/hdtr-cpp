#ifndef BITMAPQUADITERATORWRAPPER_HPP
#define BITMAPQUADITERATORWRAPPER_HPP

#include "Iterator.hpp"

namespace hdt{
class BitmapQuads;
class Permutation;
class BitSequence375;
class TripleID;

class BitmapQuadIteratorWrapper : public IteratorTripleID{
	private:
		IteratorTripleID* it;
		BitmapQuads* quads;
		Permutation* perm;
		BitSequence375* bitmapPerm;
		TripleID* previousTriple;
		unsigned int previousPosition;

	public:
		BitmapQuadIteratorWrapper(BitmapQuads* bmq, IteratorTripleID* it_tid);
		~BitmapQuadIteratorWrapper();
		bool hasNext();
		TripleID* next();
		bool hasPrevious();
		TripleID* previous();
		void goToStart();
		size_t estimatedNumResults();
		ResultEstimationType numResultEstimation();
		bool canGoTo();
		void goTo(unsigned int pos);
		void skip(unsigned int pos);
		bool findNextOccurrence(unsigned int value, unsigned char component);
		TripleComponentOrder getOrder();
		bool isSorted(TripleComponentRole role);

	private:
		unsigned int getGraphID(const TripleID& tid);
		unsigned int getGraphIDNextTriple(const TripleID& tid);
		unsigned int getGraphIDNewTriple(const TripleID& tid);


};

inline bool BitmapQuadIteratorWrapper::hasNext()
{return it->hasNext();}

inline bool BitmapQuadIteratorWrapper::hasPrevious()
{return it->hasPrevious();}

inline void BitmapQuadIteratorWrapper::goToStart()
{it->goToStart();}

inline size_t BitmapQuadIteratorWrapper::estimatedNumResults()
{return it->estimatedNumResults();}

inline ResultEstimationType BitmapQuadIteratorWrapper::numResultEstimation()
{return it->numResultEstimation();}

inline bool BitmapQuadIteratorWrapper::canGoTo()
{return it->canGoTo();}

inline void BitmapQuadIteratorWrapper::goTo(unsigned int pos)
{return it->goTo(pos);}

inline void BitmapQuadIteratorWrapper::skip(unsigned int pos)
{return it->skip(pos);}

inline bool BitmapQuadIteratorWrapper::findNextOccurrence(unsigned int value, unsigned char component)
{return it->findNextOccurrence(value, component);}

inline TripleComponentOrder BitmapQuadIteratorWrapper::getOrder()
{return it->getOrder();}

inline bool BitmapQuadIteratorWrapper::isSorted(TripleComponentRole role)
{return it->isSorted(role);}

}
#endif
