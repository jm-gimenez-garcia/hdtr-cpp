#ifndef __GRAPHS_KYOTO_DICTIONARY_H__
#define __GRAPHS_KYOTO_DICTIONARY_H__


#ifdef HAVE_KYOTO


#include "BaseKyotoDictionary.hpp"
#include "GraphsDictionary.hpp"

namespace hdt {

class GraphsKyotoDictionary: public BaseKyotoDictionary, public GraphsDictionary {
public:
	//unsafe due to non-const kyotocabinet::TreeDB::count() and kyotocabinet::TreeDB::get() methods
	mutable kyotocabinet::TreeDB graphs;

public:
	GraphsKyotoDictionary(){};
	virtual ~GraphsKyotoDictionary();

    IteratorUCharString *getGraphs();
    IteratorUCharString *getGraphs()const;
	size_t getNumberOfElements()const;
	
	unsigned int getNunused()const;
	
	unsigned int stringToId(const std::string &key, const TripleComponentRole position)const;

	void startProcessing(ProgressListener *listener);
	unsigned int insert(const std::string & str, const TripleComponentRole pos);

	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const ;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const {return getGlobalId(mapping, id, position);}
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const {return getLocalId(mapping, id, position);}

	void dumpSizes(std::ostream &out);



};

}
#endif

#endif
