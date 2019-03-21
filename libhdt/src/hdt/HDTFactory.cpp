/*
 * File: HDTFactory.cpp
 * Last modified: $Date$
 * Revision: $Revision$
 * Last modified by: $Author$
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
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */
#include <stdexcept>
#include <HDT.hpp>
#include <HDTVocabulary.hpp>

#include "HDTFactory.hpp"

#include "BasicHDT.hpp"
#include "BasicModifiableHDT.hpp"

#include "../header/PlainHeader.hpp"

#include <Dictionary.hpp>
#include "../dictionary/TriplesDictionary.hpp"
#include "../dictionary/TriplesPlainDictionary.hpp"
#include "../dictionary/GraphsPlainDictionary.hpp"
#include "../dictionary/TriplesFourSectionDictionary.hpp"
#include "../dictionary/GraphsFourSectionDictionary.hpp"
#include "../dictionary/QueryableReificationDictionary.hpp"
#include "../dictionary/ModifiableReificationDictionary.hpp"
#ifdef HAVE_CDS
#include "../dictionary/TriplesLiteralDictionary.hpp"
#include "../dictionary/GraphsLiteralDictionary.hpp"
#endif

#include "../triples/TriplesList.hpp"
#include "../triples/QuadsList.hpp"
#include "../triples/BitmapQuads.hpp"
#ifndef WIN32
#include "../triples/TripleListDisk.hpp"
#endif
#include "../triples/PlainTriples.hpp"
#include "../triples/BitmapTriples.hpp"


using namespace hdt;


namespace hdt {

HDT *HDTFactory::createDefaultHDT()
{
	BasicHDT *h = new BasicHDT();
	return h;
}

HDT *HDTFactory::createHDT(HDTSpecification &spec)
{
	BasicHDT *h = new BasicHDT(spec);
	return h;
}

ModifiableHDT *HDTFactory::createDefaultModifiableHDT()
{
	BasicModifiableHDT *h = new BasicModifiableHDT();
	return h;
}

ModifiableHDT *HDTFactory::createModifiableHDT(HDTSpecification &spec)
{
	BasicModifiableHDT *h = new BasicModifiableHDT(spec);
	return h;
}

Triples *HDTFactory::readTriples(ControlInformation &controlInformation) {
	std::string triplesType = controlInformation.getFormat();

	if(triplesType==HDTVocabulary::TRIPLES_TYPE_BITMAP) {
		return new BitmapTriples();
	} else if(triplesType==HDTVocabulary::QUADS_TYPE_BITMAP) {
		return new BitmapQuads();
	} else if(triplesType==HDTVocabulary::TRIPLES_TYPE_PLAIN) {
		return new PlainTriples();
	} else if(triplesType==HDTVocabulary::TRIPLES_TYPE_TRIPLESLIST) {
		return new TriplesList();
	} else if(triplesType==HDTVocabulary::QUADS_TYPE_TRIPLESLIST) {
		return new QuadsList();
#ifndef WIN32
	} else if(triplesType==HDTVocabulary::TRIPLES_TYPE_TRIPLESLISTDISK) {
		return new TripleListDisk();
#endif
	}

	throw std::runtime_error("Triples Implementation not available");
}

Dictionary *HDTFactory::readDictionary(ControlInformation &controlInformation) {
	std::string type = controlInformation.getFormat();

	if(type==HDTVocabulary::DICTIONARY_TYPE_PLAIN) {
		return new TriplesPlainDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_PLAIN_GRAPH) {
		return new GraphsPlainDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_FOUR) {
		return new TriplesFourSectionDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_FOUR_GRAPH) {
		return new GraphsFourSectionDictionary();

	} else if(type==HDTVocabulary::DICTIONARY_TYPE_LITERAL) {
#ifdef HAVE_CDS
		return new TriplesLiteralDictionary();
#else
		throw std::runtime_error("This version has been compiled without support for this dictionary");
#endif
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_LITERAL_GRAPH) {
#ifdef HAVE_CDS
		return new GraphsLiteralDictionary();
#else
		throw std::runtime_error("This version has been compiled without support for this dictionary");
#endif
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_QUERYABLEREIFICATION) {
		return new QueryableReificationDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_MODIFIABLEREIFICATION) {
		return new ModifiableReificationDictionary();
	}

	throw std::runtime_error("Dictionary Implementation not available");
}

TriplesDictionary *HDTFactory::readTriplesDictionary(ControlInformation &controlInformation) {
	std::string type = controlInformation.getFormat();

	if(type==HDTVocabulary::DICTIONARY_TYPE_PLAIN) {
		return new TriplesPlainDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_FOUR) {
		return new TriplesFourSectionDictionary();

	} else if(type==HDTVocabulary::DICTIONARY_TYPE_LITERAL) {
#ifdef HAVE_CDS
		return new TriplesLiteralDictionary();
#else
		throw std::runtime_error("This version has been compiled without support for this dictionary");
#endif
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_QUERYABLEREIFICATION) {
		return new QueryableReificationDictionary();
	} else if(type==HDTVocabulary::DICTIONARY_TYPE_MODIFIABLEREIFICATION) {
		return new ModifiableReificationDictionary();
	}

	throw std::runtime_error("Dictionary Implementation not available");
}

Header *HDTFactory::readHeader(ControlInformation &controlInformation) {
    if(controlInformation.getType()!=HEADER)
		throw std::runtime_error("Trying to get Header from Non-Header section");

	return new PlainHeader();
}


}
