/*
 * File: SingleQuad.hpp
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

#ifndef __HDT_SINGLEQUAD_HPP__
#define __HDT_SINGLEQUAD_HPP__

#include "HDTEnums.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include "SingleTriple.hpp"

using namespace std;

namespace hdt {

#define IS_VARIABLE(a) ( (a).size()>0 && (a).at(0)=='?')
#define IS_URI(a) ( (a).size()>0 && (a).at(0)!='<' && (a).at(0)!='_')
#define IS_LITERAL(a) ( (a).size()>0 && (a).at(0)=='"')

/**
 * Represents a single quad, where the subject, predicate, and object components are
 * represented using integer IDs after applying the dictionary conversion.
 */
class QuadID : public TripleID
{
protected:
	unsigned int identifier;

public:

	/**
	 * Create empty QuadID.
	 * @return
	 */
	QuadID(){}

	/**
	 * Create a new QuadID initialized using the supplied subject, predicate, object, identifier.
	 * @param subject
	 * @param predicate
	 * @param object
	 * @param identifier
	 * @return
	 */
	QuadID(const unsigned int subj, const unsigned int pred, const unsigned int obj, const unsigned int ident) :
		TripleID(subj, pred, obj),
		identifier(ident){}

	QuadID(const QuadID& qId) :
		TripleID(qId.subject, qId.predicate, qId.object),
		identifier(qId.identifier){}
	
	QuadID(const TripleID& trId, const unsigned int ident):
		TripleID(trId),
		identifier(ident){}

	~QuadID() {
	}

	/**
	 * Get the identifier component of this QuadID.
	 * @return
	 */
	unsigned int getIdentifier() const {
		return identifier;
	}

	/**
	 * Set the identifier component of this QuadID.
	 * @return
	 */
	void setIdentifier(const unsigned int ident) {
		identifier = ident;
	}

	void setAll(const unsigned int subj, const unsigned int pred, const unsigned int obj, const unsigned int ident) {
		subject = subj;
		predicate = pred;
		object = obj;
		identifier = ident;
	}


	/**
	 * Clear all components of the QuadID to zero values.
	 */
	void clear() {
		subject = 0;
		predicate = 0;
		object = 0;
		identifier = 0;
	}

	/**
	 * Serialize this QuadID to a stream.
	 * @param stream Stream to serialize
	 * @param ti QuadID
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadID &qi) {
		stream << qi.subject << " "<< qi.predicate <<" "<< qi.object <<" "<< qi.identifier;

		return stream;
	}

	/**
	 * Compares two quads
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	bool operator==(const QuadID &op) {
		return (subject == op.subject) && (object == op.object) && (predicate == op.predicate) && (identifier == op.identifier)
	}

	/**
	 * Checks wether two QuadID instances are different.
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	bool operator!=(const QuadID &operand) {
		return !(this->operator==(operand));
	} // !=()

	/**
	 * Compares two quads and returns -1, 0 or 1, establishing an order.
	 *
	 * @param other
	 * @return
	 */
	int compare(const QuadID &other) 
	{
		int result = subject - other.subject;

		if(result==0) 
		{
			result = predicate - other.predicate;
			if(result==0) 
				return object - other.object;
			 else 
				return result;
		}
		else 
			return result;
	}

	/**
	 * Check wether this subject matches the supplied pattern.
	 * The special value 0 acts as wildcard and means "any".
	 *
	 * @param pattern The pattern to match against
	 * @return boolean
	 */
	bool match(const QuadID& q) 
	{
		return ((q.subject == 0 || q.subject == subject)
			&& (q.predicate == 0 || q.predicate == predicate)
			&& (q.object == 0 || q.object == object)
			&& (q.identifier == 0 || q.identifier == identifier));
	}

	/**
	 * Replaces the contents of a quad with the provided replacement
	 *
	 * @param replacement
	 */
	void replace(const QuadID& q) 
	{setAll(q.subject, q.predicate, q.object, q.identifier);} // replace()

        /**
         * Check wether a QuadID is empty. i.e. all of the components are zero.
         *
         * @return boolean
         */
        bool isEmpty() const {return (subject==0 && predicate==0 && object==0);}

	/**
	 * Check wether a QuadID is valid. i.e. all of the components are non-zero.
	 *
	 * @return boolean
	 */
	bool isValid() const {
        return subject != 0 && predicate != 0 && object != 0;
	}

