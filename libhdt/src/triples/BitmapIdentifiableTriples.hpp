
#ifndef __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__
#define __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__

#include "BitmapTriples.hpp"
namespace hdt {
class PermutationId;

class BitmapIdentifiableTriples : public BitmapTriples {
public:
	~BitmapIdentifiableTriples(){}
	IteratorTripleID *search(TripleID &pattern);
	void load(ModifiableTriples &triples, ProgressListener *listener);
	void save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener);
	
private:
	BitSequence375* bitmapId;
	PermutationId* permId;
	//permutation* permId;


};
}

#endif
