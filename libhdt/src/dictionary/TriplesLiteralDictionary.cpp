#include "BaseLiteralDictionary.hpp"
#include "TriplesLiteralDictionary.hpp"

#if HAVE_CDS

#include <HDTVocabulary.hpp>
#include <Iterator.hpp>
#include "../libdcs/CSD_PFC.h"
#include "../libdcs/CSD_Cache.h"
#include "../libdcs/CSD_Cache2.h"
#include <Header.hpp>


namespace hdt {

TriplesLiteralDictionary::TriplesLiteralDictionary() : predicates(new csd::CSD_PFC()){}
TriplesLiteralDictionary::TriplesLiteralDictionary(HDTSpecification & spec) : BaseLiteralDictionary(spec), predicates(new csd::CSD_PFC()){}
	

TriplesLiteralDictionary::~TriplesLiteralDictionary(){
	clear_loc();
}

void TriplesLiteralDictionary::clear_loc()
{
	if (predicates!=NULL){
		delete predicates; predicates=NULL;
	}	
}
void TriplesLiteralDictionary::create()
{
	clear_loc();
	predicates = new csd::CSD_PFC();
	BaseLiteralDictionary::create();
}


unsigned int TriplesLiteralDictionary::getNpredicates()const
{return predicates->getLength();}

unsigned int TriplesLiteralDictionary::getMaxPredicateID()const
{return predicates->getLength();}



unsigned int TriplesLiteralDictionary::stringToId(const std::string &key, const TripleComponentRole position)const {

	if (key.length() == 0) 
		return 0;

	if(position != PREDICATE)
		return BaseLiteralDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
		ret = predicates->locate((const unsigned char *) key.c_str(), key.length());
		if (ret != 0) 
			return getGlobalId(ret, NOT_SHARED_PREDICATES);
	}
    return 0;
}


void TriplesLiteralDictionary::loadFourthSection(std::istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50, 75);
	iListener.notifyProgress(0, "Dictionary read predicates.");

	predicates = csd::CSD::load(input);
	if (predicates == NULL) {
		predicates = new csd::CSD_PFC();
		throw std::runtime_error("Could not read predicates.");
	}
	predicates = new csd::CSD_Cache2(predicates);
	
}

void TriplesLiteralDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read predicates.");
    predicates = csd::CSD::create(ptr[count]);
    if(predicates==NULL){
        predicates = new csd::CSD_PFC();
        throw std::runtime_error("Could not read predicates.");
    }
    count += predicates->load(&ptr[count], ptrMax);
    predicates = new csd::CSD_Cache(predicates);
}

void TriplesLiteralDictionary::importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)
{
	if(TriplesDictionary* other_td = dynamic_cast<TriplesDictionary*>(other))
	{
		//NOTIFY(listener, "DictionaryPFC loading predicates", 25, 30);
		IteratorUCharString *itPred = other_td->getPredicates();
		iListener.setRange(20, 21);
		predicates = loadSectionPFC(itPred, blocksize, &iListener);
		predicates = new csd::CSD_Cache2(predicates);
		delete itPred;
	}
	else
		throw std::runtime_error("Downcast error from Dictionary to TriplesDictionary.");
}
void TriplesLiteralDictionary::saveFourthSection(std::ostream & output, IntermediateListener& iListener)
{
	iListener.setRange(45, 60);
	iListener.notifyProgress(0, "Dictionary save predicates.");
	predicates->save(output);
}

IteratorUCharString *TriplesLiteralDictionary::getPredicates()
{throw std::logic_error("Not implemented");}
IteratorUCharString *TriplesLiteralDictionary::getPredicates()const
{throw std::logic_error("Not implemented");}
/*IteratorUCharString *TriplesLiteralDictionary::getGraphs() {
	throw std::runtime_error("No graph section in this kind of dictionary");
	return NULL;
}
unsigned int TriplesLiteralDictionary::getNgraphs()const{
	throw std::runtime_error("No graph section in this kind of dictionary");
	return 0;
}
unsigned int TriplesLiteralDictionary::getMaxGraphID()const{
	throw std::runtime_error("No graph section in this kind of dictionary");
	return 0;
}*/


void TriplesLiteralDictionary::populateHeaderNumFourthSection(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNpredicates());}


void TriplesLiteralDictionary::populateHeaderMaxFourthSectionId(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXPREDICATEID, getMaxPredicateID());}

uint64_t TriplesLiteralDictionary::size()const
{return BaseLiteralDictionary::size() + predicates->getSize();}

unsigned int TriplesLiteralDictionary::getMaxID()const
{return BaseLiteralDictionary::getMaxID();}


size_t TriplesLiteralDictionary::getNumberOfElements()const {
	return BaseLiteralDictionary::getNumberOfElements() + predicates->getLength();
}

csd::CSD *TriplesLiteralDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const
{
	if(position!=PREDICATE)
		return BaseLiteralDictionary::getDictionarySection(id, position);
	else
		return predicates;
}


unsigned int TriplesLiteralDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch(position)
	{
		case NOT_SHARED_PREDICATES:
			return id;
		case SHARED_SUBJECTS:
		case NOT_SHARED_SUBJECTS:
		case SHARED_OBJECTS:
		case NOT_SHARED_OBJECTS:
			return BaseLiteralDictionary::getGlobalId(mapping, id, position);
		default:
			throw runtime_error("Invalid DictionarySection in TriplesDictionary");
	}
}



unsigned int TriplesLiteralDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	if(position==PREDICATE)
		if (id <= predicates->getLength())
			return id;
		else
			throw std::runtime_error("This globalID does not correspond to a PREDICATE");
	else
		return BaseLiteralDictionary::getLocalId(mapping, id, position);
}

void TriplesLiteralDictionary::getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults)
{
	if (role == PREDICATE) 
		predicates->fillSuggestions(base, out, maxResults);
	else
		BaseLiteralDictionary::getSuggestions(base, role, out, maxResults);
}

}
#endif
