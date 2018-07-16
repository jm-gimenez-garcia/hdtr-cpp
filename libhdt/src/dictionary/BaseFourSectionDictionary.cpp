#include "BaseFourSectionDictionary.hpp"
#include "Iterator.hpp"
#include <istream>
#include <ostream>
#include <string>
//#include "../libdcs/CSD.h"
#include "../libdcs/CSD_PFC.h"
#include "ControlInformation.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"
#include "HDTVocabulary.hpp"
#include <algorithm>

using namespace std;

namespace hdt {

BaseFourSectionDictionary::BaseFourSectionDictionary(): blocksize(16), subjects(new csd::CSD_PFC()), objects(new csd::CSD_PFC()), shared(new csd::CSD_PFC()) {}

BaseFourSectionDictionary::BaseFourSectionDictionary(HDTSpecification & spec): blocksize(16), subjects(new csd::CSD_PFC()), objects(new csd::CSD_PFC()), shared(new csd::CSD_PFC())
{
	string blockSizeStr = "";
	try{
		spec.get("dict.block.size");
	}catch(exception& e){}

	if(blockSizeStr!=""){
		//blocksize = atoi((const char*)blockSizeStr.c_str());
	}
}

BaseFourSectionDictionary::~BaseFourSectionDictionary(){
	BaseFourSectionDictionary::clear_loc();
}


void BaseFourSectionDictionary::create()
{
	BaseFourSectionDictionary::clear_loc();
	subjects = new csd::CSD_PFC();
	objects = new csd::CSD_PFC();
	shared = new csd::CSD_PFC();
}

void BaseFourSectionDictionary::clear_loc()
{
	if (subjects!=NULL)
		{delete subjects; subjects=NULL;}
	if (objects!=NULL)
		{delete objects; objects=NULL;}
	if (shared!=NULL)
		{delete shared; shared=NULL;}
}


void BaseFourSectionDictionary::clear()
{
	clear_loc();
	BaseFourSectionDictionary::clear_loc();
}

csd::CSD *loadSection(IteratorUCharString *iterator, uint32_t blocksize, ProgressListener *listener) {
	return new csd::CSD_PFC(iterator, blocksize, listener);
	//return new csd::CSD_HTFC(iterator, blocksize, listener);
}






string BaseFourSectionDictionary::idToString(const unsigned int id, const TripleComponentRole position)const
{
	csd::CSD *section = getDictionarySection(id, position);

	unsigned int localid = getLocalId(id, position);

	if(localid<=section->getLength()) {
		const char * ptr = (const char *)section->extract(localid);
		if(ptr!=NULL) {
			string out = ptr;
			section->freeString((unsigned char*)ptr);
			return out;
		} else {
			//cout << "Not found: " << id << " as " << position << endl;
		}
	}

	return string();
}

unsigned int BaseFourSectionDictionary::stringToId(const string &key, const TripleComponentRole position)const
{
	unsigned int ret;

        if(key.length()==0)
		return 0;
        
	
	if(position==SUBJECT) {
		ret = shared->locate((const unsigned char *)key.c_str(), key.length());
		if( ret != 0) 
			return getGlobalId(ret,SHARED_SUBJECT);
		ret = subjects->locate((const unsigned char *)key.c_str(), key.length());
		if(ret != 0) 
			return getGlobalId(ret,NOT_SHARED_SUBJECT);
        	return 0;
	}
	else if (position==OBJECT){
		ret = shared->locate((const unsigned char *)key.c_str(), key.length());
		if( ret != 0) 
			return getGlobalId(ret,SHARED_OBJECT);
		ret = objects->locate((const unsigned char *)key.c_str(), key.length());
		if(ret != 0) 
			return getGlobalId(ret,NOT_SHARED_OBJECT);
        	return 0;
	}
	else
		return 0;
}





void BaseFourSectionDictionary::load(istream & input, ControlInformation & ci, ProgressListener *listener/*=NULL*/)
{
	IntermediateListener iListener(listener);
	clear();
	loadControlInfo(input, ci);
	loadShared(input, iListener);
	loadSubjects(input, iListener);
	loadFourthSection(input, iListener);
	loadObjects(input, iListener);
}

void BaseFourSectionDictionary::loadControlInfo(istream & input, ControlInformation & ci)
{
	string format = ci.getFormat();
	if(format!=getType()) {
		throw runtime_error("Trying to read a BaseFourSectionDictionary but the data is not BaseFourSectionDictionary");
	}
	//this->mapping = ci.getUint("mapping");
	this->mapping = MAPPING2;
	//this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseFourSectionDictionary::loadShared(istream & input, IntermediateListener& iListener)
{
	iListener.setRange(0,25);
	iListener.notifyProgress(0, "Dictionary read shared area.");
	shared = csd::CSD::load(input);
	if(shared==NULL){
		shared = new csd::CSD_PFC();
		throw runtime_error("Could not read shared.");
	}
	//shared = new csd::CSD_Cache(shared);
}
void BaseFourSectionDictionary::loadSubjects(istream & input, IntermediateListener& iListener)
{
	iListener.setRange(25,50);
	iListener.notifyProgress(0, "Dictionary read subjects.");
	subjects = csd::CSD::load(input);
	if(subjects==NULL){
		subjects = new csd::CSD_PFC();
		throw runtime_error("Could not read subjects.");
	}
	//subjects = new csd::CSD_Cache(subjects);
}
void BaseFourSectionDictionary::loadObjects(istream & input, IntermediateListener& iListener)
{
	iListener.setRange(75,100);
	iListener.notifyProgress(0, "Dictionary read objects.");
	objects = csd::CSD::load(input);
	if(objects==NULL){
		objects = new csd::CSD_PFC();
		throw runtime_error("Could not read objects.");
	}
	//objects = new csd::CSD_Cache(objects);
}


size_t BaseFourSectionDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/)
{
    size_t count=0;
    IntermediateListener iListener(listener);
	clear();	
    loadControlInfo(ptr, ptrMax, count);
    loadShared(ptr, ptrMax, count, iListener);
    loadSubjects(ptr, ptrMax, count, iListener);
    loadFourthSection(ptr, ptrMax, count, iListener);
    loadObjects(ptr, ptrMax, count, iListener);

    return count;
}
void BaseFourSectionDictionary::loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count){
    ControlInformation ci;
    count += ci.load(&ptr[count], ptrMax);

    //this->mapping = ci.getUint("mapping");
    this->mapping = MAPPING2;
    //this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseFourSectionDictionary::loadShared(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(0,25);
    iListener.notifyProgress(0, "Dictionary read shared area.");
    shared = csd::CSD::create(ptr[count]);
    if(shared==NULL){
        shared = new csd::CSD_PFC();
        throw runtime_error("Could not read shared.");
    }
    count += shared->load(&ptr[count], ptrMax);
    //shared = new csd::CSD_Cache(shared);
}

