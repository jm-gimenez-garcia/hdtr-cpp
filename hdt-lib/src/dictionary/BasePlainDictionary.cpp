/*
 * File: BasePlainDictionary.cpp
 * Last modified: $Date$
 * Revision: $Revision$
 * Last modified by: $Author$
 *
 * Copyright (C) 2012, Mario Arias, Javier D. Fernandez, Miguel A. Martinez-Prieto
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */

#include <sstream>
#include <stdexcept>
#include "BasePlainDictionary.hpp"
#include "../util/Histogram.h"

#include <HDTVocabulary.hpp>

namespace hdt {

/* DICTIONARY ENTRY */


bool DictionaryEntry::cmpLexicographic(DictionaryEntry *c1, DictionaryEntry *c2) {
    return strcmp(c1->str,c2->str)<0;
}

bool DictionaryEntry::cmpID(DictionaryEntry *c1, DictionaryEntry *c2) {
	return c1->id>c2->id;
}


BasePlainDictionary::BasePlainDictionary() {
	mapping = MAPPING2;
}

BasePlainDictionary::BasePlainDictionary(HDTSpecification &specification) : spec(specification) {
	string map ="";
	try{
		map = spec.get("dictionary.mapping");
	}catch(exception& e){}
	if(map=="mapping1") {
		this->mapping = MAPPING1;
	} else {
		this->mapping = MAPPING2;
	}
}

BasePlainDictionary::~BasePlainDictionary() {
	unsigned int i;

	for(i=0;i<shared.size();i++) {
        	delete [] shared[i]->str;
		delete shared[i];
	}

	for(i=0;i<subjects.size();i++) {
		delete [] subjects[i]->str;
		delete subjects[i];
	}

	for(i=0;i<objects.size();i++) {
		delete [] objects[i]->str;
		delete objects[i];
	}
}

std::string BasePlainDictionary::idToString(const unsigned int id, const TripleComponentRole position)const
{
	const vector<DictionaryEntry*> &vector = getDictionaryEntryVector(id, position);

	unsigned int localid = getLocalId(id, position);

	if(localid<vector.size()) {
		DictionaryEntry *entry = vector[localid];
		string result(entry->str);
		return result;
	}

	return string();
}

unsigned int BasePlainDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{
	DictEntryIt ret;

	if(key.length()==0)
		return 0;

	if(position==SUBJECT) {
		ret = hashSubject.find(key.c_str());
    		return ret==hashSubject.end()   ? 0 : ret->second->id;
	}
	else if(position==OBJECT){
		ret = hashObject.find(key.c_str());
    		return ret==hashObject.end()    ? 0 : ret->second->id;
	}
	else
		throw std::runtime_error("BasePlainDictionary::stringToId : Uknown TripleComponentRole");
	return 0;
}

void BasePlainDictionary::startProcessing(ProgressListener *listener)
{
	subjects.clear();
	shared.clear();
	objects.clear();
}

void BasePlainDictionary::stopProcessing(ProgressListener *listener)
{
	IntermediateListener iListener(listener);
	iListener.setRange(0,50);
	split(listener);

	iListener.setRange(0,100);
	lexicographicSort(&iListener);

	//dumpSizes(cout);
}

void BasePlainDictionary::saveControlInfo(std::ostream &output, ControlInformation &controlInformation)
{
	controlInformation.setFormat(HDTVocabulary::DICTIONARY_TYPE_PLAIN);
	controlInformation.setUint("mapping", this->mapping);
	controlInformation.setUint("sizeStrings", this->sizeStrings);
	controlInformation.setUint("numEntries", this->getNumberOfElements());
	controlInformation.save(output);
}

void BasePlainDictionary::saveShared(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//shared subjects-objects from subjects
	for (unsigned int i = 0; i < shared.size(); i++) {
		output << shared[i]->str;
		output.put(marker); //character to split file
		counter++;
		NOTIFYCOND(listener, "BasePlainDictionary saving shared", counter, getNumberOfElements());
	}
	output.put(marker); //extra line to set the begining of next part of dictionary
}

void BasePlainDictionary::saveSubjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//not shared subjects
	for (unsigned int i = 0; i < subjects.size(); i++) {
		output << subjects[i]->str;
		output.put(marker); //character to split file
		counter++;
		NOTIFYCOND(listener, "BasePlainDictionary saving subjects", counter, getNumberOfElements());
	}
	output.put(marker); //extra line to set the begining of next part of dictionary
}

void BasePlainDictionary::saveObjects(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//not shared objects
	for (unsigned int i = 0; i < objects.size(); i++) {
		output << objects[i]->str;
		output.put(marker); //character to split file
		counter++;
		NOTIFYCOND(listener, "BasePlainDictionary saving objects", counter, getNumberOfElements());
	}
	output.put(marker); //extra line to set the begining of next part of dictionary
}



