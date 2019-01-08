#include <istream>
#include <ostream>
#include <string>
#include "BaseFourSectionDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "Iterator.hpp"
#include "../libdcs/CSD_PFC.h"
#include "../libdcs/CSD_Cache2.h"
#include "HDTListener.hpp"

using namespace std;
namespace hdt {


TriplesFourSectionDictionary::TriplesFourSectionDictionary() : predicates(new csd::CSD_PFC()){}

TriplesFourSectionDictionary::TriplesFourSectionDictionary(HDTSpecification & spec): predicates(new csd::CSD_PFC()){}

TriplesFourSectionDictionary::~TriplesFourSectionDictionary(){
	clear_loc();
}

void TriplesFourSectionDictionary::clear_loc()
{
	if (predicates!=NULL){
		delete predicates; predicates=NULL;
	}	
}

void TriplesFourSectionDictionary::create()
{
	clear_loc();
	predicates = new csd::CSD_PFC();
	BaseFourSectionDictionary::create();
}

size_t TriplesFourSectionDictionary::getNumberOfElements()const{
	return BaseFourSectionDictionary::getNumberOfElements()+predicates->getLength();
}

uint64_t TriplesFourSectionDictionary::size()const{
	return BaseFourSectionDictionary::size()+predicates->getLength();
}


unsigned int TriplesFourSectionDictionary::stringToId(const string &key, const TripleComponentRole position)const
{
	if (position!=PREDICATE)
		return BaseFourSectionDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
        if(key.length()==0)
			return 0;
		ret = predicates->locate((const unsigned char *)key.c_str(), key.length());
		return (ret==0) ? 0 : getGlobalId(ret, NOT_SHARED_PREDICATE);
	}
}


void TriplesFourSectionDictionary::loadFourthSection(istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50,75);
	iListener.notifyProgress(0, "Dictionary read predicates.");
	predicates = csd::CSD::load(input);
	if(predicates==NULL){
		predicates = new csd::CSD_PFC();
		throw runtime_error("Could not read predicates.");
	}
	predicates = new csd::CSD_Cache2(predicates);
}

void TriplesFourSectionDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read predicates.");
	predicates = csd::CSD::create(ptr[count]);
    if(predicates==NULL){
        predicates = new csd::CSD_PFC();
        throw runtime_error("Could not read predicates.");
    }
    count += predicates->load(&ptr[count], ptrMax);
    predicates = new csd::CSD_Cache2(predicates);
}

void TriplesFourSectionDictionary::importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	if(TriplesDictionary* other_td = dynamic_cast<TriplesDictionary*>(other))
	{
		NOTIFY(listener, "DictionaryPFC loading predicates", 25, 30);
		iListener.setRange(20, 21);
		IteratorUCharString *itPred = other_td->getPredicates();
		predicates = loadSection(itPred, blocksize, &iListener);
		delete itPred;
	}
	else
		throw runtime_error("Downcast error from Dictionary to TriplesDictionary.");
}



IteratorUCharString *TriplesFourSectionDictionary::getPredicates()
{return predicates->listAll();}
IteratorUCharString *TriplesFourSectionDictionary::getPredicates()const
{return predicates->listAll();}

/*IteratorUCharString *TriplesFourSectionDictionary::getGraphs(){
	throw runtime_error("No graph section in this kind of dictionary");
	return NULL;
}*/

void TriplesFourSectionDictionary::saveFourthSection(ostream& output, IntermediateListener& iListener){

	iListener.setRange(45,60);
	iListener.notifyProgress(0, "Dictionary save predicates.");
	predicates->save(output);
}

unsigned int TriplesFourSectionDictionary::getNpredicates()const
{return predicates->getLength();}

/*unsigned int TriplesFourSectionDictionary::getNgraphs()const{
	
	throw runtime_error("No graph section in this kind of dictionary");
	return 0;
}*/

unsigned int TriplesFourSectionDictionary::getMaxPredicateID()const
{return predicates->getLength();}

/*unsigned int TriplesFourSectionDictionary::getMaxGraphID()const{
	throw runtime_error("No graph section in this kind of dictionary");
	return 0;
}*/



csd::CSD *TriplesFourSectionDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
	return (position==PREDICATE) ? predicates : BaseFourSectionDictionary::getDictionarySection(id, position);
}

unsigned int TriplesFourSectionDictionary::getGlobalId(unsigned int map, unsigned int id, DictionarySection position) const{
	switch(position)
	{
		case NOT_SHARED_PREDICATE:
			return id;
		case SHARED_SUBJECT:
		case NOT_SHARED_SUBJECT:
		case SHARED_OBJECT:
		case NOT_SHARED_OBJECT:
			return BaseFourSectionDictionary::getGlobalId(map, id, position);
		default:
			throw runtime_error("Invalid DictionarySection in TriplesDictionary");
	}
}


unsigned int TriplesFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{

	if(position==PREDICATE)
		if (id <= predicates->getLength())
			return id;
		else
			throw runtime_error("This globalID does not correspond to a PREDICATE");
	else
		return BaseFourSectionDictionary::getLocalId(mapping, id, position);

}


void TriplesFourSectionDictionary::getSuggestions(const char *base, TripleComponentRole role, vector<string> &out, int maxResults)
{
	if(role==PREDICATE) 
		predicates->fillSuggestions(base, out, maxResults);
	else
		BaseFourSectionDictionary::getSuggestions(base, role, out, maxResults);	
}

IteratorUCharString* TriplesFourSectionDictionary::getSuggestions(const char *prefix, TripleComponentRole role){
	if(role==PREDICATE) 
		return predicates->getSuggestions(prefix);
	else
		return BaseFourSectionDictionary::getSuggestions(prefix, role);
}

IteratorUInt *TriplesFourSectionDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role){
	if(role==PREDICATE) 
		return predicates->getIDSuggestions(prefix);

	return BaseFourSectionDictionary::getIDSuggestions(prefix, role);
}


}
