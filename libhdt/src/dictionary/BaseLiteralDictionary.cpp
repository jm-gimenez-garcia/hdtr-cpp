
#if HAVE_CDS

#include "BaseLiteralDictionary.hpp"
#include <HDTVocabulary.hpp>
#include <ControlInformation.hpp>
#include <Iterator.hpp>
#include "../libdcs/CSD_PFC.h"
//#include "../libdcs/CSD_HTFC.h"
#include "../libdcs/CSD_FMIndex.h"
#include "../libdcs/CSD_Cache.h"
#include <Header.hpp>

namespace hdt {

BaseLiteralDictionary::BaseLiteralDictionary() : 
	subjects(new csd::CSD_PFC()),
	objectsNotLiterals(new csd::CSD_PFC()), 
	objectsLiterals(new csd::CSD_FMIndex()), 
	shared(new csd::CSD_PFC()), 
	blocksize(8) {}

BaseLiteralDictionary::BaseLiteralDictionary(HDTSpecification & spec) : 
	subjects(new csd::CSD_PFC()),
	objectsNotLiterals(new csd::CSD_PFC()), 
	objectsLiterals(new csd::CSD_FMIndex()), 
	shared(new csd::CSD_PFC()), 
	blocksize(8) 
{
	string blockSizeStr = "";
	try{
		blockSizeStr = spec.get("dict.block.size");
	}catch(exception& e){}
	if (blockSizeStr != "") {
		blocksize = atoi(blockSizeStr.c_str());
	}
}

BaseLiteralDictionary::~BaseLiteralDictionary() {
	BaseLiteralDictionary::clear_loc();
}


void BaseLiteralDictionary::create()
{
	BaseLiteralDictionary::clear();
	subjects = new csd::CSD_PFC();
	objectsNotLiterals = new csd::CSD_PFC();
	objectsLiterals = new csd::CSD_FMIndex();
	shared = new csd::CSD_PFC();
}

void BaseLiteralDictionary::clear_loc()
{
	if (subjects!=NULL){
		delete subjects; subjects=NULL;
	}
	if (objectsNotLiterals!=NULL){
		delete objectsNotLiterals; objectsNotLiterals=NULL;
	}
	if (objectsLiterals!=NULL){
		delete objectsLiterals; objectsLiterals=NULL;
	}
	if (shared!=NULL){
		delete shared; shared=NULL;
	}
}

void BaseLiteralDictionary::clear()
{
	clear_loc();
	BaseLiteralDictionary::clear_loc();
}



csd::CSD *loadSectionPFC(IteratorUCharString *iterator, uint32_t blocksize, ProgressListener *listener) {
return new csd::CSD_PFC(iterator, blocksize, listener);
//return new csd::CSD_HTFC(iterator, blocksize, listener);
}

/*
* use_sample = true ; enables to locate substrings.
*/
csd::CSD *loadSectionFMIndex(IteratorUCharString *iterator, bool sparse_bitsequence, int bparam, size_t bwt_sample, bool use_sample, hdt::ProgressListener *listener) {
return new csd::CSD_FMIndex(iterator, sparse_bitsequence, bparam, bwt_sample, use_sample, listener);
}

std::string BaseLiteralDictionary::idToString(const unsigned int id, const TripleComponentRole position)const {
csd::CSD *section = getDictionarySection(id, position);
unsigned int localid = getLocalId(id, position);

if (localid <= section->getLength()) {
	const char * ptr = (const char *) section->extract(localid);
	if (ptr != NULL) {
		const string out(ptr);
		//section->freeString((unsigned char*)ptr);
					// TODO: find out why overloaded function 'freeString' isn't getting called, this solves it for now
					delete [] ptr;
		return out;
	} else {
		//cout << "Not found: " << id << " as " << position << endl;
	}
}

return string();
}

unsigned int BaseLiteralDictionary::stringToId(const std::string &key, const TripleComponentRole position)const {
unsigned int ret;

if (key.length() == 0) {
	return 0;
}

switch (position) {
case SUBJECT:
	ret = shared->locate((const unsigned char *) key.c_str(), key.length());
	if (ret != 0) {
		return getGlobalId(ret, SHARED_SUBJECTS);
	}
	ret = subjects->locate((const unsigned char *) key.c_str(), key.length());
	if (ret != 0) {
		return getGlobalId(ret, NOT_SHARED_SUBJECTS);
	}
	return 0;

case OBJECT:
	if (key.at(0) == '"') {
		ret = objectsLiterals->locate((const unsigned char *) key.c_str(),
				key.length());
		if (ret != 0) {
			return getGlobalId(ret, NOT_SHARED_OBJECTS);
		}
		return 0;
	} else {
		ret = shared->locate((const unsigned char *) key.c_str(), key.length());
		if (ret != 0) {
			return getGlobalId(ret, SHARED_OBJECTS);
		}
		ret = objectsNotLiterals->locate((const unsigned char *) key.c_str(), key.length());
		if (ret != 0) {
			return getGlobalId(ret, NOT_SHARED_OBJECTS)+	objectsLiterals->getLength();
		}
		return 0;
	}
default:
	return 0;

}
return 0;
}

void BaseLiteralDictionary::loadControlInfo(std::istream & input, ControlInformation & ci)
{
std::string format = ci.getFormat();
if(format!=getType()) {
	throw std::runtime_error("Trying to read a BaseLiteralDictionary but the data is not BaseLiteralDictionary");
}
this->mapping = ci.getUint("mapping");
this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseLiteralDictionary::loadShared(std::istream & input, IntermediateListener& iListener)
{
iListener.setRange(0, 25);
iListener.notifyProgress(0, "Dictionary read shared area.");
shared = csd::CSD::load(input);
if (shared == NULL) {
	shared = new csd::CSD_PFC();
	throw std::runtime_error("Could not read shared sectionsss.");
}
}

void BaseLiteralDictionary::loadSubjects(std::istream & input, IntermediateListener& iListener)
{
iListener.setRange(25, 50);
iListener.notifyProgress(0, "Dictionary read subjects.");
subjects = csd::CSD::load(input);
if (subjects == NULL) {
	subjects = new csd::CSD_PFC();
	throw std::runtime_error("Could not read subjects.");
}
subjects = new csd::CSD_Cache(subjects);
}


void BaseLiteralDictionary::loadObjects(std::istream & input, IntermediateListener& iListener)
{
iListener.setRange(75, 100);
iListener.notifyProgress(0, "Dictionary read objects.");

//load Objects Literal
objectsLiterals = csd::CSD::load(input);
if (objectsLiterals == NULL) {
	objectsLiterals = new csd::CSD_FMIndex();
	throw std::runtime_error("Could not read objects Literals.");
}
objectsLiterals = new csd::CSD_Cache(objectsLiterals);

// loadObjects Not Literal
objectsNotLiterals = csd::CSD::load(input);
if (objectsNotLiterals == NULL) {
	objectsNotLiterals = new csd::CSD_PFC();
	throw std::runtime_error("Could not read objects not Literals.");
}
objectsNotLiterals = new csd::CSD_Cache(objectsNotLiterals);
}

void BaseLiteralDictionary::load(std::istream & input, ControlInformation & ci,	ProgressListener *listener/*=NULL*/) {

IntermediateListener iListener(listener);
clear();
loadControlInfo(input, ci);
loadShared(input, iListener);
loadSubjects(input, iListener);
loadFourthSection(input, iListener);
loadObjects(input, iListener);
}



void BaseLiteralDictionary::loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count)
{
ControlInformation ci;
count += ci.load(&ptr[count], ptrMax);

this->mapping = ci.getUint("mapping");
this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseLiteralDictionary::loadShared(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
iListener.setRange(0,25);
iListener.notifyProgress(0, "Dictionary read shared area.");
shared = csd::CSD::create(ptr[count]);
if(shared==NULL){
	shared = new csd::CSD_PFC();
	throw std::runtime_error("Could not read shared.");
}
count += shared->load(&ptr[count], ptrMax);
shared = new csd::CSD_Cache(shared);
}

void BaseLiteralDictionary::loadSubjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
iListener.setRange(25,50);
iListener.notifyProgress(0, "Dictionary read subjects.");
subjects = csd::CSD::create(ptr[count]);
if(subjects==NULL){
	subjects = new csd::CSD_PFC();
	throw std::runtime_error("Could not read subjects.");
}
count += subjects->load(&ptr[count], ptrMax);
subjects = new csd::CSD_Cache(subjects);
}

void BaseLiteralDictionary::loadObjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
iListener.setRange(75,85);
iListener.notifyProgress(0, "Dictionary read objects literals.");
objectsLiterals = csd::CSD::create(ptr[count]);
if(objectsLiterals==NULL){
	objectsLiterals = new csd::CSD_PFC();
	throw std::runtime_error("Could not read object Literals.");
}
count += objectsLiterals->load(&ptr[count], ptrMax);
objectsLiterals = new csd::CSD_Cache(objectsLiterals);

iListener.setRange(75,100);
iListener.notifyProgress(0, "Dictionary read objects Rest.");
objectsNotLiterals = csd::CSD::create(ptr[count]);
if(objectsNotLiterals==NULL){
	objectsNotLiterals = new csd::CSD_PFC();
	throw std::runtime_error("Could not read objects Not Literals.");
}
count += objectsNotLiterals->load(&ptr[count], ptrMax);
objectsNotLiterals = new csd::CSD_Cache(objectsNotLiterals);

}


