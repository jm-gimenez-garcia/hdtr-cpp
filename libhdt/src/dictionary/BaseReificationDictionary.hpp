#ifndef HDT_BASE_REIFICATION_DICTIONARY_HPP_
#define HDT_BASE_REIFICATION_DICTIONARY_HPP_

#include "TriplesDictionary.hpp"
#include "GraphsDictionary.hpp"
#include "HDTVocabulary.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"


namespace hdt{

class HDTSpecification;
class IteratorUCharString;
class IteratorUInt;

class BaseReificationDictionary : public TriplesDictionary, public GraphsDictionary{
protected:
	unsigned int maxTriplesDictID; 
	uint64_t sizeStrings;

public:
	BaseReificationDictionary();
	BaseReificationDictionary(HDTSpecification &specification);
	virtual ~BaseReificationDictionary();
	
	//methods from Dictionary:
	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
    unsigned int stringToId(const std::string &str, const TripleComponentRole role)const;
    size_t getNumberOfElements() const;
    uint64_t size()const;
	unsigned int getMaxID()const;
	void import(BaseReificationDictionary *other, ProgressListener *listener=NULL);
	void import(Dictionary *other, ProgressListener *listener=NULL);
	void populateHeader(Header &header, std::string rootNode);
	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
    unsigned int getNshared()const;
    unsigned int getNpredicates()const;
    unsigned int getNunused()const;
    unsigned int getNgraphs()const;
	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	unsigned int getMaxPredicateID()const;
	unsigned int getMaxGraphID()const;
    IteratorUCharString *getSubjects();
    IteratorUCharString *getSubjects()const;
    IteratorUCharString *getObjects();
    IteratorUCharString *getObjects()const;
    IteratorUCharString *getShared();
    IteratorUCharString *getShared()const;
    IteratorUCharString *getPredicates();
    IteratorUCharString *getPredicates()const;
    IteratorUCharString *getGraphs();
    IteratorUCharString *getGraphs()const;
    unsigned int getGlobalId(unsigned int id, DictionarySection position)const;
    unsigned int getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position)const;
    unsigned int getLocalId(unsigned int id, TripleComponentRole position)const;
    unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	virtual std::string getType()const=0;
	unsigned int getMapping()const;
    void getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults);
    IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);
    IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);

	// new methods
protected:
	virtual const TriplesDictionary* getTriplesDictionaryPtr()const=0;
	virtual const  GraphsDictionary* getGraphsDictionaryPtr()const=0;
	virtual TriplesDictionary* getTriplesDictionaryPtr()=0;
	virtual GraphsDictionary* getGraphsDictionaryPtr()=0;
	unsigned int getTrGrGlobalIdFromGlobalId(unsigned int globid, TripleComponentRole position)const;
	unsigned int getGlobalIdFromTrGrGlobalId(unsigned int tr_gr_globid, DictionarySection pos)const;

};

}

#endif
