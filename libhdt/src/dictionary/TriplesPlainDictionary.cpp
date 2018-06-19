
#include "TriplesPlainDictionary.hpp"
#include "HDTVocabulary.hpp"
namespace hdt {


TriplesPlainDictionary::~TriplesPlainDictionary() {
	for(unsigned int i=0;i<predicates.size();i++) {
		delete [] predicates[i]->str;
		delete predicates[i];
	}
}

unsigned int TriplesPlainDictionary::stringToId(const std::string &key, const TripleComponentRole position)
{	

	if(position!=PREDICATE)
		return BasePlainDictionary::stringToId(key, position);
	else
	{
		DictEntryIt ret;
		if(key.length()==0)
			return 0;
		ret = hashPredicate.find(key.c_str());
   		return ret==hashPredicate.end() ? 0 : ret->second->id;
	}
}

size_t TriplesPlainDictionary::getNumberOfElements()const
{return BasePlainDictionary::getNumberOfElements() + predicates.size();}

void TriplesPlainDictionary::startProcessing(ProgressListener *listener)
{
	predicates.clear();
	BasePlainDictionary::startProcessing(listener);
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



unsigned int TriplesPlainDictionary::insertFourthElement(const std::string & str, const TripleComponentRole& pos)
{
	if(pos==PREDICATE) {
		DictEntryIt it = hashPredicate.find(str.c_str());
		if(it!=hashPredicate.end()) {
			//cout << "  existing predicate: " << str << endl;
			return it->second->id;
		} else {
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			entry->id = predicates.size()+1;
			sizeStrings += str.length();
			//cout << " Add new predicate: " << str.c_str() << endl;

			hashPredicate[entry->str] = entry;
			predicates.push_back(entry);
			return entry->id;
		}
	}
	return 0;
}


void TriplesPlainDictionary::insert(const string& str, const DictionarySection& pos) 
{
	if(pos!=NOT_SHARED_PREDICATE)
		BasePlainDictionary::insert(str, pos);
	else
	{
		if(str=="") return;
		DictionaryEntry *entry = new DictionaryEntry;
		entry->str = new char [str.length()+1];
		strcpy(entry->str, str.c_str());
		predicates.push_back(entry);
		//entry->id = predicates.size();
		hashPredicate[entry->str] = entry;
	}
}

IteratorUCharString *TriplesPlainDictionary::getPredicates() {
	return new DictIterator(predicates);
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
	for (unsigned int i = 0; i < predicates.size(); i++) 
		predicates[i]->id = getGlobalId(mapping, i, NOT_SHARED_PREDICATE);
	BasePlainDictionary::updateIDs();
}

const vector<DictionaryEntry*>& TriplesPlainDictionary::getDictionaryEntryVector(unsigned int id, TripleComponentRole position) const
{return (position==PREDICATE) ? predicates : BasePlainDictionary::getDictionaryEntryVector(id, position);}

unsigned int TriplesPlainDictionary::getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position) const
{

	if(position==NOT_SHARED_PREDICATE)
		return id+1;
	else
		return BasePlainDictionary::getGlobalId(mapping_type, id, position);
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
	if (position==NOT_SHARED_PREDICATE)
		predicates[oldid]->id = newid;
	else
		BasePlainDictionary::updateID(oldid, newid, position);
}



}
