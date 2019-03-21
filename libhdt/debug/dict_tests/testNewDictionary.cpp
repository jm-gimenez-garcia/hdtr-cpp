#include <iostream>
#include <exception>
#include <ControlInformation.hpp>
#include "TriplesPlainDictionary.hpp"
#include "GraphsPlainDictionary.hpp"
#include "TriplesKyotoDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include "TriplesLiteralDictionary.hpp"
#include "DictionaryLoader.hpp"
#include <Dictionary.hpp>
#include <HDTListener.hpp>
#include <RDFParser.hpp>
#include <HDTVocabulary.hpp>
using namespace std;
using namespace hdt;
int main(int argc, char* argv[])
{
	string filename="../../../testfiles/example1.nt";
	if (argc>=2)
		filename = string(argv[1]);
	cout << "test file : " << filename <<endl;

try
{
	// TriplesPlainDictionary object - ModifiableDictionary pointer
	ProgressListener* listener = NULL;
	IntermediateListener iListener(listener);
	//ControlInformation controlInfo;
	ModifiableDictionary* plain_dict = new TriplesPlainDictionary(); 
	//ModifiableDictionary* plain_dict = new TriplesKyotoDictionary(); 
	//Dictionary* fs_dict = new TriplesFourSectionDictionary();
	Dictionary* fs_dict = new TriplesLiteralDictionary();
	plain_dict->startProcessing();	

	RDFNotation notation=NTRIPLES;
	try {
		iListener.setRange(0, 80);
		// Load data
		DictionaryLoader dictLoader(plain_dict, &iListener);

		RDFParserCallback *parser = RDFParserCallback::getParserCallback(notation);
        	parser->doParse(filename.c_str(), "http://example.com/", notation, true, &dictLoader);
		delete parser;

		iListener.setRange(80, 90);
		plain_dict->stopProcessing(&iListener);

		// Convert to final format
		if (fs_dict->getType()!=HDTVocabulary::DICTIONARY_TYPE_PLAIN){
			fs_dict->import(plain_dict);
			//delete plain_dict;
		}
		else{
			fs_dict = plain_dict;
		}
	} catch(exception& e) {
		cerr << "caught here??" << endl;
		delete plain_dict;
		throw;
}



	//plain_dict->load("../../testfiles/example1.nt");
	cout << "---------------------TESTS OF MODIFIABLE DICTIONARY METHODS-------------------------------------"<<endl;
	cout << "plain_dict->getNshared() = " << plain_dict->getNshared() << endl;
	cout << "plain_dict->getNsubjects() = " << plain_dict->getNsubjects() << endl;
	cout << "plain_dict->getNobjects() = " << plain_dict->getNobjects() << endl << endl;

	cout << "maximum GLOBAL Id :     plain_dict->getMaxID() = " << plain_dict->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : plain_dict->getMaxSubjectID() = " << plain_dict->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  plain_dict->getMaxObjectID() = " << plain_dict->getMaxObjectID() << endl << endl;

	cout << "\t------------ local shared ------------"<<endl;
	for (int locid=0 ; locid < plain_dict->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << plain_dict->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(plain_dict->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << plain_dict->getLocalId(plain_dict->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\tplain_dict->getGlobalId(" << locid << ", SHARED_OBJECT) = " << plain_dict->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(plain_dict->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << plain_dict->getLocalId(plain_dict->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	for (int globid=1 ; globid <= plain_dict->getMaxSubjectID() ; globid++)
	{
		cout << "\tplain_dict->idToString(" << globid << ", SUBJECT) = " << plain_dict->idToString(globid, SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", SUBJECT) = " << plain_dict->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local subjects ------------"<<endl;
	for (int locid=0 ; locid < plain_dict->getMaxSubjectID()-plain_dict->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << plain_dict->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(plain_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << plain_dict->getLocalId(plain_dict->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ global objects ------------"<<endl;
	for (int globid=1 ; globid <= plain_dict->getMaxObjectID() ; globid++)
	{
		cout << "\tplain_dict->idToString(" << globid << ", OBJECT) = " << plain_dict->idToString(globid, OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", OBJECT) = " << plain_dict->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local objects ------------"<<endl;
	for (int locid=0 ; locid < plain_dict->getMaxObjectID()-plain_dict->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << plain_dict->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(plain_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << plain_dict->getLocalId(plain_dict->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << endl;




	cout << "---------------------TESTS OF DICTIONARY METHODS-------------------------------------"<<endl;

	cout << "fs_dict->getNshared() = " << fs_dict->getNshared() << endl;
	cout << "fs_dict->getNsubjects() = " << fs_dict->getNsubjects() << endl;
	cout << "fs_dict->getNobjects() = " << fs_dict->getNobjects() << endl << endl;

	cout << "maximum GLOBAL Id :     fs_dict->getMaxID() = " << fs_dict->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : fs_dict->getMaxSubjectID() = " << fs_dict->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  fs_dict->getMaxObjectID() = " << fs_dict->getMaxObjectID() << endl << endl;


	cout << "\t------------ local shared ------------"<<endl;
	for (int locid=1 ; locid <= fs_dict->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << fs_dict->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(fs_dict->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << fs_dict->getLocalId(fs_dict->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\tfs_dict->getGlobalId(" << locid << ", SHARED_OBJECT) = " << fs_dict->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(fs_dict->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << fs_dict->getLocalId(fs_dict->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	for (int globid=1 ; globid <= fs_dict->getMaxSubjectID() ; globid++)
	{
		cout << "\tfs_dict->idToString(" << globid << ", SUBJECT) = " << fs_dict->idToString(globid, SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", SUBJECT) = " << fs_dict->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local subjects ------------"<<endl;
	for (int locid=1 ; locid <= fs_dict->getMaxSubjectID()-fs_dict->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << fs_dict->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(fs_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << fs_dict->getLocalId(fs_dict->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ global objects ------------"<<endl;
	for (int globid=1 ; globid <= fs_dict->getMaxObjectID() ; globid++)
	{
		cout << "\tfs_dict->idToString(" << globid << ", OBJECT) = " << fs_dict->idToString(globid, OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", OBJECT) = " << fs_dict->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local objects ------------"<<endl;
	for (int locid=1 ; locid <= fs_dict->getMaxObjectID()-fs_dict->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << fs_dict->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(fs_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << fs_dict->getLocalId(fs_dict->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << endl;




    if(TriplesDictionary* tfs_dict = dynamic_cast<TriplesDictionary*>(fs_dict))
    {
		cout << "---------------------TESTS OF TRIPLES DICTIONARY METHODS-------------------------------------" << endl;
		cout << "\t------------ global predicates ------------"<<endl;
		for (int globid=1 ; globid <= tfs_dict->getMaxPredicateID() ; globid++)
		{
			cout << "\ttfs_dict->idToString(" << globid << ", PREDICATE) = " << tfs_dict->idToString(globid, PREDICATE) << endl;
			cout << "\ttfs_dict->getLocalId(" << globid << ", PREDICATE) = " << tfs_dict->getLocalId(globid, PREDICATE) << endl;
		}
		cout << endl;
	
		cout << "\t------------ local predicates ------------"<<endl;
		for (int locid=1 ; locid <= tfs_dict->getMaxPredicateID() ; locid++)
		{
			cout << "\ttfs_dict->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE) = " << tfs_dict->getGlobalId(locid, NOT_SHARED_PREDICATE) << endl;
			cout << "\ttfs_dict->getLocalId(tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE), PREDICATE) = " << tfs_dict->getLocalId(tfs_dict->getGlobalId(locid, NOT_SHARED_PREDICATE), PREDICATE) << endl;
		}
		cout << endl;
	

    }
	else if(GraphsDictionary* gfs_dict = dynamic_cast<GraphsDictionary*>(fs_dict))
	{	
		cout << "---------------------TESTS OF GRAPHS DICTIONARY METHODS-------------------------------------" << endl;
		cout << "\t------------ global graphs ------------"<<endl;
		for (int globid=1 ; globid <= gfs_dict->getMaxGraphID() ; globid++)
		{
			cout << "\tgfs_dict->idToString(" << globid << ", GRAPH) = " << gfs_dict->idToString(globid, GRAPH) << endl;
			cout << "\tgfs_dict->getLocalId(" << globid << ", GRAPH) = " << gfs_dict->getLocalId(globid, GRAPH) << endl;
		}
		cout << endl;
	
		unsigned int max_subj_obj = (gfs_dict->getNsubjects() > gfs_dict->getNobjects()) ? gfs_dict->getNsubjects() : gfs_dict->getNobjects();
		cout << "\t------------ local graphs ------------"<<endl;
		for (int locid=1 ; locid <= gfs_dict->getMaxGraphID()- gfs_dict->getNshared()-max_subj_obj ; locid++)
		{
			cout << "\tgfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << gfs_dict->getGlobalId(locid, UNUSED_GRAPH) << endl;
			cout << "\tgfs_dict->getLocalId(gfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << gfs_dict->getLocalId(gfs_dict->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
		}
		cout << endl;
	}


delete fs_dict;
delete plain_dict;




}
catch (exception& e)
{
	cout << e.what() << '\n';
}

}


