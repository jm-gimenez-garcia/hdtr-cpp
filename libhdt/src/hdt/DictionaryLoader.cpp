#include "DictionaryLoader.hpp"
#include "SingleTriple.hpp"
#include "SingleQuad.hpp"
#include "HDTListener.hpp"
#include "ModifiableReificationDictionary.hpp"
using namespace hdt;

DictionaryLoader::DictionaryLoader(ModifiableReificationDictionary *reif_dictionary, ProgressListener *list) : reifDict(reif_dictionary), listener(list), count(0) { }

void DictionaryLoader::processTriple(const TripleString& triple, unsigned long long pos) {
	//cerr << "Triple String: " << triple << endl;
	reifDict->insert(triple.getSubject(), SUBJECT);
	reifDict->insert(triple.getPredicate(), PREDICATE);
	reifDict->insert(triple.getObject(), OBJECT);
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Dictionary: %lld K triples processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
}

void DictionaryLoader::processQuad(const QuadString& quad, unsigned long long pos) {
	reifDict->insert(quad.getSubject(), SUBJECT);
	reifDict->insert(quad.getPredicate(), PREDICATE);
	reifDict->insert(quad.getObject(), OBJECT);
	reifDict->insert(quad.getGraph(), GRAPH);
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Dictionary: %lld K quads processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
}
