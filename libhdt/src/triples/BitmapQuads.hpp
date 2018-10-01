
#ifndef __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__
#define __HDT_BITMAPIDENTIFIABLETRIPLES_HPP__

#include "TripleTranslator.hpp"
#include "BitmapTriples.hpp"




namespace hdt {
class Permutation;
class BitSequence375;
class TripleID;
class IteratorTripleID;

class BitmapQuads : public BitmapTriples {
public:
	~BitmapQuads(){}
	void load(ModifiableTriples &triples, ProgressListener *listener = NULL);
	void save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener = NULL);
	void initTripleIDFromPos(TripleID* tid_ptr, const unsigned int pos)const;
	IteratorTripleID* search(TripleID & pattern);
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
