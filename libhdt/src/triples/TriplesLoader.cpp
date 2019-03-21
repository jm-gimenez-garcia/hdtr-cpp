#include "TriplesLoader.hpp"
#include <Dictionary.hpp>
#include <Triples.hpp>
#include <SingleTriple.hpp>
#include <SingleQuad.hpp>
#include <HDTListener.hpp>

using namespace hdt;


void TriplesLoader::processTriple(const hdt::TripleString& triple, unsigned long long pos) {
	TripleID ti;
	dictionary->tripleStringtoTripleID(&triple, &ti);
	if (ti.isValid()) {
		triples->insert(ti);
	} else {
		stringstream msg;
		msg << "ERROR: Could not convert triple to IDS! " << endl << triple << endl << ti;
		throw ParseException(msg.str());
	}
	//cerr << "TripleID: " << ti << endl;
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Triples: %lld K triples processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
	if(pos>sizeBytes) {
		sizeBytes = pos;
	}
}


void TriplesLoader::processQuad(const hdt::QuadString& quad, unsigned long long pos) {
	QuadID qi;
	dictionary->quadStringtoQuadID(&quad, &qi);
	if (qi.isValid()) {
		triples->insert(qi);
	} else {
		stringstream msg;
		msg << "ERROR: Could not convert quad to IDS! " << endl << quad << endl << qi;
		throw ParseException(msg.str());
	}
	//cerr << "QuadID: " << qi << endl;
	char str[100];
	if ((listener != NULL) && (count % 100000) == 0) {
		sprintf(str, "Generating Triples: %lld K triples processed.", count / 1000);
		listener->notifyProgress(0, str);
	}
	count++;
	if(pos>sizeBytes) {
		sizeBytes = pos;
	}
}


