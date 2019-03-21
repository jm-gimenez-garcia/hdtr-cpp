/*
 * File: QuadsList.hpp
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

#ifndef QUADSLIST_H_
#define QUADSLIST_H_

#include <Triples.hpp>
#include "TripleIterators.hpp"
#include <HDTSpecification.hpp>
#include <SingleQuad.hpp>

namespace hdt {

class QuadsList : public ModifiableTriples {
private:
	std::vector<QuadID> arrayOfQuads;
    QuadID *ptrQ;
	TripleComponentOrder orderQ;
    size_t numValidQuads;

public:
	QuadsList();
	QuadsList(const std::vector<QuadID>& quadArray, const TripleComponentOrder& ord);
	QuadsList(HDTSpecification &specification);
	~QuadsList();


	// From Triples
	IteratorTripleID *search(TripleID &pattern);
	IteratorTripleID *searchJoin(TripleID &a, TripleID &b, unsigned short conditions){throw std::logic_error("Not Implemented"); return NULL;}
	float cost(TripleID &triple) const {throw std::logic_error("Not Implemented");return 0.0f;}
    size_t getNumberOfElements() const;
    size_t size() const;
	void save(std::ostream &output, ControlInformation &controlInfo, ProgressListener *listener = NULL);
	void load(std::istream &input, ControlInformation &controlInfo, ProgressListener *listener);
	size_t load(unsigned char *char_ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	void load(ModifiableTriples &input, ProgressListener *listener = NULL);
	void generateIndex(ProgressListener *listener){}
	void saveIndex(std::ostream &output, ControlInformation &controlInfo, ProgressListener *listener){}
    void loadIndex(std::istream &input, ControlInformation &controlInfo, ProgressListener *listener){}
    size_t loadIndex(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener){return 0;}
    bool isIndexed() const {return false;}
	void populateHeader(Header &header, string rootNode);
	void startProcessing(ProgressListener *listener=NULL){}
	void stopProcessing(ProgressListener *listener=NULL){}
	string getType() const;
	TripleComponentOrder getOrder() const;

	// From ModifiableTriples
	void insert(TripleID &triple);
	void insert(IteratorTripleID *triples);
	bool remove(TripleID &pattern);
	bool remove(IteratorTripleID *pattern);
	void sort(TripleComponentOrder ord, ProgressListener *listener = NULL);
	void removeDuplicates(ProgressListener *listener = NULL);
	void setOrder(TripleComponentOrder order);

	
	
	/**
	 *
	 * @param i
	 * @return
	 */
	QuadID *getQuadID(unsigned int i);

	friend class QuadsListIterator;
};

class QuadsListIterator : public IteratorTripleID {
private:
	QuadID pattern, returnQuad;
	QuadsList* quads;
	uint64_t pos;

public:
	QuadsListIterator(QuadsList *quad_list, TripleID& patt);
	// IteratorTripleID methods
	bool hasNext();
	TripleID* next();
	bool hasPrevious();
	TripleID* previous();
	void goToStart();

	//new methods
	size_t getNumberOfElements()const;
};



} // namespace hdt



#endif /* QUADSLIST_H_ */

