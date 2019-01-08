#ifndef HDT_BASE_REIFICATION_DICTIONARY_HPP_
#define HDT_BASE_REIFICATION_DICTIONARY_HPP_

#include <functional>
#include "TriplesDictionary.hpp"
#include "GraphsDictionary.hpp"
#include "HDTVocabulary.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"
#include "TripleTranslator.hpp"
#include "HDTEnums.hpp"


namespace hdt{

class HDTSpecification;
class IteratorUCharString;
class IteratorUInt;

class BaseReificationDictionary : virtual public TriplesDictionary, virtual public GraphsDictionary, public TripleTranslator{
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
	virtual void populateHeader(Header &header, std::string rootNode)=0;
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
public:
	// new methods
	virtual const TriplesDictionary* getTriplesDictionaryPtr()const=0;
	virtual const  GraphsDictionary* getGraphsDictionaryPtr()const=0;
	std::function<unsigned int (unsigned int,TripleComponentRole)> roleIdToGlobalId = [this](unsigned int arr_id, TripleComponentRole pos){
		const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
		const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
		const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	
		const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gun = getGraphsDictionaryPtr()->getNunused();
	
		switch(pos)
		{
			case PREDICATE:
				if(arr_id>=1 && arr_id <= getNpredicates())
					return arr_id;
				else
					throw std::logic_error("Incorrect array_id for PREDICATE");
			case SUBJECT:
				if(arr_id>=1 && arr_id <= getNsubjects())
					return arr_id;
				else
					throw std::logic_error("Incorrect array_id for SUBJECT");
			case OBJECT:
				if(arr_id>=1 && arr_id <= Tsh+Gsh)
					return arr_id;
				else if(arr_id <= getNobjects())
					return arr_id + Tsubj + Gsubj;
				else
					throw std::logic_error("Incorrect array_id for OBJECT");
			case GRAPH:
				if (arr_id>=1 && arr_id <= Gsh)
					return arr_id + Tsh;
				else if (arr_id <= Gsh + Gsubj)
					return arr_id + Tsh + Tsubj;
				else if (arr_id <= Gsh + Gsubj + Gobj + Gun) 
					return arr_id + Tsh + Tsubj + Tobj;
				else
					throw std::logic_error("Incorrect array_id for GRAPH");

		}
	};
	std::function<unsigned int (unsigned int,TripleComponentRole)> globalIdToRoleId = [this](unsigned int dict_id, TripleComponentRole pos){
		const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
		const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
		const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	
		const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();
		const unsigned int Gun = getGraphsDictionaryPtr()->getNunused();
	
	
		switch(pos){
			case PREDICATE:
				if(dict_id>=1 && dict_id <= getNpredicates())
					return dict_id;
				else
					throw std::logic_error("Incorrect dict_id for PREDICATE");
			case SUBJECT:
				if(dict_id>=1 && dict_id <= getNsubjects())
					return dict_id;
				else
					throw std::logic_error("Incorrect dict_id for SUBJECT");
			case OBJECT:
				if (dict_id>=1 && dict_id <= Tsh+Gsh)
				{
					return dict_id; 
				}
				else if (dict_id>getNsubjects() && dict_id<=getMaxObjectID())
				{
					return dict_id - Tsubj - Gsubj;
				}
				else
				{
					throw std::logic_error("Incorrect dict_id for OBJECT");
				}
			case GRAPH:
				if (dict_id > Tsh && dict_id <= Tsh+Gsh)
					return  dict_id - Tsh;
				else if (dict_id > Tsh+Gsh+Tsubj && dict_id <= Tsh+Gsh+Tsubj+Gsubj)
					return dict_id - Tsh - Tsubj;
				else if(dict_id > Tsh+Gsh+Tsubj+Gsubj+Tobj && dict_id <= Tsh+Gsh+Tsubj+Gsubj+Tobj+Gobj+Gun)
					return dict_id - Tsh - Tsubj -Tobj;
				else
					throw std::logic_error("Incorrect dict_id for GRAPH");
				
		}	
	};



protected:
	virtual TriplesDictionary* getTriplesDictionaryPtr()=0;
	virtual GraphsDictionary* getGraphsDictionaryPtr()=0;
	unsigned int getTrGrGlobalIdFromGlobalId(unsigned int globid, TripleComponentRole position)const;
	unsigned int getGlobalIdFromTrGrGlobalId(unsigned int tr_gr_globid, DictionarySection pos)const;
	unsigned int getGlobalIdFromTrGrGlobalId(unsigned int tr_gr_globid, TripleComponentRole role, const Dictionary* sub_dictionary)const;

};

}

#endif
