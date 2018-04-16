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

#ifndef HDT_GFS_DICTIONARY_HPP_
#define HDT_GFS_DICTIONARY_HPP_

#include <Iterator.hpp>
#include <HDTSpecification.hpp>
#include <Dictionary.hpp>

#include "../libdcs/CSD.h"

namespace hdt {

class GraphsFourSectionDictionary : public BaseFourSectionDictionary {
private:
	csd::CSD *graphs;

public:
	GraphsFourSectionDictionary();
	GraphsFourSectionDictionary(HDTSpecification &spec);
	~GraphsFourSectionDictionary();

	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;

	void loadFourthSection(std::istream & input, IntermediateListener& iListener);
	void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);	

	void importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);

	IteratorUCharString* getGraphs()const;
	void saveFourthSection(std::ostream& output, IntermediateListener& iListener);

	unsigned int getNgraphs()const;
	unsigned int getMaxGraphID()const;
	uint64_t size()const;
	size_t getNumberOfElements()const;

private:
	csd::CSD* getDictionarySection(unsigned int id, TripleComponentRole position) const;
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const;

protected:
	void clear();
	void create();
}

#endif /* HDT_GFS_DICTIONARY_HPP_ */
