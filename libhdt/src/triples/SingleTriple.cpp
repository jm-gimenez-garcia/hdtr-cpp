#include <SingleQuad.hpp>
using namespace hdt;
QuadID TripleID::to_QuadID() const {
	return QuadID(*this);
}
TripleID TripleID::to_TripleID() const {
	return *this;
}
QuadID TripleID::to_QuadID_pattern() const {
	return QuadID(*this,0);
}

QuadString TripleString::to_QuadString()const{
	return QuadString(*this);
}
TripleString TripleString::to_TripleString()const{
	return *this;
}
QuadString TripleString::to_QuadString_pattern()const{
	return QuadString(*this,0);
}