size_t BaseLiteralDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/)
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


void BaseLiteralDictionary::importShared(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)
{
	//NOTIFY(listener, "DictionaryPFC loading shared", 90, 100);
	IteratorUCharString *itShared = other->getShared();
	iListener.setRange(90, 100);
	shared = loadSectionPFC(itShared, blocksize, &iListener);
	shared = new csd::CSD_Cache(shared);
	delete itShared;
}

void BaseLiteralDictionary::importSubjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)
{
	//NOTIFY(listener, "DictionaryPFC loading subjects", 0, 100);
	IteratorUCharString *itSubj = other->getSubjects();
	iListener.setRange(0, 20);
	subjects = loadSectionPFC(itSubj, blocksize, &iListener);
	subjects = new csd::CSD_Cache(subjects);
	delete itSubj;
}

void BaseLiteralDictionary::importObjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)
{
	//NOTIFY(listener, "DictionaryPFC loading objects", 30, 90);
	iListener.setRange(21, 50);
	IteratorUCharString *itObj = other->getObjects();
	LiteralIterator litIt(itObj);

	objectsLiterals = loadSectionFMIndex(&litIt, false, 4, 64, true, &iListener);
	objectsLiterals = new csd::CSD_Cache(objectsLiterals);
	litIt.doContinue();

	iListener.setRange(50, 90);
	objectsNotLiterals = loadSectionPFC(&litIt, blocksize, &iListener);
	objectsNotLiterals = new csd::CSD_Cache(objectsNotLiterals);
	delete itObj;
}


