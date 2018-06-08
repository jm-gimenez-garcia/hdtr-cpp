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

#include <Iterator.hpp>
#include <HDTSpecification.hpp>
#include <Dictionary.hpp>

#include "../libdcs/CSD.h"

namespace hdt {

class BaseFourSectionDictionary : public virtual Dictionary {
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
	void loadShared(std::istream & input, IntermediateListener& listener);
	void loadSubjects(std::istream & input, IntermediateListener& listener);
	virtual void loadFourthSection(std::istream & input, IntermediateListener& listener)=0;
	void loadObjects(std::istream & input, IntermediateListener& listener);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	void loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count);
	void loadShared(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	void loadSubjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	virtual void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)=0;
	void loadObjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);

	void import(Dictionary *other, ProgressListener *listener);
	virtual void importFourthSection(Dictionary *other, IntermediateListener& iListener=NULL)=0;
	IteratorUCharString* getSubjects()const;
	IteratorUCharString* getObjects()const;
	IteratorUCharString* getShared()const;

	void save(std::ostream& output, ControlInformation& controlInformation, ProgressListener *listener);
	void saveControlInfo(std::ostream& output, ControlInformation& controlInformation);
	void saveShared(std::ostream & output, ProgressListener *listener);
	void saveSubjects(std::ostream & output, ProgressListener *listener);
	virtual void saveFourthSection(std::ostream & output, IntermediateListener& iListener)=0;
	void saveObjects(std::ostream & output, ProgressListener *listener);

	void populateHeader(Header & header, string rootNode);
	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
	unsigned int getNshared()const;
	unsigned int getMaxID()const;
	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	virtual size_t getNumberOfElements()const;
    	virtual uint64_t size()const;
	string getType()const;
	unsigned int getMapping()const;

protocted:
	virtual csd::CSD *getDictionarySection(unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const{return getGlobalId(mapping, id, position);}
	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return getLocalId(mapping,id,position);}

protected:
	virtual void clear();
	virtual void create();
	
};

}

#endif /* HDT_TS_DICTIONARY_HPP_ */
