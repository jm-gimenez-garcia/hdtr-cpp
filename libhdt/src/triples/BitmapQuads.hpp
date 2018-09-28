
#ifndef __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__
#define __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__

#include "TripleTranslator.hpp"
#include "BitmapTriples.hpp"
namespace hdt {
class PermutationId;

class BitmapQuads : public BitmapTriples, public TripleTranslator {
public:
	~BitmapQuads(){}
	void load(ModifiableTriples &triples, ProgressListener *listener = NULL);
	void save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener = NULL);
	IteratorTripleID* search(TripleID & pattern);
	IteratorTripleID* search(QuadID & pattern);
	Permutation* getPermutationPtr(){return permutation;}
	const Permutation* getPermutationPtr()const{return static_cast<const Permutation*>(permutation);}
	BitSequence375* getBitmapPermPtr(){return bitmapG;}
	const BitSequence375* getBitmapPermPtr()const{return static_cast<const BitSequence375*>(bitmapG);}

private:
	BitSequence375* bitmapG;
	Permutation* permutation;


};
}

#endif
