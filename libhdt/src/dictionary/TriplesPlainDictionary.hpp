#ifndef TRIPLES_PLAIN_DICTIONARY_H__
#define TRIPLES_PLAIN_DICTIONARY_H__

#include "BasePlainDictionary.hpp"
#include "ModifiableTriplesDictionary.hpp"
#include "HDTEnums.hpp"

namespace hdt {

class TriplesPlainDictionary : public BasePlainDictionary, public ModifiableTriplesDictionary {

private:
	std::vector<DictionaryEntry*> predicates;

	unsigned int getNpredicates()const;
	//unsigned int getNgraphs()const;

	unsigned int getMaxPredicateID()const;
	//unsigned int getMaxGraphID()const;

    IteratorUCharString *getPredicates();
    IteratorUCharString *getPredicates()const;
    //IteratorUCharString *getGraphs();
	
public:
	TriplesPlainDictionary(){}
	TriplesPlainDictionary(HDTSpecification &spec):BasePlainDictionary(spec){}
	~TriplesPlainDictionary();
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	size_t getNumberOfElements()const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const{return getGlobalId(mapping, id, position);}
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return getLocalId(mapping, id, position);}
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);
	// It is not necessary to override the virtual method startProcessing for TriplesPlainDictionary, beacause we MUST NOT CLEAR PREDICATES because it is already constructed before calling startProcessing
	void push_back(DictionaryEntry* entry, DictionarySection pos);
	string getType()const;



private:
	void lexicographicSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(predicates.begin(), predicates.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;
	void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements);
	void populateHeaderFourthElementNum(Header &header, string rootNode);
	void populateHeaderFourthElementMaxId(Header &header, string rootNode);

};

}
#endif //TRIPLES_PLAIN_DICTIONARY_H__
