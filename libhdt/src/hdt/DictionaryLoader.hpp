#ifndef DICTIONARYLOADER_HPP_
#define DICTIONARYLOADER_HPP_

#include "RDFParser.hpp"
#include "Dictionary.hpp"

namespace hdt {
class TripleString;
class QuadString;
class ProgressListener;
class ModifiableReificationDictionary;

class DictionaryLoader : public RDFCallback {
private:
	ModifiableReificationDictionary *reifDict;
	ProgressListener *listener;
	unsigned long long count;
public:
	DictionaryLoader(ModifiableReificationDictionary *reif_dictionary, ProgressListener *list);
	void processTriple(const TripleString &triple, unsigned long long pos);
	void processQuad(const QuadString& quad, unsigned long long pos);
	unsigned long long getCount()const;
};

inline unsigned long long DictionaryLoader::getCount()const
{return count;}

}



#endif
