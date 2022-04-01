#ifndef BITMAPQUADITERATORSINGLE_HPP_
#define BITMAPQUADITERATORSINGLE_HPP_

#include <Iterator.hpp>
#include <SingleQuad.hpp>
#include "../dictionary/TripleTranslator.hpp"

namespace hdt{
class BitmapQuads;
class Permutation;
class BitSequence375;

class BitmapQuadIteratorSingle : public TripleTranslator, public IteratorTripleID{
	private:
		BitmapQuads *quads;
		Permutation* perm;
		BitSequence375* bitmapPerm;
		QuadID pattern;
		unsigned int subj, pred, obj;
		bool has_next, has_previous;

	public:
		BitmapQuadIteratorSingle(BitmapQuads* bmq, QuadID patt);
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

};

inline bool BitmapQuadIteratorSingle::hasNext()
{return has_next;}

inline bool BitmapQuadIteratorSingle::hasPrevious()
{return has_previous;}

inline bool BitmapQuadIteratorSingle::canGoTo()
{return pattern.isEmpty();}

inline size_t BitmapQuadIteratorSingle::estimatedNumResults()
{return (has_next || has_previous) ? 1 : 0;}


}

#endif
