#ifndef __GRAPHS_LITERAL_DICTIONARY_HPP__
#define __GRAPHS_LITERAL_DICTIONARY_HPP__


#include "BaseLiteralDictionary.hpp"
#include "GraphsDictionary.hpp"

namespace hdt {

class GraphsLiteralDictionary : public BaseLiteralDictionary, public GraphsDictionary {
private:
	csd::CSD *graphs;


public:
	GraphsLiteralDictionary();
	~GraphsLiteralDictionary();

	protected:
		void clear_all();
		void create_all();
		void clear();
		void create();

	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	void loadFourthSection(std::istream & input, IntermediateListener& iListener);
	void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);

	void importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);
	void saveFourthSection(std::ostream & output, IntermediateListener& iListener);
	void populateHeaderNumFourthSection(Header & header, string rootNode);
	void populateHeaderMaxFourthSectionId(Header & header, string rootNode);

	size_t getNumberOfElements()const;
	unsigned int getMaxID()const;
    uint64_t size()const;

    //IteratorUCharString *getPredicates();
    IteratorUCharString *getGraphs();

	//unsigned int getNpredicates()const;
	unsigned int getNgraphs()const;

	//unsigned int getMaxPredicateID()const;
	unsigned int getMaxGraphID()const;


	csd::CSD* getDictionarySection(unsigned int id, TripleComponentRole position) const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

















};
}
#endif
