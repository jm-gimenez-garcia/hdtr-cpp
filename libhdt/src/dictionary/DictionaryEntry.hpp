#ifndef _HDT_DICTIONARY_ENTRY_HPP__
#define _HDT_DICTIONARY_ENTRY_HPP__

#include <cstring>
#include <ext/hash_map>

//#define GOOGLE_HASH

#ifdef GOOGLE_HASH 
#include <sparsehash/sparse_hash_map>

using google::sparse_hash_map;      // namespace where class lives by default
using __gnu_cxx::hash;  // or __gnu_cxx::hash, or maybe tr1::hash, depending on your OS

#else
#ifndef WIN32
namespace std { using namespace __gnu_cxx; }
#endif
#endif

namespace hdt{

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

void setDictionaryEntry(DictionaryEntry& entry, const string& str_value, const unsigned int id_value=0);
unsigned int idFromString(const std::vector<DictionaryEntry*> entries, const std::string& str);

inline void setDictionaryEntry(DictionaryEntry& entry, const string& str_value, const unsigned int id_value/*=0*/){
	entry.id = id_value;
	entry.str = new char[str_value.length()+1];
	strcpy(entry.str, str_value.c_str());
}



typedef std::pair<const char*, DictionaryEntry *> DictEntryPair;

#ifdef GOOGLE_HASH 
typedef sparse_hash_map<const char *, DictionaryEntry *, hash<const char *>, str_cmp> DictEntryHash;
#else
typedef std::hash_map<const char *, DictionaryEntry *, __gnu_cxx::hash<const char *>, str_cmp> DictEntryHash;
#endif

typedef DictEntryHash::const_iterator DictEntryIt;

inline unsigned int idFromString(const std::vector<DictionaryEntry*> entries, const std::string& str){
	for (std::vector<DictionaryEntry*>::const_iterator it=entries.begin() ; it!=entries.end() ; ++it)
		if(std::strcmp((*it)->str, str.c_str()) == 0)
			return (*it)->id;
	return 0;
}

}

#endif
