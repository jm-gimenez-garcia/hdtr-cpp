/*
 * File: SingleTriple.hpp
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

#ifndef HDT_SINGLETRIPLE_HPP_
#define HDT_SINGLETRIPLE_HPP_

#include <HDTEnums.hpp>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace hdt {

#define IS_VARIABLE(a) ( (a).size()>0 && (a).at(0)=='?')
#define IS_URI(a) ( (a).size()>0 && (a).at(0)!='<' && (a).at(0)!='_')
#define IS_LITERAL(a) ( (a).size()>0 && (a).at(0)=='"')

class QuadID;
class QuadString;
/**
 * Represents a single triple, where the subject, predicate, and object components are
 * represented using integer IDs after applying the dictionary conversion.
 */
class TripleID
{
protected:
	unsigned int subject;
	unsigned int predicate;
	unsigned int object;


public:

	/**
	 * Create empty TripleID.
	 * @return
	 */
	TripleID() {
	}

	/**
	 * Create a new TripleID initialized using the supplied subject, predicate, object.
	 * @param subject
	 * @param predicate
	 * @param object
	 * @return
	 */
	TripleID(const unsigned int subj, const unsigned int pred, const unsigned int obj) :
		subject(subj),
		predicate(pred),
		object(obj){}

	TripleID(const TripleID& tid):
		subject(tid.subject),
		predicate(tid.predicate),
		object(tid.object){}
	
	virtual ~TripleID() {}

	virtual TripleID* new_copy()const{return new TripleID(*this);}

	virtual QuadID to_QuadID() const ;
	virtual TripleID to_TripleID() const ;
	QuadID to_QuadID_pattern() const ;

	virtual bool hasGraph()const{return false;}
	/**
	 * Get the Subject component of this tripleID.
	 * @return
	 */
	unsigned int getSubject() const {
		return subject;
	}


	/**
	 * Set the Subject component of this tripleID.
	 * @return
	 */
	void setSubject(const unsigned int subject) {
		this->subject = subject;
	}

	/**
	 * Get the Predicate component of this tripleID.
	 * @return
	 */
	unsigned getPredicate() const {
		return this->predicate;
	}

	/**
	 * Set the Predicate component of this tripleID.
	 * @return
	 */
	void setPredicate(const unsigned int predicate) {
		this->predicate = predicate;
	}

	/**
	 * Get the Object component of this tripleID.
	 * @return
	 */
	unsigned int getObject() const {
		return this->object;
	}

	/**
	 * Set the Object component of this tripleID.
	 * @return
	 */
	void setObject(const unsigned int object) {
		this->object = object;
	}


	virtual void setAll(const unsigned int subject, const unsigned int predicate, const unsigned int object) {
		this->subject = subject;
		this->predicate = predicate;
		this->object = object;
	}

	/**
	 * Clear all components of the TripleID to zero values.
	 */
	void clear() {
		this->subject = 0;
		this->predicate = 0;
		this->object = 0;
	}

	virtual unsigned int getSizeof()const{
		return sizeof(*this);
	}

	virtual void print(std::ostream& os)const{
		os << getSubject() << " "<< getPredicate() <<" "<< getObject();
	}
	/**
	 * Serialize this TripleID to a stream.
	 * @param stream Stream to serialize
	 * @param ti TripleID
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const TripleID &ti) {
		//stream << ti.subject << " "<< ti.predicate <<" "<< ti.object;
		ti.print(stream);

		return stream;
	}

	/**
	 * Compares two triples
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	virtual bool operator==(const TripleID &operand)const {
		// Subject comparison
		if (this->subject != operand.subject) {
			return false;
		}

		// Object comparison (since subject was successful)
		if (this->object != operand.object) {
			return false;
		}

		// Predicate comparison (since subject and object were successful)
		if (this->predicate != operand.predicate) {
			return false;
		}
		return true;
	}

	/**
	 * Checks wether two TripleID instances are different.
	 *
	 * @param operand The operand to compare with
	 * @return boolean
	 */
	virtual bool operator!=(const TripleID &operand)const {
		return !(this->operator==(operand));
	} // !=()

