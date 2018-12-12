#include <iostream>
#include <exception>
#include "PlainHeader.hpp"
#include "BitmapTriples.hpp"
#include "BitmapQuads.hpp"
#include "PlainTriples.hpp"
#include "HDTVocabulary.hpp"
#include "TripleListDisk.hpp"
#include "TriplesList.hpp"
#include "TriplesQuadsList.hpp"
#include "TriplesLoader.hpp"
#include "DictionaryLoader.hpp"
#include "QueryableReificationDictionary.hpp"
#include "ModifiableReificationDictionary.hpp"
#include "ReificationDictionaryLoader.hpp"


using namespace std;
using namespace hdt;
int main(int argc, char* argv[])
{
	string fileName="../../../testfiles/example1.nt";
	if (argc>=2)
		fileName = string(argv[1]);
	cout << "test file : " << fileName <<endl;

// main (rdf2hdt.cpp)
	RDFNotation notation=NQUAD;
	string baseUri="<http://example.com/>";
	string rdfFormat="nt";
	ProgressListener* listener = NULL;
	IntermediateListener iListener(listener);
	string configFile="";
	HDTSpecification spec(configFile);
	string options="";
	spec.setOptions(options);
	
// ++++ HDTManager::generateHDT (HDTManager.cpp)
// ++++ ++++ BasicHDT::BasicHDT (BasicHDT.cpp)
	Triples* triples;
	Dictionary* dictionary;
	TripleTranslator* trTrans = NULL;
// ++++ ++++ ++++ BasicHDT::createComponents (BasicHDT.cpp)
	Header* header = new PlainHeader();
	triples = new BitmapQuads(spec);
	QueryableReificationDictionary* qrDict = new QueryableReificationDictionary();
	trTrans = qrDict;
	dictionary = qrDict;
	triples->setToGlobalIDFunction(trTrans->getToGlobalIDFunction());
	triples->setToRoleIDFunction(trTrans->getToRoleIDFunction());
// ---- ---- ----
// ---- ----

// ++++ ++++ BasicHDT::loadFromRDF (BasicHDT.cpp)
	try {
		// Make sure that URI starts and ends with <>
		if(baseUri.at(0)!='<')
			baseUri = '<'+baseUri;
		if(baseUri.at(baseUri.length()-1)!='>')
			baseUri.append(">");

		IntermediateListener iListener(listener);

// ++++ ++++ ++++ BasicHDT::loadDictionary (BasicHDT.cpp)

	ModifiableTriplesDictionary* dict = NULL;
	try {
		DictionaryLoader* dictLoader;
		ModifiableReificationDictionary* reif_dict_plain_process = new ModifiableReificationDictionary();
		reif_dict_plain_process->startProcessing();	

		dictLoader = new ReificationDictionaryLoader(reif_dict_plain_process, &iListener);

		RDFParserCallback *parser = RDFParserCallback::getParserCallback(notation);
        	parser->doParse(fileName.c_str(), baseUri.c_str(), notation, true, dictLoader);
		delete parser;

		reif_dict_plain_process->stopProcessing(&iListener);

		// Convert to final format
		if (dictionary->getType()!=reif_dict_plain_process->getType()){
			cout << "import reif_dict_fs from reif_dict_plain" << endl;
			dictionary->import(reif_dict_plain_process);
			//delete reif_dict_plain;
		}
		else{
			throw std::logic_error("Unable to convert dictionary");
			exit(EXIT_FAILURE);
		}
	} catch (...) {
		cout << "Catch exception dictionary"  << endl;
	}


// ---- ---- ----

// ++++ ++++ ++++ BasicHDT::loadTriples (BasicHDT.cpp)


	ModifiableTriples* triplesList = new TriplesQuadsList(spec);
	triplesList->setToGlobalIDFunction(trTrans->getToGlobalIDFunction());
	triplesList->setToRoleIDFunction(trTrans->getToRoleIDFunction());
	try {
		NOTIFY(listener, "Loading Triples", 0, 100);
		iListener.setRange(0, 60);

		triplesList->startProcessing(&iListener);

		TriplesLoader tripLoader(dictionary, triplesList, &iListener);
	


		RDFParserCallback *pars = RDFParserCallback::getParserCallback(notation);
		pars->doParse(fileName.c_str(), baseUri.c_str(), NQUAD, true, &tripLoader);
		delete pars;
		header->insert("_:statistics", HDTVocabulary::ORIGINAL_SIZE, tripLoader.getSize());
		triplesList->stopProcessing(&iListener);

		// SORT & Duplicates
		string ord = "";
		try{
			ord = spec.get("triplesOrder");
		}catch (std::exception& e){}
		TripleComponentOrder order = parseOrder(
				ord.c_str());
		if (order == Unknown) {
			order = SPO;
		}

		iListener.setRange(80, 85);
		triplesList->sort(order, &iListener);

		iListener.setRange(85, 90);
		triplesList->removeDuplicates(&iListener);



		
	} catch (...) {
		cout << "Catch exception triples"  << endl;
	}
	
	if (triples->getType() == triplesList->getType()) {
		delete triples;
		triples = triplesList;
	} else {
		iListener.setRange(90, 100);
		try {
			triples->load(*triplesList, &iListener);
		} catch (...) {
			cerr << "Catch exception thrown by triples->load()"<<endl; 
			delete triplesList;
		}
		delete triplesList;

// ---- ---- ---- 

	}


// ++++ ++++ ++++ BasicHDT::fillHeader (BasicHDT.cpp)
	try{
	string formatNode = "_:format";
	string dictNode = "_:dictionary";
	string triplesNode = "_:triples";
	string statisticsNode = "_:statistics";
	string publicationInfoNode = "_:publicationInformation";

	uint64_t origSize = header->getPropertyLong(statisticsNode.c_str(), HDTVocabulary::ORIGINAL_SIZE.c_str());

	header->clear();

	// BASE
	header->insert(baseUri, HDTVocabulary::RDF_TYPE, HDTVocabulary::HDT_DATASET);

	// VOID
	header->insert(baseUri, HDTVocabulary::RDF_TYPE, HDTVocabulary::VOID_DATASET);
	header->insert(baseUri, HDTVocabulary::VOID_TRIPLES, triples->getNumberOfElements());
	//header->insert(baseUri, HDTVocabulary::VOID_PROPERTIES, dictionary->getNpredicates());
	header->insert(baseUri, HDTVocabulary::VOID_DISTINCT_SUBJECTS, dictionary->getNsubjects());
	header->insert(baseUri, HDTVocabulary::VOID_DISTINCT_OBJECTS, dictionary->getNobjects());
	// TODO: Add more VOID Properties. E.g. void:classes

	// Structure
	header->insert(baseUri, HDTVocabulary::HDT_STATISTICAL_INFORMATION,	statisticsNode);
	header->insert(baseUri, HDTVocabulary::HDT_PUBLICATION_INFORMATION,	publicationInfoNode);
	header->insert(baseUri, HDTVocabulary::HDT_FORMAT_INFORMATION, formatNode);
	header->insert(formatNode, HDTVocabulary::HDT_DICTIONARY, dictNode);
	header->insert(formatNode, HDTVocabulary::HDT_TRIPLES, triplesNode);

	// Dictionary
	dictionary->populateHeader(*header, dictNode);

	// Triples
	triples->populateHeader(*header, triplesNode);

	// Sizes
	header->insert(statisticsNode, HDTVocabulary::ORIGINAL_SIZE, origSize);
	header->insert(statisticsNode, HDTVocabulary::HDT_SIZE, dictionary->size() + triples->size());

	// Current time
	time_t now;
	char date[40];
	time(&now);
	struct tm* today = localtime(&now);
	strftime(date, 40, "%Y-%m-%dT%H:%M:%S%z", today);
	char *tzm = date+strlen(date) - 2;
	memmove(tzm+1, tzm, 3);
	*tzm = ':';
	header->insert(publicationInfoNode, HDTVocabulary::DUBLIN_CORE_ISSUED, date);
	}
	catch(...)
	{cerr << "catch exception thrown in BasicHDT::fillHeader" << endl;}
// ---- ---- ---- 

	}catch (std::exception& e){
	cerr << "catch exception thrown in loadFromHDT" << endl;

	}
// ---- ----
// ----

	

}