void BaseLiteralDictionary::import(	Dictionary *other, ProgressListener *listener) {
try {
	IntermediateListener iListener(listener);
	clear();
	importShared(other,listener, iListener);
	importSubjects(other,listener, iListener);
	importFourthSection(other,listener, iListener);
	importObjects(other,listener, iListener);

	sizeStrings = other->size();
	mapping = other->getMapping();
} catch (std::exception& e) {
	create();
	throw;
}
}

IteratorUCharString *BaseLiteralDictionary::getSubjects() {
throw std::logic_error("Not implemented");
}

IteratorUCharString *BaseLiteralDictionary::getSubjects() const{
throw std::logic_error("Not implemented");
}

IteratorUCharString *BaseLiteralDictionary::getObjects() {
throw std::logic_error("Not implemented");
}

IteratorUCharString *BaseLiteralDictionary::getObjects() const{
throw std::logic_error("Not implemented");
}

IteratorUCharString *BaseLiteralDictionary::getShared(){
throw std::logic_error("Not implemented");
}
IteratorUCharString *BaseLiteralDictionary::getShared()const{
throw std::logic_error("Not implemented");
}

uint32_t BaseLiteralDictionary::substringToId(unsigned char *s, uint32_t len, uint32_t **occs){
uint32_t dummy;
return this->substringToId(s, len, 0, 0, true, occs, &dummy);
}

uint32_t BaseLiteralDictionary::substringToId(unsigned char *s, uint32_t len, uint32_t offset, uint32_t limit, bool deduplicate, uint32_t **occs, uint* num_occ){

if(len==0) {
	return 0;
}

csd::CSD_FMIndex *fmIndex=NULL;

csd::CSD_Cache *cache = dynamic_cast<csd::CSD_Cache *>(objectsLiterals);
if(cache!=NULL) {
	fmIndex = dynamic_cast<csd::CSD_FMIndex  *>(cache->getChild());
} else {
	fmIndex = dynamic_cast<csd::CSD_FMIndex  *>(objectsLiterals);
}

if(fmIndex!=NULL) {
	uint32_t ret = fmIndex->locate_substring(s, len, offset, limit, deduplicate, occs, num_occ);
	for (int i=0;i<*num_occ;i++){
		(*occs)[i] = this->getGlobalId((*occs)[i], NOT_SHARED_OBJECTS);
	}
	return ret;
}

cerr << "Warning, trying to call BaseLiteralDictionary::substringToId() but it was not an FM-Index.";
return 0;
}

