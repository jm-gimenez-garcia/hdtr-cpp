#include <iostream>
#include <exception>
#include "ControlInformation.hpp"
#include "TriplesPlainDictionary.hpp"
#include "GraphsPlainDictionary.hpp"
#include "TriplesKyotoDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include "TriplesLiteralDictionary.hpp"
#include "ModifiableReificationDictionary.hpp"
#include "QueryableReificationDictionary.hpp"
#include "DictionaryLoader.hpp"
#include "Dictionary.hpp"
#include "HDTListener.hpp"
#include "RDFParser.hpp"
#include "HDTVocabulary.hpp"
using namespace std;
using namespace hdt;

int main(int argc, char* argv[])
{
	string filename="../../testfiles/example1.nt";
	if (argc>=2)
		filename = string(argv[1]);
	cout << "test file : " << filename <<endl;

try
{
	// TriplesPlainDictionary object - ModifiableDictionary pointer
	IntermediateListener iListener(NULL);
	//ControlInformation controlInfo;
	ModifiableReificationDictionary* reif_dict_plain_process = new ModifiableReificationDictionary();// TriplesPlainDictionary(); 
	//ModifiableDictionary* plain_dict = new TriplesKyotoDictionary(); 
	//Dictionary* fs_dict = new TriplesFourSectionDictionary();
	QueryableReificationDictionary* reif_dict_fs_init = new QueryableReificationDictionary();
	reif_dict_plain_process->startProcessing();	
	RDFNotation notation=NQUAD;
	try {
		iListener.setRange(0, 80);
		// Load data
		DictionaryLoader dictLoader(reif_dict_plain_process, &iListener);

		RDFParserCallback *parser = RDFParserCallback::getParserCallback(notation);
        	parser->doParse(filename.c_str(), "http://example.com/", notation, true, &dictLoader);
		delete parser;


		iListener.setRange(80, 90);
		reif_dict_plain_process->stopProcessing(&iListener);

		// Convert to final format
		if (reif_dict_fs_init->getType()!=reif_dict_plain_process->getType()){
			cout << "import reif_dict_fs from reif_dict_plain" << endl;
			reif_dict_fs_init->import(reif_dict_plain_process);
			//delete reif_dict_plain;
		}
		else{
			throw std::logic_error("Unable to convert dictionary");
			exit(EXIT_FAILURE);
		}
	} catch(exception& e) {
		cerr << "caught here??" << endl;
		delete reif_dict_plain_process;
		throw;
}

	const ModifiableReificationDictionary* reif_dict_plain = reif_dict_plain_process;

	//reif_dict_plain->load("../../testfiles/example1.nt");
	cout << "---------------------TESTS OF MODIFIABLE DICTIONARY METHODS-------------------------------------"<<endl;
	cout << "reif_dict_plain->getNshared() = " << reif_dict_plain->getNshared() << endl;
	cout << "reif_dict_plain->getNsubjects() = " << reif_dict_plain->getNsubjects() << endl;
	cout << "reif_dict_plain->getNobjects() = " << reif_dict_plain->getNobjects() << endl << endl;
	cout << "reif_dict_plain->getNpredicates() = " << reif_dict_plain->getNpredicates() << endl << endl;
	cout << "reif_dict_plain->getNgraphs() = " << reif_dict_plain->getNgraphs() << endl << endl;
	cout << "reif_dict_plain->getNunused() = " << reif_dict_plain->getNunused() << endl << endl;

	cout << "maximum GLOBAL Id :     reif_dict_plain->getMaxID() = " << reif_dict_plain->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : reif_dict_plain->getMaxSubjectID() = " << reif_dict_plain->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  reif_dict_plain->getMaxObjectID() = " << reif_dict_plain->getMaxObjectID() << endl << endl;
	cout << "maximum GLOBAL predjId :  reif_dict_plain->getMaxPredicateID() = " << reif_dict_plain->getMaxPredicateID() << endl << endl;
	cout << "maximum GLOBAL graphId :  reif_dict_plain->getMaxGraphID() = " << reif_dict_plain->getMaxGraphID() << endl << endl;







	cout << "reif_dict_plain->getNshared() = " <<  reif_dict_plain->getNshared() << endl;
	cout << "reif_dict_plain->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local shared in Triples Dictionary ------------"<<endl;
	cout << "getNshared() = " <<  reif_dict_plain->getNshared() << endl;
	cout << "getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getGraphsDictionaryPtr()->getNshared() << endl;
	for (int locid=0 ; locid < reif_dict_plain->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\treif_dict_plain->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\treif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\treif_dict_plain->getGlobalId(" << locid << ", SHARED_OBJECT) = " << reif_dict_plain->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\treif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_plain->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local shared in Graphs Dictionary ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\treif_dict_plain->getGlobalId(" << locid << ", SHARED_SUBJECT_GRAPH) = " << reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT_GRAPH) << endl;
		cout << "\treif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_SUBJECT_GRAPH), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT_GRAPH), SUBJECT) << endl;
		cout << "\treif_dict_plain->getGlobalId(" << locid << ", SHARED_OBJECT_GRAPH) = " << reif_dict_plain->getGlobalId(locid, SHARED_OBJECT_GRAPH) << endl;
		cout << "\reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_OBJECT_GRAPH), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_OBJECT_GRAPH), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_plain->getNshared() << endl;
	cout << "getNsubjects()=" <<  reif_dict_plain->getNsubjects() << endl;
	cout << "getMaxSubjectID()=" <<  reif_dict_plain->getMaxSubjectID() << endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxSubjectID() ; globid++)
	{
		cout << "globid=" << globid << endl;
		cout << "\tplain_dict->idToString(" << globid << ", SUBJECT) = " << reif_dict_plain->idToString(globid, SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", SUBJECT) = " << reif_dict_plain->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "reif_dict_plain->getNsubjects() - reif_dict_plain->getNshared() = " <<  reif_dict_plain->getNsubjects() - reif_dict_plain->getNshared() << endl;
	cout << "reif_dict_plain->getTriplesDictionaryPtr()->getNsubjects() - reif_dict_plain->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getTriplesDictionaryPtr()->getNsubjects() - reif_dict_plain->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local subjects in Triples Dictionary ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getTriplesDictionaryPtr()->getNsubjects()-reif_dict_plain->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_plain->getGraphsDictionaryPtr()->getNsubjects() - reif_dict_plain->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getGraphsDictionaryPtr()->getNsubjects() - reif_dict_plain->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local subjects in Graphs Dictionary ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getGraphsDictionaryPtr()->getNsubjects()-reif_dict_plain->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT_GRAPH) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT_GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT_GRAPH), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT_GRAPH), SUBJECT) << endl;
	}
	cout << endl;




	cout << "\t------------ global objects ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_plain->getNshared() << endl;
	cout << "getNobjects()=" <<  reif_dict_plain->getNobjects() << endl;
	cout << "getMaxObjectID()=" <<  reif_dict_plain->getMaxObjectID() << endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxObjectID() ; globid++)
	{
		cout << "globid=" << globid << endl;
		if(globid>reif_dict_plain->getNshared() && globid <= reif_dict_plain->getMaxSubjectID())
			continue;
		cout << "\tplain_dict->idToString(" << globid << ", OBJECT) = " << reif_dict_plain->idToString(globid, OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", OBJECT) = " << reif_dict_plain->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "reif_dict_plain->getNobjects() - reif_dict_plain->getNshared() = " <<  reif_dict_plain->getNobjects() - reif_dict_plain->getNshared() << endl;
	cout << "reif_dict_plain->getTriplesDictionaryPtr()->getNobjects() - reif_dict_plain->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getTriplesDictionaryPtr()->getNobjects() - reif_dict_plain->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local objects in Triples Dictionary ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getTriplesDictionaryPtr()->getNobjects()-reif_dict_plain->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_plain->getGraphsDictionaryPtr()->getNobjects() - reif_dict_plain->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_plain->getGraphsDictionaryPtr()->getNobjects() - reif_dict_plain->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local objects in Graphs Dictionary ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getGraphsDictionaryPtr()->getNobjects()-reif_dict_plain->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT_GRAPH) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT_GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_OBJECT_GRAPH), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT_GRAPH), OBJECT) << endl;
	}
	cout << endl;



		cout << "reif_dict_plain->getMaxPredicateID()=" << reif_dict_plain->getMaxPredicateID()<<endl;
	cout << "\t------------ global predicates ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxPredicateID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\treif_dict_plain->idToString(" << globid << ", PREDICATE) = " << reif_dict_plain->idToString(globid, PREDICATE) << endl;
		cout << "\treif_dict_plain->getLocalId(" << globid << ", PREDICATE) = " << reif_dict_plain->getLocalId(globid, PREDICATE) << endl;
	}
	cout << endl;

	cout << "\t------------ local predicates ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getMaxPredicateID() ; locid++)
	{
		cout << "\treif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_PREDICATE) << endl;
		cout << "\treif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE), PREDICATE) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_PREDICATE), PREDICATE) << endl;
	}
	cout << endl;
	cout << endl;



		cout << "reif_dict_plain->getMaxGraphID()=" << reif_dict_plain->getMaxGraphID()<<endl;
		cout << "reif_dict_plain->getNunused()=" << reif_dict_plain->getNunused()<<endl;
	cout << "\t------------ global ununsed graphs ------------"<<endl;
	for (int globid=reif_dict_plain->getMaxObjectID()+1 ; globid <= reif_dict_plain->getMaxGraphID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\tplain_dict->idToString(" << globid << ", GRAPH) = " << reif_dict_plain->idToString(globid, GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", GRAPH) = " << reif_dict_plain->getLocalId(globid, GRAPH) << endl;
	}
	cout << endl;

	cout << "\t------------ local unused graphs ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getNunused() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << reif_dict_plain->getGlobalId(locid, UNUSED_GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
	}
	cout << endl;
	cout << endl;

	const QueryableReificationDictionary* reif_dict_fs = reif_dict_fs_init;

	//reif_dict_fs->load("../../testfiles/example1.nt");
	cout << "---------------------TESTS OF DICTIONARY METHODS-------------------------------------"<<endl;
	cout << "reif_dict_fs->getNshared() = " << reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getNsubjects() = " << reif_dict_fs->getNsubjects() << endl;
	cout << "reif_dict_fs->getNobjects() = " << reif_dict_fs->getNobjects() << endl << endl;
	cout << "reif_dict_fs->getNpredicates() = " << reif_dict_fs->getNpredicates() << endl << endl;
	cout << "reif_dict_fs->getNgraphs() = " << reif_dict_fs->getNgraphs() << endl << endl;
	cout << "reif_dict_fs->getNunused() = " << reif_dict_fs->getNunused() << endl << endl;

	cout << "maximum GLOBAL Id :     reif_dict_fs->getMaxID() = " << reif_dict_fs->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : reif_dict_fs->getMaxSubjectID() = " << reif_dict_fs->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  reif_dict_fs->getMaxObjectID() = " << reif_dict_fs->getMaxObjectID() << endl << endl;
	cout << "maximum GLOBAL predjId :  reif_dict_fs->getMaxPredicateID() = " << reif_dict_fs->getMaxPredicateID() << endl << endl;
	cout << "maximum GLOBAL graphId :  reif_dict_fs->getMaxGraphID() = " << reif_dict_fs->getMaxGraphID() << endl << endl;







	cout << "reif_dict_fs->getNshared() = " <<  reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local shared in Triples Dictionary ------------"<<endl;
	cout << "getNshared() = " <<  reif_dict_fs->getNshared() << endl;
	cout << "getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getGraphsDictionaryPtr()->getNshared() << endl;
	for (int locid=1 ; locid <= reif_dict_fs->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\treif_dict_fs->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\treif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\treif_dict_fs->getGlobalId(" << locid << ", SHARED_OBJECT) = " << reif_dict_fs->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\treif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_fs->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local shared in Graphs Dictionary ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\treif_dict_fs->getGlobalId(" << locid << ", SHARED_SUBJECT_GRAPH) = " << reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT_GRAPH) << endl;
		cout << "\treif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_SUBJECT_GRAPH), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT_GRAPH), SUBJECT) << endl;
		cout << "\treif_dict_fs->getGlobalId(" << locid << ", SHARED_OBJECT_GRAPH) = " << reif_dict_fs->getGlobalId(locid, SHARED_OBJECT_GRAPH) << endl;
		cout << "\reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_OBJECT_GRAPH), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_OBJECT_GRAPH), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_fs->getNshared() << endl;
	cout << "getNsubjects()=" <<  reif_dict_fs->getNsubjects() << endl;
	cout << "getMaxSubjectID()=" <<  reif_dict_fs->getMaxSubjectID() << endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxSubjectID() ; globid++)
	{
		cout << "globid=" << globid << endl;
		cout << "\tfs_dict->idToString(" << globid << ", SUBJECT) = " << reif_dict_fs->idToString(globid, SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", SUBJECT) = " << reif_dict_fs->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "reif_dict_fs->getNsubjects() - reif_dict_fs->getNshared() = " <<  reif_dict_fs->getNsubjects() - reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getTriplesDictionaryPtr()->getNsubjects() - reif_dict_fs->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getTriplesDictionaryPtr()->getNsubjects() - reif_dict_fs->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local subjects in Triples Dictionary ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getTriplesDictionaryPtr()->getNsubjects()-reif_dict_fs->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_fs->getGraphsDictionaryPtr()->getNsubjects() - reif_dict_fs->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getGraphsDictionaryPtr()->getNsubjects() - reif_dict_fs->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local subjects in Graphs Dictionary ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getGraphsDictionaryPtr()->getNsubjects()-reif_dict_fs->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT_GRAPH) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT_GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT_GRAPH), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT_GRAPH), SUBJECT) << endl;
	}
	cout << endl;




	cout << "\t------------ global objects ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_fs->getNshared() << endl;
	cout << "getNobjects()=" <<  reif_dict_fs->getNobjects() << endl;
	cout << "getMaxObjectID()=" <<  reif_dict_fs->getMaxObjectID() << endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxObjectID() ; globid++)
	{
		cout << "globid=" << globid << endl;
		if(globid>reif_dict_fs->getNshared() && globid <= reif_dict_fs->getMaxSubjectID())
			continue;
		cout << "\tfs_dict->idToString(" << globid << ", OBJECT) = " << reif_dict_fs->idToString(globid, OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", OBJECT) = " << reif_dict_fs->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "reif_dict_fs->getNshared() = " <<  reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getNobjects() - reif_dict_fs->getNshared() = " <<  reif_dict_fs->getNobjects() - reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getTriplesDictionaryPtr()->getNobjects() - reif_dict_fs->getTriplesDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getTriplesDictionaryPtr()->getNobjects() - reif_dict_fs->getTriplesDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local objects in Triples Dictionary ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getTriplesDictionaryPtr()->getNobjects()-reif_dict_fs->getTriplesDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << "reif_dict_fs->getGraphsDictionaryPtr()->getNobjects() - reif_dict_fs->getGraphsDictionaryPtr()->getNshared() = " <<  reif_dict_fs->getGraphsDictionaryPtr()->getNobjects() - reif_dict_fs->getGraphsDictionaryPtr()->getNshared() << endl;
	cout << "\t------------ local objects in Graphs Dictionary ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getGraphsDictionaryPtr()->getNobjects()-reif_dict_fs->getGraphsDictionaryPtr()->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT_GRAPH) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT_GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_OBJECT_GRAPH), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT_GRAPH), OBJECT) << endl;
	}
	cout << endl;



		cout << "reif_dict_fs->getMaxPredicateID()=" << reif_dict_fs->getMaxPredicateID()<<endl;
	cout << "\t------------ global predicates ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxPredicateID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\treif_dict_fs->idToString(" << globid << ", PREDICATE) = " << reif_dict_fs->idToString(globid, PREDICATE) << endl;
		cout << "\treif_dict_fs->getLocalId(" << globid << ", PREDICATE) = " << reif_dict_fs->getLocalId(globid, PREDICATE) << endl;
	}
	cout << endl;

	cout << "\t------------ local predicates ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getMaxPredicateID() ; locid++)
	{
		cout << "\treif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_PREDICATE) << endl;
		cout << "\treif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE), PREDICATE) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_PREDICATE), PREDICATE) << endl;
	}
	cout << endl;
	cout << endl;



		cout << "reif_dict_fs->getMaxGraphID()=" << reif_dict_fs->getMaxGraphID()<<endl;
		cout << "reif_dict_fs->getNunused()=" << reif_dict_fs->getNunused()<<endl;
	cout << "\t------------ global unused graphs ------------"<<endl;
	for (int globid=reif_dict_fs->getMaxObjectID()+1 ; globid <= reif_dict_fs->getMaxGraphID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\tfs_dict->idToString(" << globid << ", GRAPH) = " << reif_dict_fs->idToString(globid, GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", GRAPH) = " << reif_dict_fs->getLocalId(globid, GRAPH) << endl;
	}
	cout << endl;

	cout << "\t------------ local unused graphs ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getNunused() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << reif_dict_fs->getGlobalId(locid, UNUSED_GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
	}
	cout << endl;
	cout << endl;



}
catch (exception& e)
{
	cout << e.what() << '\n';
}

}


