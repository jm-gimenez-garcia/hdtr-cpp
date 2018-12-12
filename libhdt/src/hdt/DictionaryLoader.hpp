#ifndef DICTIONARYLOADER_HPP_
#define DICTIONARYLOADER_HPP_

#include "RDFParser.hpp"
#include "Dictionary.hpp"

namespace hdt {
class TripleString;
class QuadString;
class ProgressListener;
class ModifiableTriplesDictionary;

class DictionaryLoader : public RDFCallback {
public:
	DictionaryLoader(ProgressListener *list);
	void processTriple(const TripleString &triple, unsigned long long pos);
	void processQuad(const QuadString& quad, unsigned long long pos);
	unsigned long long getCount()const;
	virtual ~DictionaryLoader(){}
public:
	virtual ModifiableTriplesDictionary* getDictionary()=0;
protected:
	ProgressListener *listener;
	unsigned long long count;
};

inline unsigned long long DictionaryLoader::getCount()const
{return count;}

}



#endif
