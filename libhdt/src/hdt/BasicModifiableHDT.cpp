/*
 * HDTRW.cpp
 *
 *  Created on: 11/08/2012
 *      Author: mck
 */
#include <stdexcept>
#include "BasicModifiableHDT.hpp"
#include "ModifiableReificationDictionary.hpp"
#include "TripleIDStringIterator.hpp"
#include "../util/StopWatch.hpp"

#include "TriplesDictionary.hpp"
#include "TriplesPlainDictionary.hpp"
#include "GraphsPlainDictionary.hpp"

#include "PlainHeader.hpp"

#include "TriplesList.hpp"
#include "QuadsList.hpp"
#include "TriplesKyoto.hpp"
#ifndef WIN32
#include "../triples/TripleListDisk.hpp"
#endif
namespace hdt {

BasicModifiableHDT::BasicModifiableHDT() : trTrans(NULL) {
	createComponents();
}

BasicModifiableHDT::BasicModifiableHDT(HDTSpecification &spec) : trTrans(NULL) {
	this->spec = spec;
	createComponents();
}

BasicModifiableHDT::~BasicModifiableHDT() {
	delete header;
	delete dictionary;
	delete triples;
}

void BasicModifiableHDT::createComponents() {
	try{
		std::string dictType = spec.get("dictionary.type");
		std::string triplesType = spec.get("triples.type");
	}
	 catch (std::exception& e)
	  {
	  }

	// FIXME: SELECT
	header = new PlainHeader();
	ModifiableReificationDictionary* mrDict = new ModifiableReificationDictionary();
	dictionary = mrDict;
	trTrans = mrDict;
	//triples = new TriplesList();
	triples = new QuadsList();
	if(trTrans)
	{
		triples->setToGlobalIDFunction(trTrans->getToGlobalIDFunction());
		triples->setToRoleIDFunction(trTrans->getToRoleIDFunction());
	}
}



Header *BasicModifiableHDT::getHeader()
{
    return header;
}

TriplesDictionary *BasicModifiableHDT::getDictionary()
{
    return dictionary;
}

Triples *BasicModifiableHDT::getTriples()
{
    return triples;
}

IteratorTripleString *BasicModifiableHDT::search(const char *subject, const char *predicate, const char *object, const char* graph/*=NULL*/)
{
	TripleID* role_tid_ptr=NULL;
	TripleID* glob_tid_ptr=NULL;
	if(graph==NULL)
	{
		const TripleString ts(subject, predicate, object);
		TripleID tid;
		dictionary->tripleStringtoTripleID(&ts, &tid);
		glob_tid_ptr = new TripleID(tid);

	} else {
		const QuadString qs(subject, predicate, object, graph);
		QuadID qid;
		dictionary->quadStringtoQuadID(&qs, &qid);
		glob_tid_ptr = new QuadID(qid);
	}

	triples->toRoleIDs(role_tid_ptr, *glob_tid_ptr);

	delete glob_tid_ptr;
	glob_tid_ptr = NULL;

	IteratorTripleID* iterID = triples->search(*role_tid_ptr);

	delete glob_tid_ptr;
	glob_tid_ptr = NULL;


	if(role_tid_ptr)
	{
		delete role_tid_ptr;
		role_tid_ptr = NULL;
	}

	TripleIDStringIterator *iterator = new TripleIDStringIterator(dictionary, iterID);
	return iterator;
}

VarBindingString *BasicModifiableHDT::searchJoin(vector<TripleString> &patterns, set<string> &vars)
{
	throw std::logic_error("Not Implemented");
}

void BasicModifiableHDT::loadFromRDF(const char *fileName, string baseUri, RDFNotation notation, ProgressListener *listener)
{

}

void BasicModifiableHDT::saveToRDF(RDFSerializer &serializer, ProgressListener *listener)
{
    IteratorTripleString *it = search("","","");
    serializer.serialize(it, listener, this->getTriples()->getNumberOfElements() );
	delete it;
}

void BasicModifiableHDT::loadFromHDT(const char *fileName, ProgressListener *listener) {
	ifstream input(fileName, ios::binary | ios::in);
	if(!input.good()){
		throw std::runtime_error("Error opening file to save HDT.");
	}
	this->loadFromHDT(input, listener);
    input.close();
}

void BasicModifiableHDT::mapHDT(const char *fileName, ProgressListener *listener)
{
    throw std::logic_error("Not Implemented");
}

void BasicModifiableHDT::loadFromHDT(std::istream & input, ProgressListener *listener)
{
	//header->load(input);
	ControlInformation controlInformation;
	controlInformation.load(input);
	dictionary->load(input, controlInformation);

	controlInformation.clear();
	controlInformation.load(input);
	triples->load(input, controlInformation);
}

void BasicModifiableHDT::loadOrCreateIndex(ProgressListener *listener) {

}

void BasicModifiableHDT::saveIndex(ProgressListener *listener) {

}

void BasicModifiableHDT::saveToHDT(const char *fileName, ProgressListener *listener)
{
	ofstream out(fileName, ios::binary | ios::out);
	if(!out.good()){
		throw std::runtime_error("Error opening file to save HDT.");
	}
	this->saveToHDT(out, listener);
	out.close();

	this->fileName = fileName;
}

void BasicModifiableHDT::saveToHDT(std::ostream & output, ProgressListener *listener)
{
	ControlInformation controlInformation;

	//header->save(output);

	cerr << "Saving dictionary" << endl;
	StopWatch st;
	dictionary->save(output, controlInformation);
	cerr << "Dictionary saved in " << st << endl;

	cerr << "Saving triples" << endl;
	st.reset();
	triples->save(output, controlInformation);
	cerr << "Triples saved in " << st << endl;

	this->fileName = fileName;
}

void BasicModifiableHDT::insert(TripleString & triple)
{
	TripleID tid;
	dictionary->tripleStringtoTripleID(&triple, &tid);
	triples->insert(tid);
}


void BasicModifiableHDT::insert(IteratorTripleString *triples)
{
	throw std::logic_error("Not Implemented");
}

void BasicModifiableHDT::remove(TripleString & triple)
{
	TripleID tid;
	dictionary->tripleStringtoTripleID(&triple, &tid);
	triples->remove(tid);

	// Fixme: Need to remove from dictionary?
}

void BasicModifiableHDT::remove(IteratorTripleString *triples)
{
	throw std::logic_error("Not Implemented");
}

}
