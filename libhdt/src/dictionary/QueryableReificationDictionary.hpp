#ifndef HDT_QUERYABLE_REIFICATION_DICTIONARY_HPP_
#define HDT_QUERYABLE_REIFICATION_DICTIONARY_HPP_

#include "BaseReificationDictionary.hpp"
#include "TriplesDictionary.hpp"
#include "GraphsDictionary.hpp"
#include "HDTVocabulary.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"


namespace hdt{

class HDTSpecification;

class QueryableReificationDictionary : public BaseReificationDictionary{
private:
	TriplesDictionary* triplesDict;
	GraphsDictionary* graphsDict;

public:
	QueryableReificationDictionary(HDTSpecification &specification);
	QueryableReificationDictionary();
	~QueryableReificationDictionary();
	//from Dictionary:
	std::string getType()const;

	const TriplesDictionary* getTriplesDictionaryPtr()const;
	const GraphsDictionary* getGraphsDictionaryPtr()const;
private:
	TriplesDictionary* getTriplesDictionaryPtr();
	GraphsDictionary* getGraphsDictionaryPtr();

};



}

#endif
