/*
 * File: BasicHDT.hpp
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

#ifndef BASICHDT_HPP_
#define BASICHDT_HPP_

#include <HDTSpecification.hpp>
#include <HDT.hpp>

#include "../util/filemap.h"

namespace hdt {
class TriplesDictionary;
class ModifiableTriplesDictionary;
class ModifiableReificationDictionary;
class ModifiableTriples;
class ProgressListener;
class Header;
class Dictionary;
class Triples;
class TripleTranslator;
class FileMap;

class BasicHDT : public HDT {
private:
	Header *header;
	TriplesDictionary *dictionary;
	Triples *triples;
	HDTSpecification spec;
	string fileName;
	TripleTranslator* trTrans;

	FileMap *mappedHDT, *mappedIndex;

	void createComponents();
	void deleteComponents();

	ModifiableTriplesDictionary *getLoadTrModifDictionary();
	ModifiableReificationDictionary *getLoadReifModifDictionary();
	ModifiableTriples *getLoadTriples();

	void loadDictionary(const char *fileName, const char *baseUri, RDFNotation notation, ProgressListener *listener);
	void loadTriples(const char *fileName, const char *baseUri, RDFNotation notation, ProgressListener *listener);

	void addDictionaryFromHDT(const char *fileName, ModifiableDictionary *dict, ProgressListener *listener=NULL);
	void loadDictionaryFromHDTs(const char** fileName, size_t numFiles, const char* baseUri, ProgressListener* listener=NULL);
	void loadTriplesFromHDTs(const char** fileNames, size_t numFiles, const char* baseUri, ProgressListener* listener=NULL);

	void fillHeader(const string &baseUri);

    size_t loadMMap(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
    size_t loadMMapIndex(ProgressListener *listener=NULL);

public:
	BasicHDT();

	BasicHDT(HDTSpecification &spec);

	virtual ~BasicHDT();

	/**
	 *
	 */
	Header *getHeader();

	/**
	 *
	 */
	TriplesDictionary *getDictionary();

	/**
	 *
	 */
	Triples *getTriples();

	void loadFromRDF(const char *fileName, string baseUri, RDFNotation notation, ProgressListener *listener = NULL);

	/**
	 * @param input
	 */
	void loadFromHDT(std::istream &input, ProgressListener *listener = NULL);

	/**
	 * @param input
	 */
	void loadFromHDT(const char *fileName, ProgressListener *listener = NULL);

	void loadHeader(const char *fileName, ProgressListener *listener);

	void loadFromSeveralHDT(const char **fileNames, size_t numFiles, string baseUri, ProgressListener *listener=NULL);

    /**
     * Load an HDT from a file, using memory mapping
     * @param input
     */
    void mapHDT(const char *fileName, ProgressListener *listener = NULL);

	/**
	 * @param output
	 * @param notation
	 */
	void saveToRDF(RDFSerializer &serializer, ProgressListener *listener = NULL, TripleString* patt=NULL);

	/**
	 * @param output
	 */
	void saveToHDT(std::ostream &output, ProgressListener *listener = NULL);

	/**
	 * @param output
	 */
	void saveToHDT(const char *fileName, ProgressListener *listener = NULL);

	void loadOrCreateIndex(ProgressListener *listener = NULL);

	void saveIndex(ProgressListener *listener = NULL);

	/**
	 * @param subject
	 * @param predicate
	 * @param object
	 * @return
	 */
	IteratorTripleString *search(const char *subject, const char *predicate, const char *object, const char* graph=NULL);

    bool isIndexed() const {
        return triples->isIndexed();
    }
};

}

#endif /* BASICHDT_HPP_ */
