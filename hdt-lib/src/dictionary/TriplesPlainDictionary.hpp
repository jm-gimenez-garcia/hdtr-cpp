#ifndef TRIPLES_PLAIN_DICTIONARY_H__
#define TRIPLES_PLAIN_DICTIONARY_H__

namespace hdt {

class TriplesPlainDictionary : public BasePlainDictionary : public TriplesDictionary {

private:
	std::vector<DictionaryEntry*> predicates;
	DictEntryHash hashPredicate;
	unsigned int getNpredicates();
	unsigned int getMaxPredicateID();
    	IteratorUCharString *getPredicates();
	
public:
	TriplesPlainDictionary(){}
	~TriplesPlainDictionary(){}
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	void saveFourthSection(std::ostream &output, ProgressListener *listener, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, const unsigned char region);
	void getFourthSectionSize()const{return predicates.size();}
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);



private:
	void insert(const std::string& entry, const DictionarySection& pos);
	void lexicographicSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;

};

}
#endif //TRIPLES_PLAIN_DICTIONARY_H__