void BaseLiteralDictionary::saveControlInfo(std::ostream& output, ControlInformation& controlInformation)
{
controlInformation.setFormat(HDTVocabulary::DICTIONARY_TYPE_LITERAL);
controlInformation.setUint("mapping", mapping);
controlInformation.setUint("sizeStrings", sizeStrings);
controlInformation.save(output);
}

void BaseLiteralDictionary::saveShared(std::ostream & output, IntermediateListener& iListener)
{
iListener.setRange(0, 10);
iListener.notifyProgress(0, "Dictionary save shared area.");
shared->save(output);
}

void BaseLiteralDictionary::saveSubjects(std::ostream & output, IntermediateListener& iListener)
{
iListener.setRange(10, 45);
iListener.notifyProgress(0, "Dictionary save subjects.");
subjects->save(output);
}

void BaseLiteralDictionary::saveObjects(std::ostream & output, IntermediateListener& iListener)
{
iListener.setRange(60, 80);
iListener.notifyProgress(0, "Dictionary save literal objects.");
objectsLiterals->save(output);

iListener.setRange(80, 100);
iListener.notifyProgress(0, "Dictionary save non literal objects.");
objectsNotLiterals->save(output);
}

void BaseLiteralDictionary::save(std::ostream & output,	ControlInformation & controlInformation, ProgressListener *listener) {


IntermediateListener iListener(listener);
saveControlInfo(output, controlInformation);
saveShared(output, iListener);
saveSubjects(output, iListener);
saveFourthSection(output, iListener);
saveObjects(output, iListener);
}

void BaseLiteralDictionary::populateHeader(Header & header, string rootNode) {
header.insert(rootNode, HDTVocabulary::DICTIONARY_TYPE,
		HDTVocabulary::DICTIONARY_TYPE_LITERAL);
header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSUBJECTS,
		getNsubjects());

populateHeaderNumFourthSection(header, rootNode);

header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMOBJECTS,
		getNobjects());
header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSHARED, getNshared());
header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXSUBJECTID,
		getMaxSubjectID());

populateHeaderMaxFourthSectionId(header, rootNode);

header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXOBJECTTID,
		getMaxObjectID());
header.insert(rootNode, HDTVocabulary::DICTIONARY_MAPPING, mapping);
header.insert(rootNode, HDTVocabulary::DICTIONARY_SIZE_STRINGS, size());
header.insert(rootNode, HDTVocabulary::DICTIONARY_BLOCK_SIZE, blocksize);
}

unsigned int BaseLiteralDictionary::getNsubjects()const {
return shared->getLength() + subjects->getLength();
}
unsigned int BaseLiteralDictionary::getNobjects()const {
return shared->getLength() + objectsNotLiterals->getLength() + objectsLiterals->getLength();
}
unsigned int BaseLiteralDictionary::getNshared()const {
return shared->getLength();
}

unsigned int BaseLiteralDictionary::getMaxID()const {
unsigned int s = subjects->getLength();
unsigned int o = objectsLiterals->getLength()+objectsNotLiterals->getLength();
unsigned int sh = shared->getLength();
unsigned int max = s > o ? s : o;

if (mapping == MAPPING2) {
	return sh + max;
} else {
	return sh + s + o;
}
}

unsigned int BaseLiteralDictionary::getMaxSubjectID()const {
return getNsubjects();
}



unsigned int BaseLiteralDictionary::getMaxObjectID()const {
unsigned int s = subjects->getLength();
unsigned int o = objectsLiterals->getLength()+objectsNotLiterals->getLength();
unsigned int sh = shared->getLength();

if (mapping == MAPPING2) {
	return sh + o;
} else {
	return sh + s + o;
}
}

size_t BaseLiteralDictionary::getNumberOfElements()const {
return shared->getLength() + subjects->getLength() + objectsLiterals->getLength()+objectsNotLiterals->getLength();
}

