#ifndef __HDT_MODIFIABLE_REIFICATION_DICTIONARY_HPP__
#define __HDT_MODIFIABLE_REIFICATION_DICTIONARY_HPP__

#include "BaseReificationDictionary.hpp"
#include "ModifiableTriplesDictionary.hpp"
#include "ModifiableGraphsDictionary.hpp"

#include "DictionaryEntry.hpp"

namespace hdt{
	class TriplesPlainDictionary;
	class GraphsPlainDictionary;

class ModifiableReificationDictionary : public BaseReificationDictionary, public ModifiableTriplesDictionary, public ModifiableGraphsDictionary{
	private:
		ModifiableTriplesDictionary* triplesModifDict;
		ModifiableGraphsDictionary* graphsModifDict;
		DictEntryHash hashSubject;
		DictEntryHash hashObject;
		DictEntryHash hashPredicate;
		DictEntryHash hashGraph;
	public:
		ModifiableReificationDictionary();
		ModifiableReificationDictionary(HDTSpecification &spec);

		~ModifiableReificationDictionary();
		
		void startProcessing(ProgressListener *listener = NULL);
		void stopProcessing(ProgressListener *listener = NULL);
		void split(ProgressListener *listener);
		unsigned int insert(const std::string & str, const TripleComponentRole pos);
		void insert(const string& str, const DictionarySection& pos);
		std::string getType()const;
		void populateHeader(Header &header, std::string rootNode);
		void push_back(DictionaryEntry* entry, DictionarySection pos);
	
		const TriplesDictionary* getTriplesDictionaryPtr()const;
		const GraphsDictionary* getGraphsDictionaryPtr()const;
private:
		TriplesDictionary* getTriplesDictionaryPtr();
		GraphsDictionary* getGraphsDictionaryPtr();
};



}
#endif
