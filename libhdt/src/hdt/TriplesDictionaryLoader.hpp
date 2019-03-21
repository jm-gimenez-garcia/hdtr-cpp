#ifndef TRIPLESDICTIONARYLOADER_HPP_
#define TRIPLESDICTIONARYLOADER_HPP_

#include <RDFParser.hpp>
#include "DictionaryLoader.hpp"

namespace hdt {
class TripleString;
class QuadString;
class ProgressListener;
class ModifiableTriplesDictionary;

class TriplesDictionaryLoader : public DictionaryLoader {
	public:
		TriplesDictionaryLoader(ModifiableTriplesDictionary* triples_dict, ProgressListener *list);
		~TriplesDictionaryLoader(){}
	private:
		ModifiableTriplesDictionary* getDictionary();

	private:
		ModifiableTriplesDictionary *const trDict;
};

}



#endif