	/**
	 * Compares two triples and returns -1, 0 or 1, establishing an order.
	 *
	 * @param other
	 * @return
	 */
	virtual int compare(const TripleID &other)const {
		int result = this->subject - other.subject;

		if(result==0) {
			result = this->predicate - other.predicate;
			if(result==0) {
				return this->object - other.object;
			} else {
				return result;
			}
		} else {
			return result;
		}
	}

	/**
	 * Check wether this subject matches the supplied pattern.
	 * The special value 0 acts as wildcard and means "any".
	 *
	 * @param pattern The pattern to match against
	 * @return boolean
	 */
	bool match(const TripleID &pattern)const {
		unsigned int subject = pattern.getSubject();
		unsigned int predicate = pattern.getPredicate();
                unsigned int object = pattern.getObject();

		if (subject == 0 || subject == this->subject) {
                    if (predicate == 0 || predicate == this->predicate) {
                        if (object == 0 || object == this->object) {
                            return true;
                        }
                    }
		}

		return false;
	}

	bool isBasedOn(const TripleID& patt)const {
		if(!patt.isValid() || !isValid())
		{
			throw std::runtime_error("Error in TripleID::isBasedOn : TripleID is not valid. Use 'match' method instead.");
			return false;
		}
		else
			return patt.subject==subject && patt.predicate==predicate && patt.object==object ;
	}
	/**
	 * Replaces the contents of a triple with the provided replacement
	 *
	 * @param replacement
	 */
	virtual void replace(const TripleID &replacement) {
		subject = replacement.getSubject();
		object = replacement.getObject();
		predicate = replacement.getPredicate();
	} // replace()

        /**
         * Check wether a TripleID is empty. i.e. all of the components are zero.
         *
         * @return boolean
         */
        bool virtual isEmpty() const {
            return !(this->subject != 0 || this->predicate != 0 || this->object != 0);
        }

	/**
	 * Check wether a TripleID is valid. i.e. all of the components are non-zero.
	 *
	 * @return boolean
	 */
	virtual bool isValid() const {
                return this->subject != 0 && this->predicate != 0 && this->object != 0;
	}

	/**
	 * Get
	 */
	virtual std::string getPatternString()const{
		std::string tmp;
		tmp.append(subject==0 ? "?" : "S");
		tmp.append(predicate==0 ? "?" : "P");
		tmp.append(object==0 ? "?" : "O");
		return tmp;
	}
};


/**
 * Represents a Triple where any of the componets subject,predicate,object are strings.
 */
class TripleString
{
protected:
	std::string subject;
	std::string predicate;
	std::string object;

public:
	/**
	 * Create empty TripleString.
	 * @return
	 */
	TripleString() {
	}

	/** Create TripleString with the supplied components.
	 *
	 * @param subject
	 * @param predicate
	 * @param object
	 * @return
	 */
	TripleString(const std::string subj, const std::string pred, const std::string obj):
		subject(subj),
		predicate(pred),
		object(obj){}

	TripleString(const TripleString &other) :
		subject(other.subject),
		predicate(other.predicate),
		object(other.object){}

	TripleString & operator=(const TripleString &other) {
		if(this!=&other) {
			this->subject = other.subject;
			this->predicate = other.predicate;
			this->object = other.object;
		}
		return *this;
	}

	virtual ~TripleString() {

	}

	virtual QuadString to_QuadString()const;
	virtual TripleString to_TripleString()const;
	QuadString to_QuadString_pattern()const;

	virtual bool hasGraph()const{return false;}


	/**
	 * Get Subject.
	 * @return
	 */
	const std::string &getSubject() const {
		return subject;
	}

	/**
	 * Set Subject.
	 * @param subject
	 */
	void setSubject(const std::string &subject) {
		this->subject = subject;
	}

	virtual void setAll(const std::string &subject, const std::string &predicate, const std::string &object) {
		this->subject = subject;
		this->predicate = predicate;
		this->object = object;
	}