void BasePlainDictionary::save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener)
{
	unsigned int counter=0;
	const char marker = '\1';

	saveControlInfo(output, controlInformation);
	saveShared(output, listener, counter, marker);
	saveSubjects(output, listener, counter, marker);
	saveObjects(output, listener, counter, marker);
	saveFourthSection(output, listener, counter, marker);

	output.put(marker);
}

void BasePlainDictionary::load(std::istream & input, ControlInformation &ci, ProgressListener *listener)
{
	std::string line;
	unsigned char region = 1;

	startProcessing();

	std::string format = ci.getFormat();
	if(format!=getType()) {
		throw std::runtime_error("Trying to read a BasePlainDictionary but the data is not BasePlainDictionary");
	}

	this->mapping = ci.getUint("mapping");
	this->sizeStrings = ci.getUint("sizeStrings");
	unsigned int numElements = ci.getUint("numEntries");
	unsigned int numLine = 0;

	IntermediateListener iListener(listener);
	iListener.setRange(0,25);
	while(region<5 && getline(input, line,'\1')) {
		//std::cout << line << std::endl;
		if(line!="") {
			if (region == 1) { //shared SO
				NOTIFYCOND(&iListener, "Dictionary loading shared area.", numLine, numElements);
				insert(line, SHARED_SUBJECT);
			} else if (region == 2) { //not shared Subjects
				NOTIFYCOND(&iListener, "Dictionary loading subjects.", numLine, numElements);
				insert(line, NOT_SHARED_SUBJECT);
			} else if (region == 3) { //not shared Objects
				NOTIFYCOND(&iListener, "Dictionary loading objects.", numLine, numElements);
				insert(line, NOT_SHARED_OBJECT);
			} else if (region == 4) { //predicates or graphs
				insertFourthRegion(iListener, line, region);
			}
		} else {
			region++;
		}

		numLine++;
	}

	// No stopProcessing() Needed. Dictionary already split and sorted in file.
    updateIDs();
}

size_t BasePlainDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener)
{
    throw std::logic_error("Not implemented load");
}

void BasePlainDictionary::import(Dictionary *other, ProgressListener *listener) {
	throw std::logic_error("Not implemented import");
}

IteratorUCharString *BasePlainDictionary::getSubjects() {
	return new DictIterator(this->subjects);
}
IteratorUCharString *BasePlainDictionary::getPredicates() {
	return new DictIterator(this->predicates);
}
IteratorUCharString *BasePlainDictionary::getObjects(){
	return new DictIterator(this->objects);
}
IteratorUCharString *BasePlainDictionary::getShared() {
	return new DictIterator(this->shared);
}

size_t BasePlainDictionary::getNumberOfElements()const{
	return shared.size() + subjects.size() + objects.size();
}

uint64_t BasePlainDictionary::size()const{
    return sizeStrings;
}


unsigned int BasePlainDictionary::insert(const std::string & str, const TripleComponentRole pos)
{
	if(str=="") return 0;

	insertFourthElement(str, pos);

	DictEntryIt subjectIt = hashSubject.find(str.c_str());
	DictEntryIt objectIt = hashObject.find(str.c_str());

	bool foundSubject = subjectIt!=hashSubject.end();
	bool foundObject = objectIt!=hashObject.end();
	//cout << "A: " << foundSubject << " B: " << foundSubject << endl;

	if(pos==SUBJECT) {
		if( !foundSubject && !foundObject) {
			// Did not exist, create new.
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();

			//cout << " Add new subject: " << str << endl;
			hashSubject[entry->str] = entry;
		} else if(foundSubject) {
			// Already exists in subjects.
			//cout << "   existing subject: " << str << endl;
		} else if(foundObject) {
			// Already exists in objects.
			//cout << "   existing subject as object: " << str << endl;
			hashSubject[objectIt->second->str] = objectIt->second;
		}
	} else if(pos==OBJECT) {
		if(!foundSubject && !foundObject) {
			// Did not exist, create new.
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();

			//cout << " Add new object: " << str << endl;
			hashObject[entry->str] = entry;
		} else if(foundObject) {
			// Already exists in objects.
			//cout << "     existing object: " << str << endl;
		} else if(foundSubject) {
			// Already exists in subjects.
			//cout << "     existing object as subject: " << str << endl;
			hashObject[subjectIt->second->str] = subjectIt->second;
		}
	}

	// FIXME: Return inserted index?
	return 0;
}

string intToStr(int val) {
	std::stringstream out;
	out << val;
	return out.str();
}


// PRIVATE

