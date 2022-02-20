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

#ifndef HDT_TFS_DICTIONARY_HPP_
#define HDT_TFS_DICTIONARY_HPP_

#include "BaseFourSectionDictionary.hpp"
#include "TriplesDictionary.hpp"
//#include <Iterator.hpp>
//#include <HDTSpecification.hpp>"

//#include "../libdcs/CSD.h"

namespace hdt {

class TriplesFourSectionDictionary : virtual public BaseFourSectionDictionary, public TriplesDictionary {
protected:
	csd::CSD *predicates;

public:
	TriplesFourSectionDictionary();
	TriplesFourSectionDictionary(HDTSpecification &spec);
	~TriplesFourSectionDictionary();

	unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;

	void loadFourthSection(std::istream & input, IntermediateListener& iListener);
	void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);

	void importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);

	csd::CSD* getDictionarySection(unsigned int id, TripleComponentRole position) const;
	csd::CSD *getDictionarySection(DictionarySection section)const;
	
	IteratorUCharString* getPredicates();
	IteratorUCharString* getPredicates()const;
	//IteratorUCharString* getGraphs();

	void saveFourthSection(std::ostream & output, IntermediateListener& iListener);
	
	unsigned int getNpredicates()const;
	//unsigned int getNgraphs()const;


	unsigned int getMaxPredicateID()const;
	//unsigned int getMaxGraphID()const;

	uint64_t size()const;
	size_t getNumberOfElements()const;
	void getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults);
    IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);
    IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);

private:
	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const{
		return getGlobalId(mapping, id, position);}
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const{return getLocalId(mapping, id, position);}

	

protected:
	void create();
	void clear_loc();
};


}

#endif /* HDT_TFS_DICTIONARY_HPP_ */
