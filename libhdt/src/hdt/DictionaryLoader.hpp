#ifndef DICTIONARYLOADER_HPP_
#define DICTIONARYLOADER_HPP_

#include "RDFParser.hpp"
#include "Dictionary.hpp"
#include "HDTListener.hpp"

namespace hdt {
class TripleString;

class DictionaryLoader : public RDFCallback {
private:
	ModifiableDictionary *dictionary;
	ProgressListener *listener;
	unsigned long long count;
public:
	DictionaryLoader(ModifiableDictionary *dictionary, ProgressListener *listener) : dictionary(dictionary), listener(listener), count(0) { }
	void processTriple(const TripleString &triple, unsigned long long pos);
	unsigned long long getCount()const;
};

inline unsigned long long DictionaryLoader::getCount()const
{return count;}

}



#endif
