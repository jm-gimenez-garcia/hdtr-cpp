
#ifndef GRAPHS_PLAIN_DICTIONARY_H__
#define GRAPHS_PLAIN_DICTIONARY_H__

#include "BasePlainDictionary.hpp"
#include "ModifiableGraphsDictionary.hpp"

namespace hdt {
class GraphsPlainDictionary : public BasePlainDictionary, public ModifiableGraphsDictionary {

private:
	std::vector<DictionaryEntry*> graphs;

	unsigned int getNunused()const;

    IteratorUCharString *getGraphs();
    IteratorUCharString *getGraphs()const;

public:
	GraphsPlainDictionary(){}
	GraphsPlainDictionary(HDTSpecification &spec):BasePlainDictionary(spec){}
	~GraphsPlainDictionary();
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	size_t getNumberOfElements()const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const{return getGlobalId(mapping, id, position);}
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return getLocalId(mapping, id, position);}
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);
	void startProcessing(ProgressListener *listener = NULL);
	void push_back(DictionaryEntry* entry, DictionarySection pos);
	string getType()const;



private:
	void lexicographicSortFourthElement(){std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;
	void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements);
	void populateHeaderFourthElementNum(Header &header, string rootNode);
	void populateHeaderFourthElementMaxId(Header &header, string rootNode);

};

}
#endif //GRAPHS_PLAIN_DICTIONARY_H__
