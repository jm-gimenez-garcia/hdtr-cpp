#ifndef REIFICATIONDICTIONARYLOADER_HPP_
#define REIFICATIONDICTIONARYLOADER_HPP_

#include "RDFParser.hpp"
#include "DictionaryLoader.hpp"

namespace hdt {
class TripleString;
class QuadString;
class ProgressListener;
class ModifiableReificationDictionary;
class ModifiableTriplesDictionary;

class ReificationDictionaryLoader : public DictionaryLoader {
	public:
		ReificationDictionaryLoader(ModifiableReificationDictionary *reif_dictionary, ProgressListener *list);
		~ReificationDictionaryLoader(){}
	private:
		ModifiableTriplesDictionary* getDictionary();

	private:
		ModifiableReificationDictionary *const reifDict;
};

}



#endif