uint64_t BaseLiteralDictionary::size()const {
return shared->getSize() + subjects->getSize()
		+ objectsLiterals->getSize()+objectsNotLiterals->getSize();
}

void BaseLiteralDictionary::startProcessing(ProgressListener *listener) {
}

void BaseLiteralDictionary::stopProcessing(ProgressListener *listener) {

}

unsigned int BaseLiteralDictionary::insert(const std::string & str,
	const TripleComponentRole position) {
throw std::runtime_error("This dictionary does not support insertions.");
}

string BaseLiteralDictionary::getType()const {
return HDTVocabulary::DICTIONARY_TYPE_LITERAL;
}

unsigned int BaseLiteralDictionary::getMapping()const {
return mapping;
}

csd::CSD *BaseLiteralDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
switch (position) {
case SUBJECT:
	if (id <= shared->getLength()) {
		//cout << "Section SHARED" << endl;
		return shared;
	}else if(id <= shared->getLength() + subjects->getLength()){
		//cout << "Section SUBJECTS" << endl;
		return subjects;
	}
	break;
	

case OBJECT:

	if (id <= shared->getLength()) {
		//cout << "Section SHARED" << endl;
		return shared;
	} else {
		unsigned int localId = 0;
		if (mapping == MAPPING2) {
			if(id <= shared->getLength() + objectsNotLiterals->getLength() + objectsLiterals->getLength())
				localId = id - shared->getLength();
			else
				throw std::runtime_error("Item id too high");
		}else {
			if(id > shared->getLength()+subjects->getLength() && id <= shared->getLength() + subjects->getLength() + objectsNotLiterals->getLength() + objectsLiterals->getLength())
				localId = 2+ id - shared->getLength() - subjects->getLength();
			else
				throw std::runtime_error("Item id too high");
		}

		if (localId <= objectsLiterals->getLength()) 
			return objectsLiterals;
		else 
			return objectsNotLiterals;
	}
	break;
default:
throw std::runtime_error("Item not found");
}
return NULL;
}

unsigned int BaseLiteralDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
switch (position) {
case NOT_SHARED_SUBJECTS:
case NOT_SHARED_SUBJECTS_GRAPHS:
	return shared->getLength() + id;

case NOT_SHARED_OBJECTS:
case NOT_SHARED_OBJECTS_GRAPHS:
	if (mapping == MAPPING2) {
		return shared->getLength() + id;
	} else {
		return shared->getLength() + subjects->getLength() + id;
	}

case SHARED_SUBJECTS:
case SHARED_OBJECTS:
case SHARED_SUBJECTS_GRAPHS:
case SHARED_OBJECTS_GRAPHS:
	return id;
default:
	throw std::runtime_error("Item not found");
	return 0;
}

}

unsigned int BaseLiteralDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
switch (position) {
case SUBJECT:
	if (id <= shared->getLength()) {
		return id;
	} else {
		return id - shared->getLength();
	}
case OBJECT:
	if (id <= shared->getLength()) {
		return id;
	} else {
		unsigned int localId = 0;
		if (mapping == MAPPING2) {
			localId = id - shared->getLength();
		} else {
			localId =  2 + id - shared->getLength() - subjects->getLength();
		}

		if (localId <= objectsLiterals->getLength()) {
						return localId;
		} else {
			return localId-objectsLiterals->getLength();
		}


	}
default:
	throw std::runtime_error("Item not found");
	return 0;
}

}


hdt::IteratorUCharString* BaseLiteralDictionary::getSuggestions(const char *prefix, hdt::TripleComponentRole role)
{
throw std::logic_error("getSuggestions not implemented");
}

hdt::IteratorUInt *BaseLiteralDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role){
throw std::logic_error("getIDSuggestions not implemented");
}

void BaseLiteralDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role, std::vector<std::string> &out, int maxResults) {

vector<string> v1, v2;
shared->fillSuggestions(base, v1, maxResults);
if (role == SUBJECT) {
	subjects->fillSuggestions(base, v2, maxResults);
} else if (role == OBJECT) {
	if (base[0]=='"'){
		objectsLiterals->fillSuggestions(base, v2, maxResults);
	}
	else{
		objectsNotLiterals->fillSuggestions(base, v2, maxResults);
	}
}

// Merge results from shared and subjects/objects keeping order
merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(out));

// Remove possible extra items
if (out.size() > maxResults) {
	out.resize(maxResults);
}
}

}

#else
int LiteralDictionayDummySymbol;
#endif