void BaseFourSectionDictionary::loadSubjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(25,50);
    iListener.notifyProgress(0, "Dictionary read subjects.");
    subjects = csd::CSD::create(ptr[count]);
    if(subjects==NULL){
        subjects = new csd::CSD_PFC();
        throw runtime_error("Could not read subjects.");
    }
    count += subjects->load(&ptr[count], ptrMax);
    //subjects = new csd::CSD_Cache(subjects);
}
void BaseFourSectionDictionary::loadObjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(75,100);
    iListener.notifyProgress(0, "Dictionary read objects.");
    objects = csd::CSD::create(ptr[count]);
    if(objects==NULL){
        objects = new csd::CSD_PFC();
        throw runtime_error("Could not read objects.");
    }
    count += objects->load(&ptr[count], ptrMax);
    //objects = new csd::CSD_Cache(objects);
    
}



void BaseFourSectionDictionary::import(Dictionary *other, ProgressListener *listener) 
{
	try 
	{
		IntermediateListener iListener(listener);
		clear();
		importSubjects(other, listener, iListener);
		importFourthSection(other, listener, iListener);
		importObjects(other, listener, iListener);
		importShared(other, listener, iListener);

		sizeStrings = other->size();
		mapping = other->getMapping();
	}
	catch (exception& e) 
	{
		clear();
		create();
		throw;
	}
}	
void BaseFourSectionDictionary::importSubjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	NOTIFY(listener, "DictionaryPFC loading subjects", 0, 100);
	iListener.setRange(0, 20);
	IteratorUCharString *itSubj = other->getSubjects();
	subjects = loadSection(itSubj, blocksize, &iListener);
	delete itSubj;
}
void BaseFourSectionDictionary::importObjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	NOTIFY(listener, "DictionaryPFC loading objects", 30, 90);
	iListener.setRange(21, 90);
	IteratorUCharString *itObj = other->getObjects();
	objects = loadSection(itObj, blocksize, &iListener);
	delete itObj;
}
void BaseFourSectionDictionary::importShared(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	NOTIFY(listener, "DictionaryPFC loading shared", 90, 100);
	iListener.setRange(90, 100);
	IteratorUCharString *itShared = other->getShared();
	shared = loadSection(itShared, blocksize, &iListener);
	delete itShared;
}

