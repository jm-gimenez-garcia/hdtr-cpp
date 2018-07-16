#include "QueryableReificationDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include "HDTSpecification.hpp"

namespace hdt{

QueryableReificationDictionary::QueryableReificationDictionary() : triplesDict(new TriplesFourSectionDictionary()), graphsDict(new GraphsFourSectionDictionary()){}
QueryableReificationDictionary::QueryableReificationDictionary(HDTSpecification &spec) : BaseReificationDictionary(spec), triplesDict(new TriplesFourSectionDictionary(spec)), graphsDict(new GraphsFourSectionDictionary(spec)){}

QueryableReificationDictionary::~QueryableReificationDictionary(){
	if(triplesDict){
		delete triplesDict;
		triplesDict=NULL;
	}
	if(graphsDict){
		delete graphsDict;
		graphsDict=NULL;
	}
}


const TriplesDictionary* QueryableReificationDictionary::getTriplesDictionaryPtr()const
{
	return static_cast<const TriplesDictionary*>(triplesDict);}

const GraphsDictionary* QueryableReificationDictionary::getGraphsDictionaryPtr()const
{return static_cast<const GraphsDictionary*>(graphsDict);}

TriplesDictionary* QueryableReificationDictionary::getTriplesDictionaryPtr()
{return static_cast<TriplesDictionary*>(triplesDict);}

GraphsDictionary* QueryableReificationDictionary::getGraphsDictionaryPtr()
{return static_cast<GraphsDictionary*>(graphsDict);}



std::string QueryableReificationDictionary::getType()const
{return HDTVocabulary::DICTIONARY_TYPE_QUERYABLEREIFICATION;}


}
