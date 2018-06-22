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
private:
	unsigned int identifier;
	bool has_identifier;

protected:
	size_t get_sizeof(){return sizeof(QuadID);}

public:

	/**
	 * Create empty QuadID.
	 * @return
	 */
	QuadID(): TripleID(0, 0, 0), identifier(0),has_identifier(false){}

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
		identifier(ident),has_identifier(true){}

	QuadID(const QuadID& qId) :
		TripleID(qId.subject, qId.predicate, qId.object),
		identifier(qId.identifier),has_identifier(true){}

	QuadID(const TripleID& trId):
		TripleID(trId),
		identifier(0),has_identifier(false) {}

	QuadID(const TripleID& trId, const unsigned int ident):
		TripleID(trId),
		identifier(ident),has_identifier(true){}

	~QuadID() {}

	inline QuadID to_QuadID() const {
		return *this;
	}

	bool hasIdentifier()const{return has_identifier;}

	/**
	 * Get the identifier component of this QuadID.
	 * @return
	 */
	unsigned int getIdentifier() const {
		if !hasIdentifier()
			throw std::runtime_error("This QuadID is a TripleID (no identifier)");
		return identifier;
	}

	/**
	 * Set the identifier component of this QuadID.
	 * @return
	 */
	void setIdentifier(const unsigned int ident) {
		identifier = ident;
		has_identifier = true;
	}

	void setAll(const unsigned int subj, const unsigned int pred, const unsigned int obj, const unsigned int ident) {
		subject = subj;
		predicate = pred;
		object = obj;
		identifier = ident;
		has_identifier = true;
	}


	/**
	 * Clear all components of the QuadID to zero values.
	 */
	void clear() {
		subject = 0;
		predicate = 0;
		object = 0;
		identifier = 0;
		has_identifier=false;
	}

	/**
	 * Serialize this QuadID to a stream.
	 * @param stream Stream to serialize
	 * @param ti QuadID
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadID &qi) {
		if(qi.hasIdentifier())
			stream << qi.subject << " "<< qi.predicate <<" "<< qi.object <<" "<< qi.identifier;
		else
			stream << qi.subject << " "<< qi.predicate <<" "<< qi.object <<" "<< (unsigned int)0;

		return stream;
	}

	/**
	 * Compares two quads
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	bool operator==(const QuadID &op)const {
		return (subject == op.subject) && (object == op.object) && (predicate == op.predicate) && ((has_identifier && op.has_identifier && identifier == op.identifier) || !(has_identifier||op.has_identifier))
	}

	bool operator==(const TripleID &op)const {
		return this->operator==(op.toQuadID());
	}


	/**
	 * Checks wether two QuadID instances are different.
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	bool operator!=(const QuadID &operand)const {
		return !(this->operator==(operand));
	} // !=()

	bool operator!=(const TripleID &op)const {
		return !this->operator==(op.toQuadID());
	} // !=()




	/**
	 * Compares two quads and returns -1, 0 or 1, establishing an order.
	 *
	 * @param other
	 * @return
	 */
	int compare(const QuadID &other)const
	{
		int result = subject - other.subject;

		if(result==0) 
		{
			result = predicate - other.predicate;
			if(result==0) 
			{
				result = object - other.object;
				if(result==0)
					if !(has_identifier || other. has_identifier)
						return 0;
					else
						return identifier - other.identifier;
				else 
					return result;
			}
			 else 
				return result;
		}
		else 
			return result;
	}

	int compare(const TripleID &other)const {
		return compare(other.toQuadID());
	}


	/**
	 * Check wether this subject matches the supplied pattern.
	 * The special value 0 acts as wildcard and means "any".
	 *
	 * @param pattern The pattern to match against
	 * @return boolean
	 */
	bool match(const QuadID& q)const {
		bool q_ident_wildcard = q.identifier==0 && q.hasIdentifier();
		return (q.subject == 0 || q.subject == subject)
			&& (q.predicate == 0 || q.predicate == predicate)
			&& (q.object == 0 || q.object == object)
			&& ((q.hasIdentifier() && hasIdentifier() && (q.identifier==0 || q.identifier == identifier)) || !(q.hasIdentifier() || hasIdentifier()));
	}
	bool match(const TripleID& t)const {
		return match(t.to_QuadID());
	}


	/**
	 * Replaces the contents of a quad with the provided replacement
	 *
	 * @param replacement
	 */
	void replace(const QuadID& q) 
	{
		if q.hasIdentifier()
			setAll(q.subject, q.predicate, q.object, q.identifier); // replace()
		else
		{
			setAll(q.subject, q.predicate, q.object, 0); // replace()
			has_identifier = false;	
		}
	}

	void replace(const TripleID& q){
		replace(q.to_QuadID());
	}

        /**
         * Check wether a QuadID is empty. i.e. all of the components are zero.
         *
         * @return boolean
         */
        bool isEmpty() const {
			return (TripleID::isEmpty() && ((has_identifier && identifier==0) || (!has_identifier)));
		}

	/**
	 * Check wether a QuadID is valid. i.e. all of the components are non-zero.
	 *
	 * @return boolean
	 */
	inline bool isValid() const {
            return TripleID::isValid() && (!hasIdentifier() || (hasIdentifier() && identifier==0));
	}

	/**
	 * Get
	 */
	std::string getPatternString()const{
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
	bool has_identifier;

public:
	/**
	 * Create empty QuadString.
	 * @return
	 */
	QuadString() {}

	QuadString(const QuadString& q):
		TripleString(q.subject, q.predicate, q.object),
		identifier(q.identifier), has_identifier(true){}

	QuadString(const TripleString& t):
		TripleString(t.getSubject(), t.getPredicate(), t.getObject()), has_identifier(false){}

	QuadString(const TripleString& t, const std::string ident):
		TripleString(t.getSubject(), t.getPredicate(), t.getObject()),
		identifier(ident), has_identifier(true){}



	/** Create QuadString with the supplied components.
	 *
	 * @param subject
	 * @param predicate
	 * @param object
	 * @param identifier
	 * @return
	 */
	QuadString(const std::string subj, const std::string pred, const std::string obj, const std::string ident) :
		TripleString(subj,pred,obj), identifier(ident), has_identifier(true){}


	QuadString& operator=(const QuadString& other) {
		if(this!=&other) 
		{
			subject = other.subject;
			predicate = other.predicate;
			object = other.object;
			identifier = other.identifier;
			has_identifier = other.has_identifier;
		}
		return *this;
	}

	~QuadString() {}

	// returns a copy of the object
	QuadString to_QuadString()const{return *this;}

	virtual bool hasIdentifier(){return has_identifier;}


	void setAll(const std:: string &subj, const std:: string &pred, const std:: string &obj, const std:: string& ident) 
	{
		subject = subj;
		predicate = pred;
		object = obj;
		identifier = ident;
		has_identifier = true;
	}

	/**
	 * Get Identifier.
	 * @return
	 */
	const std::string& getIdentifier() const{
		return hasIdentifier() ? identifier : 0;
	}

	/**
	 * Set Identifier.
	 * @param identifier
	 */
	void setIdentifier(std::string& ident) {
		identifier = ident;
		has_identifier = true;
	}

	/**
	 * Serialize QuadString to a stream.
	 * @param stream
	 * @param ts
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadString &ts) {
		stream << ts.subject << " "<< ts.predicate <<" "<< ts.object <<" "<< ts.identifier;
		return stream;
	}

    bool operator==(const QuadString& qs) const	{
		return (subject==qs.subject) && (object==qs.object) && (predicate==qs.predicate) && ((has_identifier && qs.has_identifier && identifier == qs.identifier) || !(has_identifier||oqs.has_identifier));
	}
    
	bool operator==(const TripleString& ts) const{
		return this->operator==(ts.to_QuadString());
	}

    bool operator!=(const QuadString &operand)const{
		return !(this->operator==(operand));
	}

    bool operator!=(const TripleString &operand)const{
		return !(this->operator==(operand.to_QuadID()));
	}

    inline bool match(const QuadString& q) const{
		return ((subject==q.subject || q.subject=="")
		  	&& (predicate==q.predicate || q.predicate=="")
			&& (object==q.object || q.object=="")
			&& ((q.hasIdentifier() && hasIdentifier() && (q.identifier=="" || q.identifier == identifier)) || !(q.hasIdentifier() || hasIdentifier()));
    }

	/**
	 * Clear all components to the empty String "";
	 */
	void clear() 
	{subject = predicate = object = identifier = ""; has_identifier=false;}

	/**
	 * Check wether all components of the QuadString are empty.
	 * @return
	 */
	bool isEmpty() const {
		return (TripleString::isEmpty() && ((has_identifier && identifier==0) || (!has_identifier)));
	}

	/**
	 * Check wether any of the components of the QuadString is empty.
	 * @return
	 */
	bool hasEmpty() const {
		return TripleString::hasEmpty() || (hasIdentifier && identifier == "") ;
	}

	/**
	 * Read a QuadString from a stream, where each component is represented using an empty space.
	 * @param line
	 */
	void read(std::string line)
	{
		size_t pos_a = 0, pos_b;

		//trim the line
		line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
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
