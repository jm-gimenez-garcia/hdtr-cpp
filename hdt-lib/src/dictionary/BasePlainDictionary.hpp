#ifndef BASE_PLAIN_DICTIONARY_H_
#define BASE_PLAIN_DICTIONARY_H_


#include <Iterator.hpp>
#include <HDTSpecification.hpp>
#include <Dictionary.hpp>

#include <string.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

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


#ifdef GOOGLE_HASH 
typedef sparse_hash_map<const char *, DictionaryEntry *, hash<const char *>, str_cmp> DictEntryHash;
#else
typedef std::hash_map<const char *, DictionaryEntry *, __gnu_cxx::hash<const char *>, str_cmp> DictEntryHash;
#endif

typedef DictEntryHash::const_iterator DictEntryIt;


class BasePlainDictionary : public ModifiableDictionary {
private:
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
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	virtual void startProcessing(ProgressListener *listener = NULL);
	void stopProcessing(ProgressListener *listener = NULL);
	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	virtual void saveFourthSection(std::ostream &output, ProgressListener *listener, const char marker)=0;
	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	virtual void insertFourthRegion(IntermediateListener& iListener, const std::string& line, const unsigned char region)=0;
	virtual void getFourthSectionSize()const=0;
	unsigned int insert(const std::string &str, const TripleComponentRole position);
	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const;
	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const;
	unsigned int getMaxID();
	virtual void updateID(unsigned int oldid, unsigned int newid, DictionarySection position);



private:
	virtual void insert(const std::string& entry, const DictionarySection& pos);
	void split(ProgressListener *listener = NULL);
	void lexicographicSort(ProgressListener *listener = NULL);
	virtual void lexicographicSortFourthElement()=0;
	void idSort();
	virtual void idSortFourthElement()=0; 
	virtual void updateIDs() ;
	void convertMapping(unsigned int mapping);
	virtual const std::vector<DictionaryEntry*> &getDictionaryEntryVector(unsigned int id, TripleComponentRole position)const;




public:

    size_t getNumberOfElements();

    uint64_t size();

	unsigned int getNsubjects();
	unsigned int getNobjects();
	unsigned int getNshared();

	unsigned int getMaxSubjectID();
	unsigned int getMaxObjectID();

	void populateHeader(Header &header, string rootNode);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);

    void import(Dictionary *other, ProgressListener *listener=NULL);

    IteratorUCharString *getSubjects();
    IteratorUCharString *getObjects();
    IteratorUCharString *getShared();

// ModifiableDictionary


	string getType();
	unsigned int getMapping();

	void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

// Private methods



public:

};


class DictIterator : public IteratorUCharString {
private:
	std::vector<DictionaryEntry *> &vector;
    size_t pos;
public:
	DictIterator(std::vector<DictionaryEntry *> &vector) : vector(vector), pos(0){

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
