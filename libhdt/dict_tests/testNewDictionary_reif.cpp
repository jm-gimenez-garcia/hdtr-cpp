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
	ModifiableReificationDictionary* reif_dict_plain = new ModifiableReificationDictionary();// TriplesPlainDictionary(); 
	//ModifiableDictionary* plain_dict = new TriplesKyotoDictionary(); 
	//Dictionary* fs_dict = new TriplesFourSectionDictionary();
	BaseReificationDictionary* reif_dict_fs = new QueryableReificationDictionary();
	reif_dict_plain->startProcessing();	
	RDFNotation notation=NQUAD;
	try {
		iListener.setRange(0, 80);
		// Load data
		DictionaryLoader dictLoader(reif_dict_plain, &iListener);

		RDFParserCallback *parser = RDFParserCallback::getParserCallback(notation);
        	parser->doParse(filename.c_str(), "http://example.com/", notation, true, &dictLoader);
		delete parser;


		iListener.setRange(80, 90);
		reif_dict_plain->stopProcessing(&iListener);

		// Convert to final format
		if (reif_dict_fs->getType()!=reif_dict_plain->getType()){
			cout << "import reif_dict_fs from reif_dict_plain" << endl;
			reif_dict_fs->import(reif_dict_plain);
			//delete reif_dict_plain;
		}
		else{
			reif_dict_fs = reif_dict_plain;
		}
	} catch(exception& e) {
		cerr << "caught here??" << endl;
		delete reif_dict_plain;
		throw;
}



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







	cout << "\t------------ local shared ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_plain->getNshared() << endl;
	for (int locid=0 ; locid < reif_dict_plain->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\tplain_dict->getGlobalId(" << locid << ", SHARED_OBJECT) = " << reif_dict_plain->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	cout << "getNshared()=" <<  reif_dict_plain->getNshared() << endl;
	cout << "getNsubjects()=" <<  reif_dict_plain->getNsubjects() << endl;
	cout << "getMaxSubjectID()=" <<  reif_dict_plain->getMaxSubjectID() << endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxSubjectID() ; globid++)
	{
		cout << "\tplain_dict->idToString(" << globid << ", SUBJECT) = " << reif_dict_plain->idToString(globid, SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", SUBJECT) = " << reif_dict_plain->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local subjects ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getMaxSubjectID()-reif_dict_plain->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;

		cout << "reif_dict_plain->getMaxObjectID()=" << reif_dict_plain->getMaxObjectID()<<endl;
		cout << "reif_dict_plain->getNshared()=" << reif_dict_plain->getNshared() << endl;
	cout << "\t------------ global objects ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxObjectID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		if (globid > reif_dict_plain->getNshared() && globid <=reif_dict_plain->getMaxSubjectID())
		{
			continue;
		}

		cout << "\tplain_dict->idToString(" << globid << ", OBJECT) = " << reif_dict_plain->idToString(globid, OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", OBJECT) = " << reif_dict_plain->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local objects ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getMaxObjectID()-reif_dict_plain->getNsubjects() + reif_dict_plain->getNshared() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << endl;




		cout << "reif_dict_plain->getMaxPredicateID()=" << reif_dict_plain->getMaxPredicateID()<<endl;
	cout << "\t------------ global predicates ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_plain->getMaxPredicateID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\tplain_dict->idToString(" << globid << ", PREDICATE) = " << reif_dict_plain->idToString(globid, PREDICATE) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", PREDICATE) = " << reif_dict_plain->getLocalId(globid, PREDICATE) << endl;
	}
	cout << endl;

	cout << "\t------------ local predicates ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getMaxPredicateID() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE) = " << reif_dict_plain->getGlobalId(locid, NOT_SHARED_PREDICATE) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE), PREDICATE) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, NOT_SHARED_PREDICATE), PREDICATE) << endl;
	}
	cout << endl;
	cout << endl;



		cout << "reif_dict_plain->getMaxGraphID()=" << reif_dict_plain->getMaxGraphID()<<endl;
		cout << "reif_dict_plain->getNunused()=" << reif_dict_plain->getNunused()<<endl;
	cout << "\t------------ global graphs ------------"<<endl;
	for (int globid=reif_dict_plain->getMaxObjectID()+1 ; globid <= reif_dict_plain->getMaxGraphID() ; globid++)
	{
		cout << "globid=" <<globid<<endl;

		cout << "\tplain_dict->idToString(" << globid << ", GRAPH) = " << reif_dict_plain->idToString(globid, GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(" << globid << ", GRAPH) = " << reif_dict_plain->getLocalId(globid, GRAPH) << endl;
	}
	cout << endl;

	cout << "\t------------ local graphs ------------"<<endl;
	for (int locid=0 ; locid < reif_dict_plain->getNunused() ; locid++)
	{
		cout << "\tplain_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << reif_dict_plain->getGlobalId(locid, UNUSED_GRAPH) << endl;
		cout << "\tplain_dict->getLocalId(reif_dict_plain->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << reif_dict_plain->getLocalId(reif_dict_plain->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
	}
	cout << endl;
	cout << endl;




	cout << "---------------------TESTS OF DICTIONARY METHODS-------------------------------------"<<endl;
	cout << "reif_dict_fs->getNshared() = " << reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getNsubjects() = " << reif_dict_fs->getNsubjects() << endl;
	cout << "reif_dict_fs->getNobjects() = " << reif_dict_fs->getNobjects() << endl << endl;
	cout << "reif_dict_fs->getNpredicates() = " << reif_dict_fs->getNpredicates() << endl << endl;
	cout << "reif_dict_fs->getNgraphs() = " << reif_dict_fs->getNgraphs() << endl << endl;
	cout << "reif_dict_fs->getNunused() = " << reif_dict_fs->getNunused() << endl << endl;

	cout << "maximum GLOBAL Id :     reif_dict_plain->getMaxID() = " << reif_dict_fs->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : reif_dict_plain->getMaxSubjectID() = " << reif_dict_fs->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  reif_dict_plain->getMaxObjectID() = " << reif_dict_fs->getMaxObjectID() << endl << endl;
	cout << "maximum GLOBAL predjId :  reif_dict_plain->getMaxPredicateID() = " << reif_dict_fs->getMaxPredicateID() << endl << endl;
	cout << "maximum GLOBAL graphId :  reif_dict_plain->getMaxGraphID() = " << reif_dict_fs->getMaxGraphID() << endl << endl;

	cout << "reif_dict_fs->getNshared() = " << reif_dict_fs->getNshared() << endl;
	cout << "reif_dict_fs->getNsubjects() = " << reif_dict_fs->getNsubjects() << endl;
	cout << "reif_dict_fs->getNobjects() = " << reif_dict_fs->getNobjects() << endl << endl;

	cout << "maximum GLOBAL Id :     reif_dict_fs->getMaxID() = " << reif_dict_fs->getMaxID() << endl;
	cout << "maximum GLOBAL subjId : reif_dict_fs->getMaxSubjectID() = " << reif_dict_fs->getMaxSubjectID() << endl;
	cout << "maximum GLOBAL objId :  reif_dict_fs->getMaxObjectID() = " << reif_dict_fs->getMaxObjectID() << endl << endl;


	cout << "\t------------ local shared ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", SHARED_SUBJECT) = " << reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_SUBJECT), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_SUBJECT), SUBJECT) << endl;
		cout << "\tfs_dict->getGlobalId(" << locid << ", SHARED_OBJECT) = " << reif_dict_fs->getGlobalId(locid, SHARED_OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", SHARED_OBJECT), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;


	cout << "\t------------ global subjects ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxSubjectID() ; globid++)
	{
		cout << "\tfs_dict->idToString(" << globid << ", SUBJECT) = " << reif_dict_fs->idToString(globid, SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", SUBJECT) = " << reif_dict_fs->getLocalId(globid, SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local subjects ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getMaxSubjectID()-reif_dict_fs->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_SUBJECT), SUBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_SUBJECT), SUBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ global objects ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxObjectID() ; globid++)
	{
		if(globid>reif_dict_fs->getNshared() && globid <= reif_dict_fs->getMaxSubjectID())
		{
			continue;
		}
		cout << "\tfs_dict->idToString(" << globid << ", OBJECT) = " << reif_dict_fs->idToString(globid, OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", OBJECT) = " << reif_dict_fs->getLocalId(globid, OBJECT) << endl;
	}
	cout << endl;

	cout << "\t------------ local objects ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getMaxObjectID() - reif_dict_fs->getMaxSubjectID() + reif_dict_fs->getNshared() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_OBJECT) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_OBJECT), OBJECT) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_OBJECT), OBJECT) << endl;
	}
	cout << endl;
	cout << endl;


	cout << "\t------------ global predicates ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxPredicateID() ; globid++)
	{
		cout << "\tfs_dict->idToString(" << globid << ", PREDICATE) = " << reif_dict_fs->idToString(globid, PREDICATE) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", PREDICATE) = " << reif_dict_fs->getLocalId(globid, PREDICATE) << endl;
	}
	cout << endl;

	cout << "\t------------ local predicates ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getNpredicates() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE) = " << reif_dict_fs->getGlobalId(locid, NOT_SHARED_PREDICATE) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", NOT_SHARED_PREDICATE), PREDICATE) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, NOT_SHARED_PREDICATE), PREDICATE) << endl;
	}
	cout << endl;
	cout << endl;



	cout << "\t------------ global graphs ------------"<<endl;
	for (int globid=1 ; globid <= reif_dict_fs->getMaxGraphID() ; globid++)
	{
		if(globid<=reif_dict_fs->getNsubjects() + reif_dict_fs->getNobjects() -reif_dict_fs->getNshared() || globid > reif_dict_fs->getMaxGraphID())
		{
			continue;
		}
		cout << "\tfs_dict->idToString(" << globid << ", GRAPH) = " << reif_dict_fs->idToString(globid, GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(" << globid << ", GRAPH) = " << reif_dict_fs->getLocalId(globid, GRAPH) << endl;
	}
	cout << endl;

	cout << "\t------------ local graphs ------------"<<endl;
	for (int locid=1 ; locid <= reif_dict_fs->getNunused() ; locid++)
	{
		cout << "\tfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << reif_dict_fs->getGlobalId(locid, UNUSED_GRAPH) << endl;
		cout << "\tfs_dict->getLocalId(reif_dict_fs->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << reif_dict_fs->getLocalId(reif_dict_fs->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
	}
	cout << endl;
	cout << endl;



    if(TriplesDictionary* tfs_dict = dynamic_cast<TriplesDictionary*>(reif_dict_fs))
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
	else if(GraphsDictionary* gfs_dict = dynamic_cast<GraphsDictionary*>(reif_dict_fs))
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
		cout << "gfs_dict->getNunused() = " <<gfs_dict->getNunused()<<endl;
		for (int locid=1 ; locid <= gfs_dict->getNunused() ; locid++)
		{
			cout << "\tgfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH) = " << gfs_dict->getGlobalId(locid, UNUSED_GRAPH) << endl;
			cout << "\tgfs_dict->getLocalId(gfs_dict->getGlobalId(" << locid << ", UNUSED_GRAPH), GRAPH) = " << gfs_dict->getLocalId(gfs_dict->getGlobalId(locid, UNUSED_GRAPH), GRAPH) << endl;
		}
		cout << endl;
	}


delete reif_dict_fs;
delete reif_dict_plain;




}
catch (exception& e)
{
	cout << e.what() << '\n';
}

}


