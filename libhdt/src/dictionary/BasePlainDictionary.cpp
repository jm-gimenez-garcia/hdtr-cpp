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

#include "BasePlainDictionary.hpp"
#include "../util/Histogram.h"

#include "ControlInformation.hpp"
#include "HDTListener.hpp"
#include "Header.hpp"
#include "HDTVocabulary.hpp"
#include <algorithm>

#include "DictionaryEntry.hpp"

namespace hdt {

/* DICTIONARY ENTRY */


bool DictionaryEntry::cmpLexicographic(DictionaryEntry *c1, DictionaryEntry *c2) {
    return strcmp(c1->str,c2->str)<0;
}

bool DictionaryEntry::cmpID(DictionaryEntry *c1, DictionaryEntry *c2) {
	return c1->id>c2->id;
}


BasePlainDictionary::BasePlainDictionary() : mapping(MAPPING2), dict_sorted(false){}

BasePlainDictionary::BasePlainDictionary(HDTSpecification &specification) : spec(specification), dict_sorted(false) {
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
	unsigned int ret;

	if(key.length()==0 || key.at(0) == '?') 
		return 0;

	switch (position) {
		case SUBJECT:
			ret = idFromString(shared, key);
				return (ret==0) ? idFromString(subjects, key) : ret;
		case OBJECT:
			ret = idFromString(shared, key);
				return (ret==0) ? idFromString(objects, key) : ret;
		default:
			return 0;
	}
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
	iListener.setRange(0,100);
	lexicographicSort(&iListener);
	

	//dumpSizes(cout);
}
void BasePlainDictionary::insert(const string& str, const DictionarySection& pos){
	throw std::logic_error("This method (BasePlainDictionary::insert(...)) shouldn't be called. ModifiableReificationDictionary::insert(...) should have been called instead.");

}

unsigned int BasePlainDictionary::insert(const std::string & str, const TripleComponentRole pos){
	throw std::logic_error("This method (BasePlainDictionary::insert(...)) shouldn't be called. ModifiableReificationDictionary::insert(...) should have been called instead.");
	}
void BasePlainDictionary::saveControlInfo(std::ostream &output, ControlInformation &controlInfo)
{
	controlInfo.setFormat(HDTVocabulary::DICTIONARY_TYPE_PLAIN);
	controlInfo.setUint("mapping", mapping);
	controlInfo.setUint("sizeStrings", sizeStrings);
	controlInfo.setUint("numEntries", getNumberOfElements());
	controlInfo.save(output);
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

	//ControlInformation has already been read from input in BasicHDT::loadFromHDT
	
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
			} else if (region == 4) { //predicates or graphs, numElements)
				insertFourthRegion(iListener, line, numLine, numElements);
			}
		} else {
			region++;
		}

		numLine++;
	}

	// No stopProcessing() Needed. Dictionary already split and sorted in file.
    updateIDs();

	dict_sorted = true;
}

size_t BasePlainDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener)
{
    throw std::logic_error("Not implemented load");
}

void BasePlainDictionary::import(Dictionary *other, ProgressListener *listener) {
	throw std::logic_error("Not implemented import");
}