IteratorUCharString *BaseFourSectionDictionary::getSubjects() {
	return subjects->listAll();
}
IteratorUCharString *BaseFourSectionDictionary::getSubjects() const{
	return subjects->listAll();
}
IteratorUCharString *BaseFourSectionDictionary::getObjects() {
	return objects->listAll();
}
IteratorUCharString *BaseFourSectionDictionary::getObjects() const{
	return objects->listAll();
}
IteratorUCharString *BaseFourSectionDictionary::getShared(){
	return shared->listAll();
}
IteratorUCharString *BaseFourSectionDictionary::getShared()const{
	return shared->listAll();
}



void BaseFourSectionDictionary::save(ostream& output, ControlInformation & controlInformation, ProgressListener *listener){

	IntermediateListener iListener(listener);

	saveControlInfo(output, controlInformation);
	saveShared(output, iListener);
	saveSubjects(output, iListener);
	saveFourthSection(output, iListener);	
	saveObjects(output, iListener);	
}

void BaseFourSectionDictionary::saveControlInfo(ostream& output, ControlInformation & controlInformation)
{
	controlInformation.setFormat(HDTVocabulary::DICTIONARY_TYPE_FOUR);
	controlInformation.setUint("mapping", this->mapping);
	controlInformation.setUint("sizeStrings", this->sizeStrings);
	controlInformation.save(output);
}

void BaseFourSectionDictionary::saveShared(ostream& output, IntermediateListener& iListener)
{
	iListener.setRange(0,10);
	iListener.notifyProgress(0, "Dictionary save shared area.");
	shared->save(output);
}
void BaseFourSectionDictionary::saveSubjects(ostream& output, IntermediateListener& iListener)
{
	iListener.setRange(10,45);
	iListener.notifyProgress(0, "Dictionary save subjects.");
	subjects->save(output);
}
void BaseFourSectionDictionary::saveObjects(ostream& output, IntermediateListener& iListener)
{
	iListener.setRange(60,100);
	iListener.notifyProgress(0, "Dictionary save objects.");
	objects->save(output);
}


void BaseFourSectionDictionary::populateHeader(Header& header, string rootNode)
{
	header.insert(rootNode, HDTVocabulary::DICTIONARY_TYPE, getType());
#if 0
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSUBJECTS, getNsubjects());
	//header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNpredicates());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMOBJECTS, getNobjects());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXSUBJECTID, getMaxSubjectID());
	//header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXPREDICATEID, getMaxPredicateID());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXOBJECTTID, getMaxObjectID());
#endif
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSHARED, getNshared());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAPPING, this->mapping);
	header.insert(rootNode, HDTVocabulary::DICTIONARY_SIZE_STRINGS, size());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_BLOCK_SIZE, this->blocksize);
}

unsigned int BaseFourSectionDictionary::getNsubjects()const
{return shared->getLength()+subjects->getLength();}
unsigned int BaseFourSectionDictionary::getNobjects()const
{return shared->getLength()+objects->getLength();}
unsigned int BaseFourSectionDictionary::getNshared()const
{return shared->getLength();}

unsigned int BaseFourSectionDictionary::getMaxID()const
{
	unsigned int s = subjects->getLength();
	unsigned int o = objects->getLength();
	unsigned int sh = shared->getLength();
	unsigned int max = s>o ? s : o;

	return (mapping ==MAPPING2) ? sh+max : sh+s+o;
}
unsigned int BaseFourSectionDictionary::getMaxSubjectID()const
{return getNsubjects();}

unsigned int BaseFourSectionDictionary::getMaxObjectID()const{
	unsigned int s = subjects->getLength();
	unsigned int o = objects->getLength();
	unsigned int sh = shared->getLength();

	return (mapping == MAPPING2) ? sh+o : sh+s+o;
}
size_t BaseFourSectionDictionary::getNumberOfElements()const{
	return shared->getLength()+subjects->getLength()+objects->getLength();
}
uint64_t BaseFourSectionDictionary::size()const{
	return shared->getSize()+subjects->getSize()+objects->getSize();
}