void BasePlainDictionary::insert(const string& str, const DictionarySection& pos) {

	if(str=="") return;

	DictionaryEntry *entry = new DictionaryEntry;
	entry->str = new char [str.length()+1];
	strcpy(entry->str, str.c_str());

	switch(pos) {
	case SHARED_SUBJECT:
	case SHARED_OBJECT:
		shared.push_back(entry);
		//entry->id = subjects_shared.size();
		hashSubject[entry->str] = entry;
		hashObject[entry->str] = entry;
		break;
	case NOT_SHARED_SUBJECT:
		subjects.push_back(entry);
		//entry->id = subjects_shared.size()+subjects_not_shared.size();
		hashSubject[entry->str] = entry;
		break;
	case NOT_SHARED_OBJECT:
		objects.push_back(entry);
		//entry->id = subjects_shared.size()+objects_not_shared.size();
		hashObject[entry->str] = entry;
		break;
	default:
		throw std::runtime_error("BasePlainDictionary::insert : uknown DictionarySection");
		break;
	}
}

/** Split
 * @return void
 */
void BasePlainDictionary::split(ProgressListener *listener) {
	subjects.clear();
	shared.clear();
	objects.clear();

	unsigned int total = hashSubject.size()+hashObject.size();
	unsigned int count = 0;

	for(DictEntryIt subj_it = hashSubject.begin(); subj_it!=hashSubject.end() && subj_it->first; subj_it++) {
		//cout << "Check Subj: " << subj_it->first << endl;
		DictEntryIt other = hashObject.find(subj_it->first);

		if(other==hashObject.end()) {
			// Only subject
			subjects.push_back(subj_it->second);
		} else {
			// Exist in both
			shared.push_back(subj_it->second);
		}
		count++;
		NOTIFYCOND(listener, "Extracting shared subjects", count, total);
	}

	for(DictEntryIt obj_it = hashObject.begin(); obj_it!=hashObject.end(); ++obj_it) {
		//cout << "Check Obj: " << obj_it->first << endl;
		DictEntryIt other = hashSubject.find(obj_it->first);

		if(other==hashSubject.end()) {
			// Only object
			objects.push_back(obj_it->second);
		}
		count++;
		NOTIFYCOND(listener, "Extracting shared objects", count, total);
	}
}

/** Lexicographic Sort
 * @param mapping Description of the param.
 * @return void
 */
void BasePlainDictionary::lexicographicSort(ProgressListener *listener) {

#ifdef _OPENMP
    NOTIFY(listener, "Sorting dictionary", 0, 100);
    #pragma omp parallel sections if(objects.size()>100000)
    {
        { lexicographicSortFourthElement(); }
        { sort(shared.begin(), shared.end(), DictionaryEntry::cmpLexicographic); }
        { sort(subjects.begin(), subjects.end(), DictionaryEntry::cmpLexicographic); }
        { sort(objects.begin(), objects.end(), DictionaryEntry::cmpLexicographic); }
    }
#else
    NOTIFY(listener, "Sorting shared", 0, 100);
    sort(shared.begin(), shared.end(), DictionaryEntry::cmpLexicographic);

    NOTIFY(listener, "Sorting subjects", 20, 100);
    sort(subjects.begin(), subjects.end(), DictionaryEntry::cmpLexicographic);

    NOTIFY(listener, "Sorting objects", 50, 100);
    sort(objects.begin(), objects.end(), DictionaryEntry::cmpLexicographic);

    NOTIFY(listener, "Sorting fourth elements (predicates or graphs)", 90, 100);
    lexicographicSortFourthElement();

#endif
    NOTIFY(listener, "Update Dictionary IDs", 99, 100);
    updateIDs();
}

void BasePlainDictionary::idSort() {

#ifdef _OPENMP
    #pragma omp parallel sections
    {
        { sort(shared.begin(), shared.end(), DictionaryEntry::cmpID); }
        { sort(subjects.begin(), subjects.end(), DictionaryEntry::cmpID); }
        { sort(objects.begin(), objects.end(), DictionaryEntry::cmpID); }
        { idSortFourthElement(); }
    }
#else
	sort(shared.begin(), shared.end(), DictionaryEntry::cmpID);
	sort(subjects.begin(), subjects.end(), DictionaryEntry::cmpID);
	sort(objects.begin(), objects.end(), DictionaryEntry::cmpID);
	idSortFourthElement();
#endif

	updateIDs();
}


/** Map
 * @param mapping Description of the param.
 * @return void
 */
