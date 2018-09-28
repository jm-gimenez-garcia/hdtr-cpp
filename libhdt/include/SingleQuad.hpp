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
	unsigned int graph;
	bool has_graph;


public:

	/**
	 * Create empty QuadID.
	 * @return
	 */
	QuadID(): TripleID(0, 0, 0), graph(0),has_graph(false){}

	/**
	 * Create a new QuadID initialized using the supplied subject, predicate, object, graph.
	 * @param subject
	 * @param predicate
	 * @param object
	 * @param graph
	 * @return
	 */
	QuadID(const unsigned int subj, const unsigned int pred, const unsigned int obj, const unsigned int ident) :
		TripleID(subj, pred, obj),
		graph(ident),has_graph(true){}

	QuadID(const QuadID& qId) :
		TripleID(qId.subject, qId.predicate, qId.object),
		graph(qId.graph),has_graph(true){}

	QuadID(const TripleID& trId):
		TripleID(trId),
		graph(0),has_graph(false) {}

	QuadID(const TripleID& trId, const unsigned int ident):
		TripleID(trId),
		graph(ident),has_graph(true){}

	~QuadID() {}

	QuadID to_QuadID() const {
		return *this;
	}
	TripleID to_TripleID() const {
		if(!has_graph)
			return TripleID(subject,predicate,object);
		else
			throw std::runtime_error("This QuadID is not a TripleID (there is a graph). Unable to do a lossless conversion from QuadID to TripleID.");
	}
	
	bool hasGraph()const{return has_graph;}

	/**
	 * Get the graph component of this QuadID.
	 * @return
	 */
	unsigned int getGraph() const {
		if (!hasGraph())
			throw std::runtime_error("This QuadID is a TripleID (no graph)");
		return graph;
	}

	/**
	 * Set the graph component of this QuadID.
	 * @return
	 */
	void setIdentifier(const unsigned int ident) {
		graph = ident;
		has_graph = true;
	}

	void setAll(const unsigned int subj, const unsigned int pred, const unsigned int obj, const unsigned int ident) {
		subject = subj;
		predicate = pred;
		object = obj;
		graph = ident;
		has_graph = true;
	}


	/**
	 * Clear all components of the QuadID to zero values.
	 */
	void clear() {
		subject = 0;
		predicate = 0;
		object = 0;
		graph = 0;
		has_graph=false;
	}

	/**
	 * Serialize this QuadID to a stream.
	 * @param stream Stream to serialize
	 * @param ti QuadID
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadID &qi) {
		if(qi.hasGraph())
			stream << qi.subject << " "<< qi.predicate <<" "<< qi.object <<" "<< qi.graph;
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
		return (subject == op.subject) && (object == op.object) && (predicate == op.predicate) && ((has_graph && op.has_graph && graph == op.graph) || !(has_graph||op.has_graph));
	}

	bool operator==(const TripleID &op)const {
		return this->operator==(op.to_QuadID());
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
		return !this->operator==(op.to_QuadID());
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
					if (!(has_graph || other.has_graph))
						return 0;
					else
						return graph - other.graph;
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
		return compare(other.to_QuadID());
	}


	/**
	 * Check wether this subject matches the supplied pattern.
	 * The special value 0 acts as wildcard and means "any".
	 *
	 * @param pattern The pattern to match against
	 * @return boolean
	 */
	bool match(const QuadID& patt)const {
		return (patt.subject == 0 || patt.subject == subject)
			&& (patt.predicate == 0 || patt.predicate == predicate)
			&& (patt.object == 0 || patt.object == object)
			&& ((patt.hasGraph() && hasGraph() && (patt.graph==0 || patt.graph == graph)) || !patt.hasGraph());
	}

	/**
	 * Replaces the contents of a quad with the provided replacement
	 *
	 * @param replacement
	 */
	void replace(const QuadID& q) 
	{
		if (q.hasGraph())
			setAll(q.subject, q.predicate, q.object, q.graph); // replace()
		else
		{
			setAll(q.subject, q.predicate, q.object, 0); // replace()
			has_graph = false;	
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
			return (TripleID::isEmpty() && ((has_graph && graph==0) || (!has_graph)));
		}

	/**
	 * Check wether a QuadID is valid. i.e. all of the components are non-zero.
	 *
	 * @return boolean
	 */
	bool isValid() const {
            return TripleID::isValid() && (!hasGraph() || (hasGraph() && graph==0));
	}

	/**
	 * Get
	 */
	std::string getPatternString()const{
		std::string tmp;
		tmp.append(subject==0 ? "?" : "S");
		tmp.append(predicate==0 ? "?" : "P");
		tmp.append(object==0 ? "?" : "O");
		if (has_graph)
			tmp.append(graph==0 ? "?" : "G");
		return tmp;
	}
};