	/**
	 * Get
	 */
	std::string getPatternString(){
		std::string tmp;
		tmp.append(subject==0 ? "?" : "S");
		tmp.append(predicate==0 ? "?" : "P");
		tmp.append(object==0 ? "?" : "O");
		tmp.append(identifier==0 ? "?" : "I");
		return tmp;
	}
};


/**
 * Represents a Quad where any of the componets subject,predicate,object,identifier are strings.
 */
class QuadString : public TripleString
{
private:
	std::string identifier;

public:
	/**
	 * Create empty QuadString.
	 * @return
	 */
	QuadString() {}

	QuadString(const QuadString& q):
		TripleString(q.subject, q.predicate, q.object),
		identifier(q.identifier){}

	QuadString(const TripleString& t, const std::string ident):
		TripleString(t.getSubject(), t.getPredicate(), t.getObject()),
		identifier(ident){}



	/** Create QuadString with the supplied components.
	 *
	 * @param subject
	 * @param predicate
	 * @param object
	 * @param identifier
	 * @return
	 */
	QuadString(const std::string subj, const std::string pred, const std::string obj, const std::string ident) :
		TripleString(subj,pred,obj),
		identifier(ident){}


	QuadString& operator=(const QuadString& other) {
		if(this!=&other) 
		{
			subject = other.subject;
			predicate = other.predicate;
			object = other.object;
			identifier = other.identifier;
		}
		return *this;
	}

	~QuadString() {}


	void setAll(const std:: string &subj, const std:: string &pred, const std:: string &obj, const std:: string& ident) 
	{
		subject = subj;
		predicate = pred;
		object = obj;
		identifier = ident;
	}

	/**
	 * Get Identifier.
	 * @return
	 */
	const std::string& getIdentifier() const
	{return identifier;}

	/**
	 * Set Identifier.
	 * @param identifier
	 */
	void setIdentifier(std::string& ident) 
	{identifier = ident;}

	/**
	 * Serialize QuadString to a stream.
	 * @param stream
	 * @param ts
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadString &ts) 
	{
		stream << ts.subject << " "<< ts.predicate <<" "<< ts.object <<" "<< ts.identifier;
		return stream;
	}

    bool operator==(const QuadString& q) const
	{return ((subject==q.subject) && (object==q.object) && (predicate==q.predicate) && (identifier==q.identifier));}

    bool operator!=(const QuadString &operand)  const
	{return !(this->operator==(operand));}

    inline bool match(const QuadString& q) const
	{
		return ((subject==q.subject || q.subject=="")
		  	&& (predicate==q.predicate || q.predicate=="")
			&& (object==q.object || q.object=="")
			&& (identifier==q.identifier || q.identifier==""));
    }

	/**
	 * Clear all components to the empty String "";
	 */
	void clear() 
	{subject = predicate = object = identifier = "";}

	/**
	 * Check wether all components of the QuadString are empty.
	 * @return
	 */
	bool isEmpty() const {
		return subject == "" && predicate == "" && object == "" && identifier == "";
	}

	/**
	 * Check wether any of the components of the QuadString is empty.
	 * @return
	 */
	bool hasEmpty() const {
		return subject == "" || predicate == "" || object == "" || identifier == "";
	}

	/**
	 * Read a QuadString from a stream, where each component is represented using an empty space.
	 * @param line
	 */
	void read(const std::string& line)
	{
		size_t pos_a = 0, pos_b;

		// Reads the subject
		pos_b = line.find(" ", pos_a);
		subject = line.substr(pos_a, pos_b - pos_a);
		//if(subject[0]=='?')	subject = "";
		pos_a = pos_b + 1;

		// Reads the predicate
		pos_b = line.find(" ", pos_a);
		predicate = line.substr(pos_a, pos_b - pos_a);
		//if(predicate[0]=='?') predicate = "";
		pos_a = pos_b + 1;

		// Reads the object
		pos_b = line.find(" ", pos_a);
		object = line.substr(pos_a, pos_b - pos_a);
		//if(object[0]=='?') object = "";
		pos_a = pos_b+1;

		// Reads the identifier
		pos_b = line.find(" ", pos_a);
		identifier = line.substr(pos_a, pos_b - pos_a);
		//if(object[0]=='?') identifier = "";
		pos_a = pos_b;
	}
};

}

#endif /* HDT_QUADTRIPLE_HPP_ */
