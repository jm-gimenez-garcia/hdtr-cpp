#include <iostream>
#include <string>
#include <exception>

#include "HDTEnums.hpp"
#include "HDTSpecification.hpp"
#include "StopWatch.hpp"
#include "HDTListener.hpp"
#include "HDTManager.hpp"
#include "RDFSerializer.hpp"
#include <ctime>

using namespace std;
using namespace hdt;
int main(int argc, char* argv[])
{
cout << __FILE__ << ":" << __LINE__ <<  endl;
	string inputFile="../../../testfiles/example_graphs_1+2+3+4+5.nq.hdt";
	unsigned int NB_TESTS = 100;
	if (argc>=2)
		NB_TESTS = atoi(argv[1]);
	cout << "test file : " << inputFile <<endl;

	srand(time(nullptr));

	string outputFile;
	RDFNotation notation=NQUAD;
	bool verbose=false;
	bool showProgress=false;
	string rdfFormat="nq";
	string configFile="";
	HDTSpecification spec(configFile);
	string options="";
	spec.setOptions(options);
	//spec.set("triples.type",HDTVocabulary::TRIPLES_TYPE_BITMAP);
		
	std::vector<string> subjects   = {"D1","D3","O2","G4","G5","G9","A1","A2","A3","C1","C3","G13","G6","G7","G8"};
	std::vector<string> predicates = {"link1","link2","link3","link4","link5","link6"};
	std::vector<string> objects    = {"D1","D3","O2","G4","G5","G9","B1","B2","B3","C2","O1","S1","G2","G3"};
	std::vector<string> graphs     = {"G4","G5","G9","G13","G6","G7","G8","G2","G3","G1","G10","G11","G12"};

	TripleString* ts = NULL;
	try {
		// Read RDF
		StopWatch globalTimer;

		ProgressListener* progress = showProgress ? new StdoutProgressListener() : NULL;
		HDT *hdt = HDTManager::mapHDT(inputFile.c_str(), progress);

		for (unsigned int i = 0 ; i<NB_TESTS ; i++)
		{
			outputFile = inputFile + ".rdf";

			string subj, pred, obj, gr;
			if((std::rand()%2)==0){
				subj="";
				outputFile += "_0";
			}else{
				string subj_end = subjects[std::rand()%(subjects.size())];
				subj="http://example.org/" + subj_end;
				outputFile += "_" + subj_end;
			}

			if((std::rand()%2)==0){
				pred="";
				outputFile += "_0";
			}else{
				string pred_end = predicates[std::rand()%(predicates.size())];
				pred="http://example.org/" + pred_end;
				outputFile += "_" + pred_end;
			}

			if((std::rand()%2)==0){
				obj="";
				outputFile += "_0";
			}else{
				string obj_end = objects[std::rand()%(objects.size())];
				obj="http://example.org/" + obj_end;
				outputFile += "_" + obj_end;
			}


			const int random_gr_type = std::rand()%3;
			if(random_gr_type==0){
				string gr_end = graphs[std::rand()%(graphs.size())]; 
				gr = "http://example.org/" + gr_end ;
				ts = new QuadString(subj.c_str(), pred.c_str(), obj.c_str(), gr.c_str());
				outputFile += "_" + gr_end;
			}else if (random_gr_type==1){
				ts = new QuadString(subj.c_str(), pred.c_str(), obj.c_str(), "");
				outputFile += "_0";
			}else{
				ts = new TripleString(subj.c_str(), pred.c_str(), obj.c_str());
			}


			RDFSerializer *serializer = RDFSerializer::getSerializer(outputFile.c_str(), notation);
			hdt->saveToRDF(*serializer, NULL, ts);
			delete ts; ts=NULL;
			delete serializer;
		}



		delete hdt;
		delete progress;
	} catch (exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}



}


