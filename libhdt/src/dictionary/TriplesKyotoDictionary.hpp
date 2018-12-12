#ifndef __TRIPLES_KYOTO_DICTIONARY_H__
#define __TRIPLES_KYOTO_DICTIONARY_H__ 

#ifdef HAVE_KYOTO
#include "BaseKyotoDictionary.hpp"
#include "ModifiableTriplesDictionary.hpp"

namespace hdt {

class TriplesKyotoDictionary: public BaseKyotoDictionary, public ModifiableTriplesDictionary {
public:
	//very unsafe due to non-const kyotocabinet::TreeDB::count() and kyotocabinet::TreeDB::get() methods
	mutable kyotocabinet::TreeDB predicates;

public:
	TriplesKyotoDictionary(){};
	virtual ~TriplesKyotoDictionary();

    IteratorUCharString *getPredicates();
    IteratorUCharString *getPredicates()const;
    //IteratorUCharString *getGraphs();
	size_t getNumberOfElements()const;

	unsigned int getNpredicates()const;
	//unsigned int getNgraphs()const;
	
	unsigned int getMaxPredicateID()const;
	//unsigned int getMaxGraphID()const;

	unsigned int stringToId(const std::string &key, const TripleComponentRole position)const;

	void startProcessing(ProgressListener *listener);
	unsigned int insert(const std::string & str, const TripleComponentRole pos);

	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const ;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const {return getGlobalId(mapping, id, position);}
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const ;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const {return getLocalId(mapping, id, position);}

	void dumpSizes(std::ostream &out);

};
}

#endif



#endif
