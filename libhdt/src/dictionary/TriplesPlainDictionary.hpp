#ifndef TRIPLES_PLAIN_DICTIONARY_H__
#define TRIPLES_PLAIN_DICTIONARY_H__

#include "BasePlainDictionary.hpp"
#include "TriplesDictionary.hpp"

namespace hdt {

class TriplesPlainDictionary : public BasePlainDictionary, public TriplesDictionary {

private:
	std::vector<DictionaryEntry*> predicates;
	DictEntryHash hashPredicate;

	unsigned int getNpredicates()const;
	//unsigned int getNgraphs()const;

	unsigned int getMaxPredicateID()const;
	//unsigned int getMaxGraphID()const;

    IteratorUCharString *getPredicates();
    //IteratorUCharString *getGraphs();
	
public:
	TriplesPlainDictionary(){}
	virtual ~TriplesPlainDictionary();
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements);
	size_t getNumberOfElements()const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);
	void startProcessing(ProgressListener *listener = NULL);
	void populateHeaderFourthElementNum(Header &header, string rootNode);
	void populateHeaderFourthElementMaxId(Header &header, string rootNode);



private:
	void insert(const std::string& entry, const DictionarySection& pos);
	void lexicographicSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;
	unsigned int insertFourthElement(const std::string & str, const TripleComponentRole& pos);

};

}
#endif //TRIPLES_PLAIN_DICTIONARY_H__
