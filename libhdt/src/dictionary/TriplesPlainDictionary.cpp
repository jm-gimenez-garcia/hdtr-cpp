
#include "TriplesPlainDictionary.hpp"
#include "HDTVocabulary.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"
#include "DictionaryEntry.hpp"

namespace hdt {

string TriplesPlainDictionary::getType()const {
	return HDTVocabulary::DICTIONARY_TYPE_PLAIN;
}

TriplesPlainDictionary::~TriplesPlainDictionary() {
	for(unsigned int i=0;i<predicates.size();i++) {
		delete [] predicates[i]->str;
		delete predicates[i];
	}
}

unsigned int TriplesPlainDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{	

	if(position!=PREDICATE)
		return BasePlainDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
		if(key.length()==0 || key.at(0) == '?') 
			return 0;
		
		return idFromString(predicates, key);

	}
}

size_t TriplesPlainDictionary::getNumberOfElements()const
{return BasePlainDictionary::getNumberOfElements() + predicates.size();}


// It is not necessary to override the virtual method startProcessing for TriplesPlainDictionary, beacause we MUST NOT CLEAR PREDICATES because it is already constructed before calling startProcessing

void TriplesPlainDictionary::push_back(DictionaryEntry* entry, DictionarySection pos){
	if (!entry)
		return;
	
	switch(pos)
	{
		case NOT_SHARED_PREDICATE:
		{
			const unsigned int nb_el = predicates.size();
			entry->id = nb_el+1;
			predicates.push_back(entry);
			sizeStrings+=strlen(entry->str);
			break;
		}
		case SHARED_SUBJECT:
		case NOT_SHARED_SUBJECT:
		case SHARED_OBJECT:
		case NOT_SHARED_OBJECT:
			BasePlainDictionary::push_back(entry, pos);
			break;
		default:
			throw runtime_error("Invalid DictionarySection in TriplesDictionary");
	}

}

void TriplesPlainDictionary::populateHeaderFourthElementNum(Header &header, string rootNode){
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNpredicates());
}
void TriplesPlainDictionary::populateHeaderFourthElementMaxId(Header &header, string rootNode){
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXPREDICATEID, getMaxPredicateID());
}

void TriplesPlainDictionary::saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//predicates
	for (unsigned int i = 0; i < predicates.size(); i++) {
		output << predicates[i]->str;
		output.put(marker); //character  to split file
		counter++;
		NOTIFYCOND(listener, "TriplesPlainDictionary saving predicates", counter, getNumberOfElements());
	}
}

void TriplesPlainDictionary::insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements)
{
	NOTIFYCOND(&iListener, "Dictionary loading predicates.", numLine, numElements);
	insert(line, NOT_SHARED_PREDICATE);	
}




IteratorUCharString *TriplesPlainDictionary::getPredicates() {
	if (dict_sorted)
		return new DictIterator(predicates);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
IteratorUCharString *TriplesPlainDictionary::getPredicates() const{
	if (dict_sorted)
		return new DictIterator_const(predicates);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
/*IteratorUCharString *TriplesPlainDictionary::getGraphs() {
	throw std::runtime_error("No graph section in this kind of dictionary");
	return NULL;
}*/

unsigned int TriplesPlainDictionary::getMaxPredicateID()const {
	return predicates.size();
}
/*unsigned int TriplesPlainDictionary::getMaxGraphID()const {
	throw std::runtime_error("No graph section in this kind of dictionary");
	return 0;
}*/

unsigned int TriplesPlainDictionary::getNpredicates()const 
{return predicates.size();}
/*unsigned int TriplesPlainDictionary::getNgraphs()const{
	throw std::runtime_error("No graph section in this kind of dictionary");
	return 0;
}*/



void TriplesPlainDictionary::updateIDs() 
{
	for (unsigned int i = 0; i < shared.size(); i++) 
		shared[i]->id = getGlobalId(i, SHARED_SUBJECT);
	for (unsigned int i = 0; i < subjects.size(); i++) 
		subjects[i]->id = getGlobalId(i, NOT_SHARED_SUBJECT);
	for (unsigned int i = 0; i < objects.size(); i++) 
		objects[i]->id = getGlobalId(i, NOT_SHARED_OBJECT);
	for (unsigned int i = 0; i < predicates.size(); i++) 
		predicates[i]->id = getGlobalId(mapping, i, NOT_SHARED_PREDICATE);
}

const vector<DictionaryEntry*>& TriplesPlainDictionary::getDictionaryEntryVector(unsigned int id, TripleComponentRole position) const
{return (position==PREDICATE) ? predicates : BasePlainDictionary::getDictionaryEntryVector(id, position);}

unsigned int TriplesPlainDictionary::getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position) const
{
	switch(position)
	{
		case NOT_SHARED_PREDICATE:
			return id+1;
		case SHARED_SUBJECT:
		case NOT_SHARED_SUBJECT:
		case SHARED_OBJECT:
		case NOT_SHARED_OBJECT:
			return BasePlainDictionary::getGlobalId(mapping_type, id, position);
		default:
			throw runtime_error("Invalid DictionarySection in TriplesDictionary");
			
	}
}
unsigned int TriplesPlainDictionary::getLocalId(unsigned int mapping_type, unsigned int id, TripleComponentRole position) const
{

	if(position==PREDICATE) 
		if (id <= predicates.size())
			return id-1;
		else
			throw std::runtime_error("This globalID does not correspond to a PREDICATE");
	else
		return BasePlainDictionary::getLocalId(mapping_type, id, position);
}

void TriplesPlainDictionary::updateID(unsigned int oldid, unsigned int newid, DictionarySection position) 
{
	switch(position)
	{
		case NOT_SHARED_PREDICATE:
			predicates[oldid]->id = newid;
			break;
		case SHARED_SUBJECT:
		case NOT_SHARED_SUBJECT:
		case SHARED_OBJECT:
		case NOT_SHARED_OBJECT:
			BasePlainDictionary::updateID(oldid, newid, position);
			break;
		default:
			throw runtime_error("Invalid DictionarySection in TriplesDictionary");
	}
}


}