IteratorUCharString *BasePlainDictionary::getSubjects() {
	if (dict_sorted)
		return new DictIterator(subjects);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
IteratorUCharString *BasePlainDictionary::getSubjects()const {
	if (dict_sorted)
		return new DictIterator_const(subjects);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}

IteratorUCharString *BasePlainDictionary::getObjects(){
	if (dict_sorted)
		return new DictIterator(objects);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
IteratorUCharString *BasePlainDictionary::getObjects()const{
	if (dict_sorted)
		return new DictIterator_const(objects);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}

IteratorUCharString *BasePlainDictionary::getShared()const {
	if (dict_sorted)
		return new DictIterator_const(shared);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
IteratorUCharString *BasePlainDictionary::getShared() {
	if (dict_sorted)
		return new DictIterator(shared);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
size_t BasePlainDictionary::getNumberOfElements()const{
	return shared.size() + subjects.size() + objects.size();
}

uint64_t BasePlainDictionary::size()const{
    return sizeStrings;
}




string intToStr(int val) {
	std::stringstream out;
	out << val;
	return out.str();
}


// PRIVATE


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

	dict_sorted = true;
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
	//dict_sorted = true;
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
			if(id >= getNsubjects() - getNshared())
				throw std::logic_error("NOT_SHARED_SUBJECT idx exceeds dimension");
			return shared.size()+id+1;
		case NOT_SHARED_OBJECT:
			if(id >= getNobjects() - getNshared())
				throw std::logic_error("NOT_SHARED_OBJECT idx exceeds dimension");
			return (mapping==MAPPING2) ? shared.size()+id+1 : shared.size()+subjects.size()+id+1;
		case SHARED_SUBJECT:
		case SHARED_OBJECT:
			if(id >= getNshared())
				throw std::logic_error("SHARED_OBJECT idx exceeds dimension");
			return id+1;
		default:
			throw std::runtime_error("Item not found");
			return 0;
	}

}

unsigned int BasePlainDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	const unsigned int sh_length = shared.size();
	const unsigned int sub_length = subjects.size();
	const unsigned int obj_length = objects.size();

	switch (position) {
		case SUBJECT:
			if(id<=sh_length)
				return id-1;
			else if(id <= sh_length+sub_length)
				return id-sh_length-1;
			else
			{
				cerr << "globid=" << id<<endl;
				cerr << "sh_length=" << sh_length<<endl;
				throw std::runtime_error("This globalID does not correspond to a SUBJECT");
			}
			break;
		case OBJECT:
			if(id <= sh_length) 
				return id-1;
			else if (mapping==MAPPING1) 
			{	
				if ( (id <= sh_length + sub_length + obj_length) && (id > sh_length + sub_length) )
					return id - sh_length - sub_length-1;
				else
					throw std::runtime_error("This globalID does not correspond to an OBJECT with MAPPING1");

			}
			else if (mapping==MAPPING2)
			{
				if (id <= sh_length + obj_length)
					return id - sh_length -1 ;
				else
					throw std::runtime_error("This globalID does not correspond to an OBJECT with MAPPING2");
			}
			else
				throw std::runtime_error("Uknown mapping");
			break;
		default:
			throw std::runtime_error("Item not found");
			return 0;
	}



}

/** Get Max Id
 * @return The expected result
 */
unsigned int BasePlainDictionary::getMaxID()const {
	unsigned int s = subjects.size();
	unsigned int o = objects.size();
	unsigned int nshared = shared.size();
	unsigned int max = s>o ? s : o;

	return (mapping ==MAPPING2) ? nshared+max : nshared+s+o;
}

unsigned int BasePlainDictionary::getMaxSubjectID()const {
	unsigned int nshared = shared.size();
	unsigned int s = subjects.size();

	return nshared+s;
}


unsigned int BasePlainDictionary::getMaxObjectID()const {
	unsigned int nshared = shared.size();
	unsigned int s = subjects.size();
	unsigned int o = objects.size();

	if(mapping ==MAPPING2) {
		return nshared+o;
	} else {
		return nshared+s+o;
	}
}

unsigned int BasePlainDictionary::getNsubjects()const 
{return shared.size()+subjects.size();}

unsigned int BasePlainDictionary::getNobjects()const 
{return shared.size()+objects.size();}

unsigned int BasePlainDictionary::getNshared()const 
{return shared.size();}


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
		default:
			break;
	}
}

void BasePlainDictionary::push_back(DictionaryEntry* entry, DictionarySection pos){

	switch(pos){
		case SHARED_SUBJECT:
		case SHARED_OBJECT:
			shared.push_back(entry);
			sizeStrings+=strlen(entry->str);
			break;
		case NOT_SHARED_SUBJECT:
			subjects.push_back(entry);
			sizeStrings+=strlen(entry->str);
			break;
		case NOT_SHARED_OBJECT:
			objects.push_back(entry);
			sizeStrings+=strlen(entry->str);
			break;
		default:
			break;
	}
}

void BasePlainDictionary::populateHeader(Header &header, string rootNode)
{
	header.insert(rootNode, HDTVocabulary::DICTIONARY_TYPE, HDTVocabulary::DICTIONARY_TYPE_PLAIN);
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSUBJECTS, getNsubjects());
	populateHeaderFourthElementNum(header, rootNode);
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMOBJECTS, getNobjects());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMSHARED, getNshared());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXSUBJECTID, getMaxSubjectID());
	populateHeaderFourthElementMaxId(header, rootNode);
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXOBJECTTID, getMaxObjectID());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAPPING, getMapping());
	header.insert(rootNode, HDTVocabulary::DICTIONARY_SIZE_STRINGS, size());
}


unsigned int BasePlainDictionary::getMapping()const {
	return mapping;
}

void BasePlainDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{ throw std::logic_error("getSuggestions not implemented");}

IteratorUCharString* BasePlainDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role)
{throw std::logic_error("getSuggestions not implemented");}

IteratorUInt *BasePlainDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role)
{ throw std::logic_error("getIDSuggestions not implemented");}



}
