#ifndef BASE_PLAIN_DICTIONARY_H_
#define BASE_PLAIN_DICTIONARY_H_


//#include <Iterator.hpp>
#include <HDTSpecification.hpp>
#include <Dictionary.hpp>
#include "Iterator.hpp"
#include <vector>
#include <utility>

#include <cstring>
#include <string>
//#include <algorithm>
//#include <fstream>
//#include <iostream>


#include "DictionaryEntry.hpp"

namespace hdt {

class ControlInformation;
class ProgressListener;
class IntermediateListener;
class Header;

class BasePlainDictionary : public ModifiableDictionary {
protected:
	std::vector<DictionaryEntry*> shared;
	std::vector<DictionaryEntry*> subjects;
	std::vector<DictionaryEntry*> objects;
	unsigned int mapping;
	uint64_t sizeStrings;
	bool dict_sorted;

	//ControlInformation controlInformation;
	HDTSpecification spec;

// Public Interface
public:
	BasePlainDictionary();
	BasePlainDictionary(HDTSpecification &spec);
	bool isDictSorted()const{return dict_sorted;}
	virtual ~BasePlainDictionary();

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	virtual void startProcessing(ProgressListener *listener = NULL);
	void stopProcessing(ProgressListener *listener = NULL);
	unsigned int insert(const std::string & str, const TripleComponentRole pos);

	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void saveControlInfo(std::ostream &output, ControlInformation &controlInformation);
	void saveShared(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void saveSubjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void saveObjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);

	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	//virtual void getFourthSectionSize()const=0;

	virtual unsigned int getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position)const;
	virtual unsigned int getGlobalId(unsigned int id, DictionarySection position)const{return BasePlainDictionary::getGlobalId(mapping, id, position);}
	virtual unsigned int getLocalId(unsigned int mapping_type, unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return BasePlainDictionary::getLocalId(mapping,id,position);}
	unsigned int getMaxID()const;
	virtual void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);
	virtual void push_back(DictionaryEntry* entry, DictionarySection pos);

protected:
	virtual void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)=0;
	virtual void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements)=0;


protected:
	void insert(const std::string& entry, const DictionarySection& pos);
	void lexicographicSort(ProgressListener *listener = NULL);
	virtual void lexicographicSortFourthElement()=0;
	void idSort();
	virtual void idSortFourthElement()=0; 
	virtual void updateIDs()=0;
	void convertMapping(unsigned int mapping_type);
	virtual const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;
	virtual void populateHeaderFourthElementNum(Header &header, string rootNode)=0;
	virtual void populateHeaderFourthElementMaxId(Header &header, string rootNode)=0;




public:

	virtual size_t getNumberOfElements()const;

	uint64_t size()const;

	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
	unsigned int getNshared()const;
	//virtual unsigned int getNpredicates()const=0;
	//virtual unsigned int getNgraphs()const=0;

	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	//virtual unsigned int getMaxPredicateID()const=0;
	//virtual unsigned int getMaxGraphID()const=0;

	void populateHeader(Header &header, string rootNode);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);

    void import(Dictionary *other, ProgressListener *listener=NULL);

    IteratorUCharString *getSubjects();
    IteratorUCharString *getSubjects()const;
    IteratorUCharString *getObjects();
    IteratorUCharString *getObjects()const;
    IteratorUCharString *getShared();
    IteratorUCharString *getShared()const;
    //virtual IteratorUCharString *getPredicates()=0;
    //virtual IteratorUCharString *getGraphs()=0;

// ModifiableDictionary


	virtual string getType()const=0;
	unsigned int getMapping()const;

	void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);
	IteratorUCharString* getSuggestions(const char *base, hdt::TripleComponentRole role);
    IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);

// Private methods



public:

};


class DictIterator : public IteratorUCharString {
private:
	std::vector<DictionaryEntry *> &vector;
    size_t pos;
public:
	DictIterator(std::vector<DictionaryEntry *> &vec) : vector(vec), pos(0){}
	~DictIterator() { }

	bool hasNext() const{
		return pos<vector.size();
	}

	unsigned char *next() {
		return (unsigned char*)vector[pos++]->str;
	}

    size_t getNumberOfElements()const {
		return vector.size();
	}

};

class DictIterator_const : public IteratorUCharString {
private:
	const std::vector<DictionaryEntry *>& vector;
	size_t pos;
public:
	DictIterator_const(const std::vector<DictionaryEntry *>& vec) : vector(vec){}
	~DictIterator_const() { }
	bool hasNext() const{return pos<vector.size();}
	unsigned char *next() {return (unsigned char*)vector[pos++]->str;}
    size_t getNumberOfElements()const {return vector.size();}
	const std::string getStr(const unsigned int idx)const;
};

inline const std::string DictIterator_const::getStr(const unsigned int idx)const{
	if (idx < vector.size())
		return std::string(vector[idx]->str);
	else
	{
		cerr << "idx greater than maximum index" << endl;
		return std::string("");
	}

}

}

#endif /* BASE_PLAIN_DICTIONARY_H_ */
