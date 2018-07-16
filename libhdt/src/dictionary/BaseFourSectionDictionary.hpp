/*
 * File: PFCDictionary.hpp
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

#ifndef HDT_BFS_DICTIONARY_HPP_
#define HDT_BFS_DICTIONARY_HPP_

//#include <Iterator.hpp>
#include "HDTSpecification.hpp"
#include "Dictionary.hpp"
//#include "../libdcs/CSD.h"
#include "HDTEnums.hpp"

namespace csd{
	class CSD;
};

namespace hdt {

class ControlInformation;
class ProgressListener;
class IntermediateListener;
class IteratorUCharString;
class Header;

class BaseFourSectionDictionary : virtual public Dictionary {
protected:

	unsigned int mapping;
	uint64_t sizeStrings;
	uint32_t blocksize;

	csd::CSD *subjects;
	csd::CSD *objects;
	csd::CSD *shared;


	//ControlInformation controlInformation;
	HDTSpecification spec;

public:
	BaseFourSectionDictionary();
	BaseFourSectionDictionary(HDTSpecification &spec);
	virtual ~BaseFourSectionDictionary();

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;

	void load(std::istream & input, ControlInformation & ci, ProgressListener *listener=NULL);
	void loadControlInfo(std::istream & input, ControlInformation & ci);
	void loadShared(std::istream & input, IntermediateListener& iListener);
	void loadSubjects(std::istream & input, IntermediateListener& iListener);
	virtual void loadFourthSection(std::istream & input, IntermediateListener& iListener)=0;
	void loadObjects(std::istream & input, IntermediateListener& listener);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	void loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count);
	void loadShared(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	void loadSubjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	virtual void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)=0;
	void loadObjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);

	void import(Dictionary *other, ProgressListener *listener);
	void importSubjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);
	virtual void importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)=0;
	void importObjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);
	void importShared(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener); 

	IteratorUCharString* getSubjects();
	IteratorUCharString* getSubjects()const;
	IteratorUCharString* getObjects();
	IteratorUCharString* getObjects()const;
	IteratorUCharString* getShared();
	IteratorUCharString* getShared()const;
	//virtual IteratorUCharString* getPredicates()=0;
	//virtual IteratorUCharString* getGraphs()=0;

	void save(std::ostream& output, ControlInformation& controlInformation, ProgressListener *listener);
	void saveControlInfo(std::ostream& output, ControlInformation& controlInformation);
	void saveShared(std::ostream & output, IntermediateListener& iListener);
	void saveSubjects(std::ostream & output, IntermediateListener& iListener);
	virtual void saveFourthSection(std::ostream & output, IntermediateListener& iListener)=0;
	void saveObjects(std::ostream & output, IntermediateListener& iListener);

	void populateHeader(Header & header, std::string rootNode);
	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
	unsigned int getNshared()const;
	//virtual unsigned int getNpredicates()const=0;
	//virtual unsigned int getNgraphs()const=0;

	unsigned int getMaxID()const;
	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	//virtual unsigned int getMaxPredicateID()const=0;
	//virtual unsigned int getMaxGraphID()const=0;
	virtual size_t getNumberOfElements()const;
    virtual uint64_t size()const;
	std::string getType()const;
	unsigned int getMapping()const;
    virtual void getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults);
    virtual IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);
    virtual IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);

protected:
	virtual csd::CSD *getDictionarySection(unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	virtual unsigned int getGlobalId(unsigned int id, DictionarySection position)const{
		return BaseFourSectionDictionary::getGlobalId(mapping, id, position);}
	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return BaseFourSectionDictionary::getLocalId(mapping,id,position);}

protected:
	virtual void create();
	virtual void clear_loc();
	void clear();
	
};

csd::CSD *loadSection(IteratorUCharString *iterator, uint32_t blocksize, ProgressListener *listener);

}

#endif /* HDT_TS_DICTIONARY_HPP_ */
