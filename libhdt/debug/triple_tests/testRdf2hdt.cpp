#include <iostream>
#include <string>
#include <exception>
#include <fstream>

#include "HDTEnums.hpp"
#include "HDTSpecification.hpp"
#include "StopWatch.hpp"
#include "HDTListener.hpp"
#include "HDTManager.hpp"


using namespace std;
using namespace hdt;
int main(int argc, char* argv[])
{
	string inputFile="../../../testfiles/example_graphs_1+2+3+4+5.nq";
	if (argc>=2)
		inputFile = string(argv[1]);
	cout << "test file : " << inputFile <<endl;

	string outputFile = inputFile + ".hdt";
	RDFNotation notation=NQUAD;
	bool verbose=false;
	bool showProgress=false;
	bool generateIndex=false;
	string baseUri="<http://example.com/>";
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
		HDT *hdt = HDTManager::generateHDT(inputFile.c_str(), baseUri.c_str(), notation, spec, progress);

		ofstream out;

		// Save HDT
		hdt->saveToHDT(outputFile.c_str(), progress);

		globalTimer.stop();
#define vout if (!verbose) {} else std::cerr /* Verbose output */
		vout << "HDT Successfully generated." << endl;
		vout << "Total processing time: ";
		vout << "Clock(" << globalTimer.getRealStr();
		vout << ")  User(" << globalTimer.getUserStr();
		vout << ")  System(" << globalTimer.getSystemStr() << ")" << endl;
#undef vout

		if(generateIndex) {
			hdt = HDTManager::indexedHDT(hdt, progress);
		}

		delete hdt;
		delete progress;
	} catch (exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}



}


