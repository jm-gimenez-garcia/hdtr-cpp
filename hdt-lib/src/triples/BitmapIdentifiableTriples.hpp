
#ifndef __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__
#define __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__

#include "BitmapTriples.hpp"
namespace hdt {


class BitmapIdentifiableTriples : public BitmapTriples {
public:
	~BitmapIdentifiableTriples(){}
	IteratorTripleID *search(TripleID &pattern);
	
	
private:
	BitSequence375* bitmapId;
	//PermutationId* permId;
	permutation* permId;


};

#endif
