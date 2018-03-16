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

#ifndef HDT_TS_DICTIONARY_HPP_
#define HDT_TS_DICTIONARY_HPP_

#include <Iterator.hpp>
#include <HDTSpecification.hpp>
#include <Dictionary.hpp>

#include "../libdcs/CSD.h"

namespace hdt {

class ThreeSectionDictionary : public virtual Dictionary {
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
	ThreeSectionDictionary();
	ThreeSectionDictionary(HDTSpecification &spec);
	~ThreeSectionDictionary();

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;
	void load(std::istream & input, ControlInformation & ci, ProgressListener *listener=NULL);
	virtual void loadFourthSection((std::istream & input, ControlInformation & ci, ProgressListener *listener)=0;
	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	virtual void loadFourthSection(unsigned char *ptr, int& count, const IntermediateListener& iListener)=0;
	void import(Dictionary *other, ProgressListener *listener);
	virtual void importFourthSection(Dictionary *other, IntermediateListener& iListener=NULL)=0;
	IteratorUCharString* getSubjects()const;
	IteratorUCharString* getObjects()const;
	IteratorUCharString* getShared()const;
	void save(std::ostream & output, ControlInformation & controlInformation, ProgressListener *listener=NULL);
	virtual void saveFourthSection(std::ostream & output, IntermediateListener& listener)=0;
	void populateHeader(Header & header, string rootNode);
	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
	unsigned int getNshared()const;
	unsigned int getMaxID()const;
	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	size_t getNumberOfElements()const;
	virtual unsigned int getFourthSectionLength()const=0;
    	uint64_t size()const;
    	virtual uint64_t getFourthSectionSize()const=0;
	string getType()const;
	unsigned int getMapping()const;

protocted:
	virtual csd::CSD *getDictionarySection(unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	virtual unsigned int getGlobalId(unsigned int id, DictionarySection position)const=0;
	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getLocalId(unsigned int id, TripleComponentRole position)const=0;
	
};

}

#endif /* HDT_TS_DICTIONARY_HPP_ */