/**
 * Represents a Quad where any of the componets subject,predicate,object,graph are strings.
 */
class QuadString : public TripleString
{
private:
	std::string graph;
	bool has_graph;

public:
	/**
	 * Create empty QuadString.
	 * @return
	 */
	QuadString() {}

	QuadString(const QuadString& q):
		TripleString(q.subject, q.predicate, q.object),
		graph(q.graph), has_graph(true){}

	QuadString(const TripleString& t):
		TripleString(t.getSubject(), t.getPredicate(), t.getObject()), has_graph(false){}

	QuadString(const TripleString& t, const std::string ident):
		TripleString(t.getSubject(), t.getPredicate(), t.getObject()),
		graph(ident), has_graph(true){}



	/** Create QuadString with the supplied components.
	 *
	 * @param subject
	 * @param predicate
	 * @param object
	 * @param graph
	 * @return
	 */
	QuadString(const std::string subj, const std::string pred, const std::string obj, const std::string ident) :
		TripleString(subj,pred,obj), graph(ident), has_graph(true){}

	QuadString& operator=(const QuadString& other) {
		if(this!=&other) 
		{
			subject = other.subject;
			predicate = other.predicate;
			object = other.object;
			graph = other.graph;
			has_graph = other.has_graph;
		}
		return *this;
	}

	~QuadString() {}

	// returns a copy of the object
	QuadString to_QuadString()const{return *this;}
	TripleString to_TripleString()const{
		if(!has_graph)
			return TripleString(subject,predicate,object);
		else
			throw std::runtime_error("This QuadString is not a TripleString (there is a graph). Unable to do a lossless conversion from QuadString to TripleString.");
	}

	virtual bool hasGraph()const{return has_graph;}


	void setAll(const std:: string &subj, const std:: string &pred, const std:: string &obj, const std:: string& ident) 
	{
		subject = subj;
		predicate = pred;
		object = obj;
		graph = ident;
		has_graph = true;
	}

	/**
	 * Get Identifier.
	 * @return
	 */
	std::string getGraph() const{
		return hasGraph() ? graph : 0;
	}

	/**
	 * Set Identifier.
	 * @param graph
	 */
	void setIdentifier(std::string& ident) {
		graph = ident;
		has_graph = true;
	}

	/**
	 * Serialize QuadString to a stream.
	 * @param stream
	 * @param ts
	 * @return
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QuadString &ts) {
		stream << ts.subject << " "<< ts.predicate <<" "<< ts.object <<" "<< ts.graph;
		return stream;
	}

    bool operator==(const QuadString& qs) const	{
		return (subject==qs.subject) && (object==qs.object) && (predicate==qs.predicate) && ((has_graph && qs.has_graph && graph == qs.graph) || !(has_graph||qs.has_graph));
	}
    
	bool operator==(const TripleString& ts) const{
		return this->operator==(ts.to_QuadString());
	}

    bool operator!=(const QuadString &operand)const{
		return !(this->operator==(operand));
	}

    bool operator!=(const TripleString &operand)const{
		return !(this->operator==(operand.to_QuadString()));
	}

    bool match(const QuadString& patt) const{
		return ((subject==patt.subject || patt.subject=="")
		  	&& (predicate==patt.predicate || patt.predicate=="")
			&& (object==patt.object || patt.object=="")
			&& ((patt.hasGraph() && hasGraph() && (patt.graph=="" || patt.graph == graph)) || !patt.hasGraph()));
    }

	/**
	 * Clear all components to the empty String "";
	 */
	void clear() 
	{subject = predicate = object = graph = ""; has_graph=false;}

	/**
	 * Check wether all components of the QuadString are empty.
	 * @return
	 */
	bool isEmpty() const {
		return (TripleString::isEmpty() && ((has_graph && graph=="") || (!has_graph)));
	}

	/**
	 * Check wether any of the components of the QuadString is empty.
	 * @return
	 */
	bool hasEmpty() const {
		return TripleString::hasEmpty() || (hasGraph() && graph == "") ;
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

		// Reads the graph
		pos_b = line.find(" ", pos_a);
		graph = line.substr(pos_a, pos_b - pos_a);
		//if(object[0]=='?') graph = "";
		pos_a = pos_b;
	}
};

}

#endif /* HDT_QUADTRIPLE_HPP_ */