string BaseFourSectionDictionary::getType()const
{return HDTVocabulary::DICTIONARY_TYPE_FOUR;}

unsigned int BaseFourSectionDictionary::getMapping()const
{return mapping;}




csd::CSD *BaseFourSectionDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
	if (position == SUBJECT) 
		return (id<=shared->getLength()) ? shared : subjects;
	else if (position == OBJECT)
		return (id<=shared->getLength()) ? shared : objects;

	throw runtime_error("Item not found");
}

unsigned int BaseFourSectionDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch (position) {
		case NOT_SHARED_SUBJECT:
			
			return shared->getLength()+id;
		case NOT_SHARED_OBJECT:
			return (mapping==MAPPING2) ? shared->getLength()+id : shared->getLength()+subjects->getLength()+id;
		case SHARED_SUBJECT:
		case SHARED_OBJECT:
			return id;
		default:
			throw runtime_error("Item not found");
			return 0;
	}
}


unsigned int BaseFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{

	const unsigned int sh_length = shared->getLength();
	const unsigned int sub_length = subjects->getLength();
	const unsigned int obj_length = objects->getLength();

	switch (position) {
		case SUBJECT:
			if(id<=sh_length)
				return id;
			else if(id <= sh_length + sub_length)
				return id - sh_length;
			else
				throw runtime_error("This globalID does not correspond to a SUBJECT");
			break;
		case OBJECT:
			if(id <= sh_length) 
				return id;
			else if (mapping==MAPPING1) 
			{	
				if ( (id <= sh_length + sub_length + obj_length) && (id > sh_length + sub_length) )
					return 2 + id - sh_length - sub_length;
				else
					throw runtime_error("This globalID does not correspond to an OBJECT with MAPPING1");

			}
			else if (mapping==MAPPING2)
			{
				if (id <= sh_length + obj_length)
					return id - sh_length ;
				else
					throw runtime_error("This globalID does not correspond to a OBJECT with MAPPING2");
			}
			else
				throw runtime_error("Uknown mapping");
			break;
		default:
			throw runtime_error("Item not found");
			return 0;
	}
}




void BaseFourSectionDictionary::getSuggestions(const char *base, TripleComponentRole role, vector<string> &out, int maxResults)
{

	vector<string> v1,v2;
	shared->fillSuggestions(base, v1, maxResults);
	if(role==SUBJECT) {
		subjects->fillSuggestions(base, v2, maxResults);
	} else if(role==OBJECT){
		objects->fillSuggestions(base, v2, maxResults);
	}

	// Merge results from shared and subjects/objects keeping order
	merge(v1.begin(),v1.end(), v2.begin(), v2.end(), back_inserter(out));

	// Remove possible extra items
	if(out.size()>maxResults) {
		out.resize(maxResults);
	}
}


IteratorUCharString* BaseFourSectionDictionary::getSuggestions(const char *prefix, TripleComponentRole role){

	IteratorUCharString * sharedIt = shared->getSuggestions(prefix);
	IteratorUCharString * subjectIt;
	IteratorUCharString * objectIt;

	// Merge results from shared and subjects/objects keeping order
	if(role==SUBJECT) {
		subjectIt = subjects->getSuggestions(prefix);
		return new MergeIteratorUCharString(sharedIt,subjectIt);
	} else if(role==OBJECT){
		objectIt = objects->getSuggestions(prefix);
		return new MergeIteratorUCharString(sharedIt,objectIt);
	}
	return NULL;

}


IteratorUInt *BaseFourSectionDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role){

	IteratorUInt * sharedIt = shared->getIDSuggestions(prefix);
	IteratorUInt * subjectIt;
	IteratorUInt * objectIt;

	// Merge results from shared and subjects/objects keeping order
	if(role==SUBJECT) {
		subjectIt = subjects->getIDSuggestions(prefix);
		return new SequentialIteratorUInt(sharedIt,subjectIt,shared->getLength());
	} else if(role==OBJECT){
		objectIt = objects->getIDSuggestions(prefix);
		return new SequentialIteratorUInt(sharedIt,objectIt,shared->getLength());
	}
	return NULL;

}


}
