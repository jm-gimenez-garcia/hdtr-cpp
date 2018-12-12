/*
 * File: KyotoBaseDictionary.hpp
 * Last modified: $Date: 2011-08-21 05:35:30 +0100 (dom, 21 ago 2011) $
 * Revision: $Revision: 180 $
 * Last modified by: $Author: mario.arias $
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

#ifndef _BASE_KYOTO_DICTIONARY_H_
#define _BASE_KYOTO_DICTIONARY_H_

#include <HDTSpecification.hpp>
#include <Dictionary.hpp> //for ModifiableDictionary
#include <Iterator.hpp>
#include "HDTEnums.hpp"

//#include <string.h>
//#include <string>
//#include <algorithm>
//#include <fstream>
//#include <iostream>

#ifdef HAVE_KYOTO

#include <kcpolydb.h>

//using namespace kyotocabinet;

namespace hdt {
class ControlInformation;
class ProgressListener;
class Header;

class BaseKyotoDictionary : virtual public ModifiableDictionary {
// Private attributes
public:

	unsigned int mapping;
	uint64_t sizeStrings;

	//ControlInformation controlInformation;
	HDTSpecification spec;

	//very unsafe due to non-const kyotocabinet::TreeDB::count() and kyotocabinet::TreeDB::get() methods
	mutable kyotocabinet::TreeDB subjects;
	mutable kyotocabinet::TreeDB objects;
	mutable kyotocabinet::TreeDB shared;


// Public Interface
public:
	BaseKyotoDictionary();
	BaseKyotoDictionary(HDTSpecification &spec);
	virtual ~BaseKyotoDictionary();

    IteratorUCharString *getSubjects();
    IteratorUCharString *getSubjects()const;
    IteratorUCharString *getObjects();
    IteratorUCharString *getObjects()const;
    IteratorUCharString *getShared();
    IteratorUCharString *getShared()const;
    //virtual IteratorUCharString *getPredicates()=0;
    //virtual IteratorUCharString *getGraphs()=0;
	virtual size_t getNumberOfElements()const;

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;

	virtual void startProcessing(ProgressListener *listener = NULL);
	void stopProcessing(ProgressListener *listener = NULL);
	virtual unsigned int insert(const std::string &str, const TripleComponentRole position);


	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
    void import(Dictionary *other, ProgressListener *listener=NULL);

	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const ;
	virtual unsigned int getGlobalId(unsigned int id, DictionarySection position)const {return BaseKyotoDictionary::getGlobalId(mapping, id, position);}

	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const ;
	virtual unsigned int getLocalId(unsigned int id, TripleComponentRole position)const {return BaseKyotoDictionary::getLocalId(mapping,id,position);}

	unsigned int getMaxID()const;
	void push_back(DictionaryEntry* entry, DictionarySection pos);

    uint64_t size()const;

	unsigned int getNsubjects()const;
	unsigned int getNobjects()const;
	unsigned int getNshared()const;
	//virtual unsigned int getNpredicates()const=0;
	//virtual unsigned int getNgraphs()const=0;

	unsigned int getMaxSubjectID()const;
	unsigned int getMaxObjectID()const;
	//virtual unsigned int getMaxPredicateID()const=0;
	//virtual unsigned int getMaxGraphID()const=0;

	void populateHeader(Header &header, string rootNode);

	string getType()const;
	unsigned int getMapping()const;

    void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

    hdt::IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);

    hdt::IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);

// Private methods
private:
    void updateIDs(kyotocabinet::DB *db);

public:

	virtual void dumpSizes(std::ostream &out);

	friend class PFCDictionary;
};

class KyotoDictIterator : public IteratorUCharString {
private:
	kyotocabinet::DB *db;
	kyotocabinet::DB::Cursor *cur;
	string key;
	size_t count;
public:
	KyotoDictIterator(kyotocabinet::DB *db) : db(db), cur(db->cursor()), count(0) {
		cur->jump();
	}

	~KyotoDictIterator() {
		delete cur;
	}

	bool hasNext()const {
		return count<db->count();
	}

	unsigned char *next() {
		cur->get_key(&key, true);
		count++;
		return (unsigned char*)(key.c_str());
	}

	size_t getNumberOfElements()const {
		return db->count();
	}
};

}

#endif  /* HAVE_KYOTO */

#endif /* KYOTODICTIONARY_H_ */