	/**
	 * Get Predicate.
	 * @return
	 */
	const std::string &getPredicate() const {
		return predicate;
	}

	/** Set Predicate
	 *
	 * @param predicate
	 */
	void setPredicate(const std::string &predicate) {
		this->predicate = predicate;
	}

	/**
	 * Get Object.
	 * @return
	 */
	const std::string &getObject() const {
		return object;
	}

	/**
	 * Set Object.
	 * @param object
	 */
	void setObject(const std::string &object) {
		this->object = object;
	}

	virtual void print(std::ostream& os)const{
		os << getSubject() << " "<< getPredicate() <<" "<< getObject();
	}

	/**
	 * Serialize TripleString to a stream.
	 * @param stream
	 * @param ts
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const TripleString &ts) {
		//stream << ts.subject << " "<< ts.predicate <<" "<< ts.object;
		ts.print(stream);

		return stream;
	}

        bool operator==(const TripleString &operand)const {
                // Subject comparison
                if (this->subject != operand.subject) {
                        return false;
                }

                // Object comparison (since subject was successful)
                if (this->object != operand.object) {
                        return false;
                }

                // Predicate comparison (since subject and object were successful)
                if (this->predicate != operand.predicate) {
                        return false;
                }
                return true;
        }

        bool operator!=(const TripleString &operand)const {
                return !(this->operator==(operand));
        }

        bool match(const TripleString &pattern)const {
            string subject = pattern.getSubject();
            string predicate = pattern.getPredicate();
            string object = pattern.getObject();

            if (subject == "" || subject == this->subject) {
                if (predicate == "" || predicate == this->predicate) {
                    if (object == "" || object == this->object) {
                        return true;
                    }
                }
            }

            return false;
        }

	/**
	 * Clear all components to the empty String "";
	 */
	virtual void clear() {
		subject = predicate = object = "";
	}

	/**
	 * Check wether all components of the TripleString are empty.
	 * @return
	 */
	virtual bool isEmpty() const {
		return subject == "" && predicate == "" && object == "";
	}

	virtual unsigned int getSizeof()const{
		return sizeof(*this);
	}
	/**
	 * Check wether any of the components of the TripleString is empty.
	 * @return
	 */
	virtual bool hasEmpty() const {
		return subject == "" || predicate == "" || object == "";
	}

	/**
	 * Read a TripleString from a stream, where each component is represented using an empty space.
	 * @param line
	 */
	virtual void read(std::string line){
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
		if (pos_b == std::string::npos)
		{
			object = line.substr(pos_a);
			pos_a = line.length()-1;
		}
		else
		{
			object = line.substr(pos_a, pos_b - pos_a);
			pos_a = pos_b;
		}
	}
};

class VarBindingID {
public:
    virtual ~VarBindingID() { }

    virtual void goToStart()=0;
    virtual unsigned int estimatedNumResults()=0;
    virtual bool findNext()=0;

    virtual unsigned int getNumVars()=0;
    virtual const char *getVarName(unsigned int numvar)=0;
    virtual unsigned int getVarValue(unsigned int numvar)=0;
};

class VarBindingString {
public:
	virtual ~VarBindingString() { }

	virtual bool findNext()=0;
	virtual unsigned int getNumVars()=0;
	virtual string getVar(unsigned int numvar)=0;
	virtual const char *getVarName(unsigned int numvar)=0;
	virtual unsigned int estimatedNumResults()=0;
	virtual void goToStart()=0;
};

class EmptyVarBingingString : public VarBindingString {
public:
	bool findNext() {
	    return false;
	}
	unsigned int getNumVars() {
	    return 0;
	}
	string getVar(unsigned int /*numvar*/) {
	    throw std::runtime_error("No such variable");
	}
	const char *getVarName(unsigned int /*numvar*/) {
	    throw std::runtime_error("No such variable");
	}
	unsigned int estimatedNumResults() {
	    return 0;
	}
	void goToStart() {

	}
};

}

#endif /* HDT_SINGLETRIPLE_HPP_ */