void BasePlainDictionary::updateIDs() {
	unsigned int i;
	for (i = 0; i < shared.size(); i++) 
		shared[i]->id = getGlobalId(i, SHARED_SUBJECT);
	for (i = 0; i < subjects.size(); i++) 
		subjects[i]->id = getGlobalId(i, NOT_SHARED_SUBJECT);
	for (i = 0; i < objects.size(); i++) 
		objects[i]->id = getGlobalId(i, NOT_SHARED_OBJECT);
}


void BasePlainDictionary::convertMapping(unsigned int mapping) {
	switch(mapping) {
	case MAPPING1:
		this->mapping = mapping;
		updateIDs();
		break;
	case MAPPING2:
		this->mapping = mapping;
		updateIDs();
		break;
	default:
		break;
	}
}

const vector<DictionaryEntry*> &BasePlainDictionary::getDictionaryEntryVector(unsigned int id, TripleComponentRole position) const{
	if(position==SUBJECT)
		return (id<=shared.size()) ? shared : subjects;
	else if (position==OBJECT)
		return (id<=shared.size()) ? shared : objects;
	throw std::runtime_error("Item not found");
}

unsigned int BasePlainDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch (position) {
		case NOT_SHARED_SUBJECT:
			return shared.size()+id+1;
		case NOT_SHARED_OBJECT:
			if(mapping==MAPPING2) ? shared.size()+id+1 : shared.size()+subjects.size()+id+1;
		case SHARED_SUBJECT:
		case SHARED_OBJECT:
			return id+1;
	}
	throw std::runtime_error("Item not found");
}


unsigned int BasePlainDictionary::getGlobalId(unsigned int id, DictionarySection position) const{
	return getGlobalId(mapping, id, position);
}

unsigned int BasePlainDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	switch (position) {
		case SUBJECT:
			return (id<=shared.size()) ? id-1 : id-shared.size()-1;
		case OBJECT:
			if(id<=shared.size())
				return id-1;
			else
				return (mapping==MAPPING2) ? id-shared.size()-1 : id-shared.size()-subjects.size()-1;
	}
	throw std::runtime_error("Item not found");
}

unsigned int BasePlainDictionary::getLocalId(unsigned int id, TripleComponentRole position) const{
	return getLocalId(mapping,id,position);
}



/** Get Max Id
 * @return The expected result
 */
unsigned int BasePlainDictionary::getMaxID() {
	unsigned int s = subjects.size();
	unsigned int o = objects.size();
	unsigned int nshared = shared.size();
	unsigned int max = s>o ? s : o;

	return (mapping ==MAPPING2) ? nshared+max : nshared+s+o;
}

unsigned int BasePlainDictionary::getMaxSubjectID() {
	unsigned int nshared = shared.size();
	unsigned int s = subjects.size();

	return nshared+s;
}

unsigned int BasePlainDictionary::getMaxPredicateID() {
	return predicates.size();
}

unsigned int BasePlainDictionary::getMaxObjectID() {
	unsigned int nshared = shared.size();
	unsigned int s = subjects.size();
	unsigned int o = objects.size();

	if(mapping ==MAPPING2) {
		return nshared+o;
	} else {
		return nshared+s+o;
	}
}

unsigned int BasePlainDictionary::getNsubjects() {
	return shared.size()+subjects.size();
}

unsigned int BasePlainDictionary::getNpredicates() {
	return predicates.size();
}

unsigned int BasePlainDictionary::getNobjects() {
	return shared.size()+objects.size();
}

unsigned int BasePlainDictionary::getNshared() {
	return shared.size();
}



void BasePlainDictionary::updateID(unsigned int oldid, unsigned int newid, DictionarySection position) {
	switch (position) {
		case SHARED_SUBJECT:
		case SHARED_OBJECT:
			shared[oldid]->id = newid;
			break;
		case NOT_SHARED_SUBJECT:
			subjects[oldid]->id = newid;
			break;
		case NOT_SHARED_OBJECT:
			objects[oldid]->id = newid;
			break;
	}
}

void BasePlainDictionary::populateHeader(Header &header, string rootNode)
{
	header.insert(rootNode, HDTVocabulary::DICTIONARY_TYPE, HDTVocabulary::DICTIONARY_TYPE_PLAIN);
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSUBJECTS, getNsubjects());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNpredicates());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMOBJECTS, getNobjects());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSHARED, getNshared());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXSUBJECTID, getMaxSubjectID());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXPREDICATEID, getMaxPredicateID());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXOBJECTTID, getMaxObjectID());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAPPING, getMapping());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_SIZE_STRINGS, size());
}

string BasePlainDictionary::getType() {
	return HDTVocabulary::DICTIONARY_TYPE_PLAIN;
}

unsigned int BasePlainDictionary::getMapping() {
	return mapping;
}

void BasePlainDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{
    throw std::logic_error("getSuggestions not implemented");
}

}
