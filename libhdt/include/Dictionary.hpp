/*
 * File: Dictionary.hpp
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

#ifndef HDT_DICTIONARY_HPP_
#define HDT_DICTIONARY_HPP_

#include <cstdint>
#include <vector>
#include "HDTEnums.hpp"
#include "SingleTriple.hpp"
#include "SingleQuad.hpp"


namespace std{
	typedef basic_istream<char> istream;
	typedef basic_ostream<char> ostream;
	typedef basic_string<char> string;
};


namespace hdt {

class TripleString;
class TripleID;
class Header;
class ControlInformation;
class ProgressListener;
class IteratorUCharString;
class IteratorUInt;
class DictionaryEntry;

/**
 * Interface representing the Dictionary part of the HDT representation.
 * Provides a means to convert Strings to ID and the other way around.
 */
class Dictionary
{
public:
    virtual ~Dictionary(){ }

    /**
    * Fetch the string associated to the specified ID as the triple role.
    * If the ID does not exist, it throws an exception.
    * @param id ID to be fetched
    * @param role Triple Role (Subject, Predicate, Object) to be fetched.
    * @return The string associated to that ID.
    */
    virtual std::string idToString(const unsigned int id, const TripleComponentRole role)const=0;

    /**
    * Fetch the ID assigned to the supplied string as the triple role.
    * If the ID does not exist, it throws an exception.
    * @param str String to be converted.
    * @param role Triple Role (Subject, Predicate, Object) to be fetched.
    * @return ID of the specified String
    */
    virtual unsigned int stringToId(const std::string &str, const TripleComponentRole role)const=0;


    /**
    * Convert a TripleString object to a TripleID, using the dictionary to perform the conversion.
    * If any of the components do not exist in the dictionary, it throws an exception.
    * @param tripleString TripleString to be converted.
    * @return resulting TripleID
    */
    void tripleStringtoTripleID(const TripleString &tripleString, TripleID &tid) const{
    	tid.setSubject(stringToId(tripleString.getSubject(), SUBJECT));
    	tid.setPredicate(stringToId(tripleString.getPredicate(), PREDICATE));
    	tid.setObject(stringToId(tripleString.getObject(), OBJECT));
    }

    /**
    * Convert a TripleID object to a TripleString, using the dictionary to perform the conversion.
    * If any of the components do not exist in the dictionary, it throws an exception.
    * @param tripleID TripleID to be converted.
    * @return resultant TripleSTring
    */
    void tripleIDtoTripleString(TripleID &tripleID, TripleString &ts) {
	    ts.setSubject(idToString(tripleID.getSubject(), SUBJECT));
    	ts.setPredicate(idToString(tripleID.getPredicate(), PREDICATE));
    	ts.setObject(idToString(tripleID.getObject(), OBJECT));
    }

    /**
    * Convert a QuadString object to a QuadID, using the dictionary to perform the conversion.
    * If any of the components do not exist in the dictionary, it throws an exception.
    * @param quadString QuadString to be converted.
    * @return resulting QuadID
    */
    void quadStringtoQuadID(const QuadString &quadString, QuadID &qid) const{
    	qid.setSubject(stringToId(quadString.getSubject(), SUBJECT));
    	qid.setPredicate(stringToId(quadString.getPredicate(), PREDICATE));
    	qid.setObject(stringToId(quadString.getObject(), OBJECT));

    	qid.setGraph(stringToId(quadString.getGraph(), GRAPH));
    }

    /**
    * Convert a TripleID object to a TripleString, using the dictionary to perform the conversion.
    * If any of the components do not exist in the dictionary, it throws an exception.
    * @param tripleID TripleID to be converted.
    * @return resultant TripleSTring
    */
    void quadIDtoQuadString(QuadID& qid, QuadString& qs) {
	    qs.setSubject(idToString(qid.getSubject(), SUBJECT));
    	qs.setPredicate(idToString(qid.getPredicate(), PREDICATE));
    	qs.setObject(idToString(qid.getObject(), OBJECT));
    	qs.setGraph(idToString(qid.getGraph(), GRAPH));
    }

    /** Number of total elements of the dictionary
    *
    * @return
    */
    virtual size_t getNumberOfElements() const=0;

    /**
     * Returns size in bytes of the overall structure.
     */
    virtual uint64_t size()const=0;

    /* Return the maximum id assigned to the overall dictionary. */
    virtual unsigned int getMaxID()const=0;

    virtual void import(Dictionary *other, ProgressListener *listener=NULL)=0;

    /**
    * Add to the supplied header all relevant information about the current Dictionary
    * @param header
    */
    virtual void populateHeader(Header &header, std::string rootNode)=0;

    /**
    * Save the current dictionary to a stream, using the format from the standard.
    * @param output
    * @return
    */
    virtual void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL)=0;

    /**
    * Load dictionary information from a stream.
    * The dictionary may fetch properties from the supplied ControlInformation.
    * @param input Input stream to read the data.
    * @param ci ControlInformation with the specified properties.
    */
    virtual void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL)=0;

    virtual size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL)=0;


    virtual unsigned int getNsubjects()const=0;
    virtual unsigned int getNobjects()const=0;
    virtual unsigned int getNshared()const=0;
    //virtual unsigned int getNpredicates()const=0;
    //virtual unsigned int getNgraphs()const=0;

    virtual unsigned int getMaxSubjectID()const=0;
    virtual unsigned int getMaxObjectID()const=0;
    //virtual unsigned int getMaxPredicateID()const=0;
    //virtual unsigned int getMaxGraphID()const=0;

    virtual IteratorUCharString *getSubjects()=0;
    virtual IteratorUCharString *getSubjects()const=0;
    virtual IteratorUCharString *getObjects()=0;
    virtual IteratorUCharString *getObjects()const=0;
    virtual IteratorUCharString *getShared()=0;
    virtual IteratorUCharString *getShared()const=0;
    //virtual IteratorUCharString *getPredicates()=0;
    //virtual IteratorUCharString *getGraphs()=0;

    virtual unsigned int getGlobalId(unsigned int id, DictionarySection position)const=0;
    virtual unsigned int getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position)const=0;
    virtual unsigned int getLocalId(unsigned int id, TripleComponentRole position)const=0;
    virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const=0;

    virtual std::string getType()const=0;
    virtual unsigned int getMapping()const=0;

    virtual void getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults)=0;

    virtual IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role)=0;
    virtual IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role)=0;
};

class ModifiableDictionary : virtual public Dictionary {
public:
	virtual ~ModifiableDictionary(){ }

    /**
    * Insert a new entry to the dictionary in the corresponding section according to the role (Subject, Predicate, Object).
    * @param str
    * @param role
    */
    virtual unsigned int insert(const std::string &str, const TripleComponentRole role)=0;

    /**
    * Function to be called before starting inserting entries to the dictionary to perform an initialization.
    */
    virtual void startProcessing(ProgressListener *listener = NULL)=0;

    /**
    * Function to be called right after we are done inserting entries, to perform any possible final cleanup.
    */
    virtual void stopProcessing(ProgressListener *listener = NULL)=0;

	virtual void push_back(DictionaryEntry* entry, DictionarySection pos)=0; 

};

}

#endif /* HDT_DICTIONARY_HPP_ */
