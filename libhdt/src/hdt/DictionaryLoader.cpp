#include "DictionaryLoader.hpp"
#include "SingleTriple.hpp"
#include "SingleQuad.hpp"
#include "HDTListener.hpp"
#include "ModifiableTriplesDictionary.hpp"
using namespace hdt;

DictionaryLoader::DictionaryLoader(ProgressListener *list) : listener(list), count(0) { }

void DictionaryLoader::processTriple(const TripleString& triple, unsigned long long pos) {
		cout << __FILE__ << ":" << __LINE__ << " : getDictionary()->getType()=" << getDictionary()->getType() << endl;
	getDictionary()->insert(triple.getSubject(), SUBJECT);
	getDictionary()->insert(triple.getPredicate(), PREDICATE);
	getDictionary()->insert(triple.getObject(), OBJECT);
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Dictionary: %lld K triples processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
}

void DictionaryLoader::processQuad(const QuadString& quad, unsigned long long pos) {
		cout << __FILE__ << ":" << __LINE__ << " : getDictionary()->getType()=" << getDictionary()->getType() << endl;
	getDictionary()->insert(quad.getSubject(), SUBJECT);
	getDictionary()->insert(quad.getPredicate(), PREDICATE);
	getDictionary()->insert(quad.getObject(), OBJECT);
	getDictionary()->insert(quad.getGraph(), GRAPH);
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Dictionary: %lld K quads processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
}
