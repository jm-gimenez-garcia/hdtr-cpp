
#ifndef __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__
#define __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__

#include "TripleTranslator.hpp"
#include "BitmapTriples.hpp"
namespace hdt {
class PermutationId;

class BitmapQuads : public BitmapTriples, public TripleTranslator {
public:
	~BitmapQuads(){}
	IteratorTripleID *search(TripleID &pattern);
	void load(ModifiableTriples &triples, ProgressListener *listener);
	void save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener);
	
private:
	BitSequence375* bitmapG;
	Permutation* permutation;


};
}

#endif
