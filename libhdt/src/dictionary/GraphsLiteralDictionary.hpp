#ifndef __GRAPHS_LITERAL_DICTIONARY_HPP__
#define __GRAPHS_LITERAL_DICTIONARY_HPP__




#include <Iterator.hpp>
#include <Dictionary.hpp>
#include <HDTSpecification.hpp>

#include "../libdcs/CSD.h"

namespace hdt {

class GraphsLiteralDictionary : public BaseLiteralDictionary {
private:
	csd::CSD *graphss;


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
	void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);

	void importFourthSection(Dictionary *other, IntermediateListener& iListener);
	void saveFourthSection(std::ostream & output, IntermediateListener& iListener);
	void populateHeaderNumFourthSection(Header & header, string rootNode);
	void populateHeaderMaxFourthSectionId(Header & header, string rootNode);

	size_t getNumberOfElements();
	unsigned int getMaxID();
    uint64_t size();
    IteratorUCharString *getGraphs();
	unsigned int getNgraphs(){return graphs->getLength();}
	unsigned int getMaxGraphID(){return graphs->getLength();}


	csd::CSD* getDictionarySection(unsigned int id, TripleComponentRole position) const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

















};
}
#endif
