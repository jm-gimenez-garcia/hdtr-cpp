#ifndef HDT_REIFICATION_DICTIONARY_HPP_
#define HDT_REIFICATION_DICTIONARY_HPP_

#include "TriplesDictionary.hpp"
#include "GraphsDictionary.hpp"
#include "HDTVocabulary.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"

namespace hdt{

class HDTSpecification;

class ReificationDictionary : public TriplesDictionary, public GraphsDictionary{
private:
	TriplesDictionary* triplesDict;
	GraphsDictionary* graphsDict;
	unsigned int maxTriplesDictID; 

public:
	ReificationDictionary(HDTSpecification &specification);
	//from Dictionary:
	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
    unsigned int stringToId(const std::string &str, const TripleComponentRole role)const;
    size_t getNumberOfElements() const;
    uint64_t size()const;
	unsigned int getMaxID()const;
	void import(Dictionary *other, ProgressListener *listener=NULL);
	void populateHeader(Header &header, std::string rootNode);
	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
    unsigned int getNshared()const;
    unsigned int getNpredicates()const;
    unsigned int getNgraphs()const;
	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	unsigned int getMaxPredicateID()const;
	unsigned int getMaxGraphID()const;
    IteratorUCharString *getSubjects();
    IteratorUCharString *getObjects();
    IteratorUCharString *getShared();
    IteratorUCharString *getPredicates();
    IteratorUCharString *getGraphs();
	std::string getType()const;
	unsigned int getMapping()const;
    IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);
    IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);


};

}

#endif
