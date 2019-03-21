#ifndef __TRIPLESLOADER_HPP__
#define __TRIPLESLOADER_HPP__

#include <RDFParser.hpp>

namespace hdt {
	class Dictionary;
	class ModifiableTriples;
	class ProgressListener;
	class TripleString;
	class QuadString;

	class TriplesLoader : public RDFCallback {
	private:
		Dictionary *dictionary;
		ModifiableTriples *triples;
		ProgressListener *listener;
		unsigned long long count;
		uint64_t sizeBytes;
	public:
		TriplesLoader(Dictionary *dictionary, ModifiableTriples *triples, ProgressListener *listener) : dictionary(dictionary), triples(triples), listener(listener), count(0), sizeBytes(0) { }
		void processTriple(const TripleString &triple, unsigned long long pos);
		void processQuad(const QuadString &quad, unsigned long long pos);
		uint64_t getSize() {return sizeBytes;}
	};

}

#endif
