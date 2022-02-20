#include <SingleQuad.hpp>
using namespace hdt;

QuadID QuadID::to_QuadID() const{
	return *this;
}

TripleID QuadID::to_TripleID() const {
	// if(!has_graph)
		return TripleID(subject,predicate,object);
	// else
		// throw std::runtime_error("This QuadID is not a TripleID (there is a graph). Unable to do a lossless conversion from QuadID to TripleID.");
}

QuadString QuadString::to_QuadString()const{
	return *this;
}

TripleString QuadString::to_TripleString()const{
	// if(!has_graph)
		return TripleString(subject,predicate,object);
	// else
		// throw std::runtime_error("This QuadString is not a TripleString (there is a graph). Unable to do a lossless conversion from QuadString to TripleString.");
}
