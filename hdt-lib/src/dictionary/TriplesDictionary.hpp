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

#ifndef HDT_TRIPLES_DICTIONARY_HPP_
#define HDT_TRIPLES_DICTIONARY_HPP_

#include "HDTListener.hpp"
#include "SingleTriple.hpp"
#include "Iterator.hpp"
#include "HDTEnums.hpp"
#include "Header.hpp"
#include "ControlInformation.hpp"
#include "Triples.hpp"

#include <string>
#include <iostream>

namespace hdt {

/**
 * Interface representing the Dictionary part of the HDT representation.
 * Provides a means to convert Strings to ID and the other way around.
 */
class TriplesDictionary : public virtual Dictionary
{
public:
    virtual ~TriplesDictionary(){ }


    /* Return the number of different subjects of the current dictionary. */
    virtual unsigned int getNsubjects()=0;

    /* Return the number of diferent predicates of the current dictionary */
    virtual unsigned int getNpredicates()=0;

    /* Return the number of different objects of the current dictionary */
    virtual unsigned int getNobjects()=0;

    /* Return the number of shared subjects-objects of the current dictionary */
    virtual unsigned int getNshared()=0;


    /* Return the maximum subject ID of the dictionary. */
    virtual unsigned int getMaxSubjectID()=0;

    /* Return the maximum predicate ID of the dictionary. */
    virtual unsigned int getMaxPredicateID()=0;

    /* Return the maximum object ID of the dictionary. */
    virtual unsigned int getMaxObjectID()=0;


    virtual IteratorUCharString *getSubjects()=0;
    virtual IteratorUCharString *getPredicates()=0;
    virtual IteratorUCharString *getObjects()=0;
    virtual IteratorUCharString *getShared()=0;


};

class TriplesModifiableDictionary : public ModifiableDictionary, public TriplesDictionary  {
public:
	virtual ~TriplesModifiableDictionary(){ }

};

}

#endif /* HDT_TRIPLES_DICTIONARY_HPP_ */
