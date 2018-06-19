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

#include <ext/hash_map>

//#define GOOGLE_HASH

#ifdef GOOGLE_HASH 
#include <sparsehash/sparse_hash_map>

using google::sparse_hash_map;      // namespace where class lives by default
using __gnu_cxx::hash;  // or __gnu_cxx::hash, or maybe tr1::hash, depending on your OS

#else

namespace std { using namespace __gnu_cxx; }

#endif


namespace hdt {

class ControlInformation;
class ProgressListener;
class IntermediateListener;
class Header;

struct DictionaryEntry {
public:
	unsigned int id;
	char *str;

	bool static cmpLexicographic(DictionaryEntry *c1, DictionaryEntry *c2);
	bool static cmpID(DictionaryEntry *c1, DictionaryEntry *c2);
};

struct str_cmp {
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) == 0;
	}
};

typedef std::pair<const char*, DictionaryEntry *> DictEntryPair;

#ifdef GOOGLE_HASH 
typedef sparse_hash_map<const char *, DictionaryEntry *, hash<const char *>, str_cmp> DictEntryHash;
#else
typedef std::hash_map<const char *, DictionaryEntry *, __gnu_cxx::hash<const char *>, str_cmp> DictEntryHash;
#endif

typedef DictEntryHash::const_iterator DictEntryIt;


class BasePlainDictionary : public ModifiableDictionary {
protected:
	std::vector<DictionaryEntry*> shared;
	std::vector<DictionaryEntry*> subjects;
	std::vector<DictionaryEntry*> objects;
	DictEntryHash hashSubject;
	DictEntryHash hashObject;
	unsigned int mapping;
	uint64_t sizeStrings;

	//ControlInformation controlInformation;
	HDTSpecification spec;

// Public Interface
public:
	BasePlainDictionary();
	BasePlainDictionary(HDTSpecification &spec);
	virtual ~BasePlainDictionary();

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position);
	virtual void startProcessing(ProgressListener *listener = NULL);
	void stopProcessing(ProgressListener *listener = NULL);

	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void saveControlInfo(std::ostream &output, ControlInformation &controlInformation);
	void saveShared(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void saveSubjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	void saveObjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker);
	virtual void saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)=0;

	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	virtual void insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements)=0;
	virtual void getFourthSectionSize()const=0;
	unsigned int insert(const std::string &str, const TripleComponentRole position);
	virtual unsigned int getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const;
	virtual unsigned int getLocalId(unsigned int mapping_type, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const;
	unsigned int getMaxID()const;
	virtual void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);

protected:
	virtual unsigned int insertFourthElement(const std::string & str, const TripleComponentRole& pos)=0;


protected:
	virtual void insert(const std::string& entry, const DictionarySection& pos);
	void split(ProgressListener *listener = NULL);
	void lexicographicSort(ProgressListener *listener = NULL);
	virtual void lexicographicSortFourthElement()=0;
	void idSort();
	virtual void idSortFourthElement()=0; 
	virtual void updateIDs() ;
	void convertMapping(unsigned int mapping_type);
	virtual const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;




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
	virtual void populateHeaderFourthElementNum(Header &header, string rootNode)=0;
	virtual void populateHeaderFourthElementMaxId(Header &header, string rootNode)=0;

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);

    void import(Dictionary *other, ProgressListener *listener=NULL);

    IteratorUCharString *getSubjects();
    IteratorUCharString *getObjects();
    IteratorUCharString *getShared();
    //virtual IteratorUCharString *getPredicates()=0;
    //virtual IteratorUCharString *getGraphs()=0;

// ModifiableDictionary


	string getType()const;
	unsigned int getMapping()const;

	void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

// Private methods



public:

};


class DictIterator : public IteratorUCharString {
private:
	std::vector<DictionaryEntry *> &vector;
    size_t pos;
public:
	DictIterator(std::vector<DictionaryEntry *> &vec) : vector(vec), pos(0){

	}
	virtual ~DictIterator() { }

	virtual bool hasNext() {
		return pos<vector.size();
	}

	virtual unsigned char *next() {
		return (unsigned char*)vector[pos++]->str;
	}

    virtual size_t getNumberOfElements() {
		return vector.size();
	}
};


}

#endif /* BASE_PLAIN_DICTIONARY_H_ */
