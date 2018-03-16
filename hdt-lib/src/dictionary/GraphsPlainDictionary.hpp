
#ifndef GRAPHS_PLAIN_DICTIONARY_H__
#define GRAPHS_PLAIN_DICTIONARY_H__

namespace hdt {
class GraphsPlainDictionary : public BasePlainDictionary : public GraphsDictionary {

private:
	std::vector<DictionaryEntry*> graphs;
	DictEntryHash hashGraphs;
	unsigned int getNgraphs();
	unsigned int getMaxGraphID();
    	IteratorUCharString *getGraphs();

public:
	GraphsPlainDictionary(){}
	~GraphsPlainDictionary(){}
	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	void saveFourthSection(std::ostream &output, ProgressListener *listener, const char marker);
	void insertFourthRegion(IntermediateListener& iListener, const std::string& line, const unsigned char region);
	void getFourthSectionSize()const{return graphs.size();}
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);



private:
	void insert(const std::string& entry, const DictionarySection& pos);
	void lexicographicSortFourthElement(){std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpLexicographic);}
	void idSortFourthElement() {std::sort(graphs.begin(), graphs.end(), DictionaryEntry::cmpID);}
	void updateIDs();
	const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;

};

}
#endif //GRAPHS_PLAIN_DICTIONARY_H__
