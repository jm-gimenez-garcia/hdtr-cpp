#include <iostream>
#include <string>
#include <exception>

#include "HDTEnums.hpp"
#include "HDTSpecification.hpp"
#include "StopWatch.hpp"
#include "HDTListener.hpp"
#include "HDTManager.hpp"
#include "RDFSerializer.hpp"

using namespace std;
using namespace hdt;
int main(int argc, char* argv[])
{
	string inputFile="../../../testfiles/example_graphs_1+2+3+4+5.nq.hdt";
	if (argc>=2)
		inputFile = string(argv[1]);
	cout << "test file : " << inputFile <<endl;

	string outputFile = inputFile + ".rdf";
	RDFNotation notation=NQUAD;
	bool verbose=false;
	bool showProgress=false;
	string rdfFormat="nq";
	string configFile="";
	HDTSpecification spec(configFile);
	string options="";
	spec.setOptions(options);
	//spec.set("triples.type",HDTVocabulary::TRIPLES_TYPE_BITMAP);
	

	try {
		// Read RDF
		StopWatch globalTimer;

		ProgressListener* progress = showProgress ? new StdoutProgressListener() : NULL;
		HDT *hdt = HDTManager::mapHDT(inputFile.c_str(), progress);

		RDFSerializer *serializer = RDFSerializer::getSerializer(outputFile.c_str(), notation);
		hdt->saveToRDF(*serializer);
		delete serializer;

		delete hdt;
		delete progress;
	} catch (exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}



}


