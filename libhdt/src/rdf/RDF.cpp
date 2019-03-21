#include <RDF.hpp>
#include <SingleQuad.hpp>

using namespace hdt;

IteratorTripleString* RDFAccess::search(TripleString &pattern) 
{
	if (pattern.hasGraph())
	{
		QuadString qpatt = pattern.to_QuadString();
		return search(qpatt.getSubject().c_str(), qpatt.getPredicate().c_str(), qpatt.getObject().c_str(), qpatt.getGraph().c_str());
	}
	else
	{
		return search(pattern.getSubject().c_str(), pattern.getPredicate().c_str(), pattern.getObject().c_str());
	}
}


