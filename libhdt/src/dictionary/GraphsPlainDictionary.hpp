
#ifndef GRAPHS_PLAIN_DICTIONARY_H__
#define GRAPHS_PLAIN_DICTIONARY_H__

#include "BasePlainDictionary.hpp"
#include "GraphsDictionary.hpp"

namespace hdt {
class GraphsPlainDictionary : public BasePlainDictionary, public GraphsDictionary {

private:
	std::vector<DictionaryEntry*> graphs;
	DictEntryHash hashGraph;

	//unsigned int getNpredicates()const;
	unsigned int getNgraphs()const;

	//unsigned int getMaxPredicateID()const;
	unsigned int getMaxGraphID()const;

    //IteratorUCharString *getPredicates();
    IteratorUCharString *getGraphs();

public:
	GraphsPlainDictionary(){}
	virtual ~GraphsPlainDictionary();
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	size_t getNumberOfElements()const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);
	void startProcessing(ProgressListener *listener = NULL);



private:
	void insert(const std::string& entry, const DictionarySection& pos);
	void lexicographicSortFourthElement(){std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;
	unsigned int insertFourthElement(const std::string & str, const TripleComponentRole& pos);
	void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements);
	void populateHeaderFourthElementNum(Header &header, string rootNode);
	void populateHeaderFourthElementMaxId(Header &header, string rootNode);

};

}
#endif //GRAPHS_PLAIN_DICTIONARY_H__
