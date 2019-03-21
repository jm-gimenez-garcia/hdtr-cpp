#include <iostream>
#include <exception>
#include "PlainHeader.hpp"
#include "BitmapTriples.hpp"
#include "BitmapQuads.hpp"
#include "PlainTriples.hpp"
#include <HDTVocabulary.hpp>
#include "BasicHDT.hpp"
#include "TripleListDisk.hpp"
#include "TriplesList.hpp"
#include "TriplesQuadsList.hpp"
#include "TriplesLoader.hpp"
#include "DictionaryLoader.hpp"
#include "ReificationDictionaryLoader.hpp"
#include "QueryableReificationDictionary.hpp"
#include "ModifiableReificationDictionary.hpp"
#include <HDTManager.hpp>


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
	//spec.set("triples.type",HDTVocabulary::TRIPLES_TYPE_BITMAP);
	
	HDT *hdt = HDTManager::generateHDT(fileName.c_str(), baseUri.c_str(), notation, spec, listener);

	

}


