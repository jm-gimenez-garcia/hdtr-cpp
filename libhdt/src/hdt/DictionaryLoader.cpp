#include "DictionaryLoader.hpp"
#include "SingleTriple.hpp"
#include "HDTListener.hpp"
using namespace hdt;

void DictionaryLoader::processTriple(const TripleString& triple, unsigned long long pos) {
	//cerr << "Triple String: " << triple << endl;
	dictionary->insert(triple.getSubject(), SUBJECT);
	dictionary->insert(triple.getPredicate(), PREDICATE);
	dictionary->insert(triple.getObject(), OBJECT);
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Dictionary: %lld K triples processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
}
