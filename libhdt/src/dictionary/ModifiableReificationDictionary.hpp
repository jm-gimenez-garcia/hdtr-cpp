#ifndef __HDT_MODIFIABLE_REIFICATION_DICTIONARY_HPP__
#define __HDT_MODIFIABLE_REIFICATION_DICTIONARY_HPP__

#include "BaseReificationDictionary.hpp"

#include "DictionaryEntry.hpp"

namespace hdt{
	class TriplesPlainDictionary;
	class GraphsPlainDictionary;

class ModifiableReificationDictionary : public BaseReificationDictionary, public ModifiableDictionary{
	private:
		TriplesPlainDictionary* triplesModifDict;
		GraphsPlainDictionary* graphsModifDict;
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
	
private:
		const TriplesDictionary* getTriplesDictionaryPtr()const;
		const GraphsDictionary* getGraphsDictionaryPtr()const;
		TriplesDictionary* getTriplesDictionaryPtr();
		GraphsDictionary* getGraphsDictionaryPtr();
};



}
#endif
