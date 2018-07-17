#include "ModifiableReificationDictionary.hpp"
#include "DictionaryEntry.hpp"
#include "TriplesPlainDictionary.hpp"
#include "GraphsPlainDictionary.hpp"
namespace hdt{

ModifiableReificationDictionary::ModifiableReificationDictionary(): triplesModifDict(new TriplesPlainDictionary()), graphsModifDict(new GraphsPlainDictionary()){}

ModifiableReificationDictionary::ModifiableReificationDictionary(HDTSpecification &spec): BaseReificationDictionary(spec), triplesModifDict(new TriplesPlainDictionary(spec)), graphsModifDict(new GraphsPlainDictionary(spec)){}

ModifiableReificationDictionary::~ModifiableReificationDictionary(){
	if(triplesModifDict){
		delete triplesModifDict;
		triplesModifDict=NULL;
	}
	if(graphsModifDict){
		delete graphsModifDict;
		graphsModifDict=NULL;
	}
}

const TriplesDictionary* ModifiableReificationDictionary::getTriplesDictionaryPtr()const
{return static_cast<const TriplesDictionary*>(triplesModifDict);}

const GraphsDictionary* ModifiableReificationDictionary::getGraphsDictionaryPtr()const
{return static_cast<const GraphsDictionary*>(graphsModifDict);}

TriplesDictionary* ModifiableReificationDictionary::getTriplesDictionaryPtr()
{return static_cast<TriplesDictionary*>(triplesModifDict);}

GraphsDictionary* ModifiableReificationDictionary::getGraphsDictionaryPtr()
{return static_cast<GraphsDictionary*>(graphsModifDict);}


void  ModifiableReificationDictionary::startProcessing(ProgressListener *listener/*=NULL*/){
	triplesModifDict->startProcessing(listener);
	graphsModifDict->startProcessing(listener);
}
void  ModifiableReificationDictionary::stopProcessing(ProgressListener *listener/*=NULL*/){	

	IntermediateListener iListener(listener);
	iListener.setRange(0,50);
	split(listener);
	triplesModifDict->stopProcessing(listener);
	graphsModifDict->stopProcessing(listener);
}

unsigned int ModifiableReificationDictionary::insert(const std::string & str, const TripleComponentRole pos){
	if(str=="") return 0;


	DictEntryIt subjectIt = hashSubject.find(str.c_str());
	DictEntryIt objectIt = hashObject.find(str.c_str());
	DictEntryIt graphIt = hashGraph.find(str.c_str());

	bool foundSubject = subjectIt!=hashSubject.end();
	bool foundObject = objectIt!=hashObject.end();
	bool foundGraph = graphIt!=hashGraph.end();
	//cout << "A: " << foundSubject << " B: " << foundSubject << endl;

	if(pos==PREDICATE) {
		DictEntryIt it = hashPredicate.find(str.c_str());
		if(it!=hashPredicate.end()) {
			//cout << "  existing predicate: " << str << endl;
			return it->second->id;
		} else {
			DictionaryEntry *entry = new DictionaryEntry;
            entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();
			//cout << " Add new predicate: " << str.c_str() << endl;

			hashPredicate[entry->str] = entry;
			triplesModifDict->push_back(entry, NOT_SHARED_PREDICATE); // push_back set also entry->id for PREDICATE only
			return entry->id;
		}
	}
	if(pos==SUBJECT) {
		if( !foundSubject && !foundObject && !foundGraph) {
			// Did not exist, create new.
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();

			//cout << " Add new subject: " << str << endl;
			hashSubject[entry->str] = entry;
		} else if(foundSubject) {
			// Already exists in subjects.
			//cout << "   existing subject: " << str << endl;
		} else if(foundGraph) {
			// Already exists in graphss.
			//cout << "   existing subject as graph: " << str << endl;
			hashSubject[graphIt->second->str] = graphIt->second;
		} else if(foundObject) {
			// Already exists in objects.
			//cout << "   existing subject as object: " << str << endl;
			hashSubject[objectIt->second->str] = objectIt->second;
		}
	} else if(pos==OBJECT) {
		if(!foundSubject && !foundObject && !foundGraph) {
			// Did not exist, create new.
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();

			//cout << " Add new object: " << str << endl;
			hashObject[entry->str] = entry;
		} else if(foundObject) {
			// Already exists in objects.
			//cout << "     existing object: " << str << endl;
		} else if(foundGraph) {
			// Already exists in graphs.
			//cout << "     existing object as graph: " << str << endl;
			hashObject[graphIt->second->str] = graphIt->second;
		} else if(foundSubject) {
			// Already exists in subjects.
			//cout << "     existing object as subject: " << str << endl;
			hashObject[subjectIt->second->str] = subjectIt->second;
		}
	} else if(pos==GRAPH) {
		if(!foundSubject && !foundObject && !foundGraph) {
			// Did not exist, create new.
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			sizeStrings += str.length();

			//cout << " Add new object: " << str << endl;
			hashGraph[entry->str] = entry;
		} else if(foundGraph) {
			// Already exists in graphs.
			//cout << "     existing graph: " << str << endl;
		} else if(foundObject) {
			// Already exists in objects.
			//cout << "     existing graph as object: " << str << endl;
			hashGraph[objectIt->second->str] = objectIt->second;
		} else if(foundSubject) {
			// Already exists in subjects.
			//cout << "     existing object as subject: " << str << endl;
			hashGraph[subjectIt->second->str] = subjectIt->second;
		}
		
	}

	// FIXME: Return inserted index?
	return 0;

}


void ModifiableReificationDictionary::insert(const string& str, const DictionarySection& pos){

	if(str=="") return;

	DictionaryEntry *entry = new DictionaryEntry;
	entry->str = new char [str.length()+1];
	strcpy(entry->str, str.c_str());

	DictEntryIt gr_it = hashGraph.find(str.c_str());
	const bool is_graph = (gr_it!=hashGraph.end());
	if(is_graph)
	{
		graphsModifDict->push_back(entry, pos);
		hashGraph[entry->str] = entry;
	}
	else
		triplesModifDict->push_back(entry, pos);

	switch(pos) {
	case SHARED_SUBJECT:
	case SHARED_OBJECT:
	case SHARED_SUBJECT_GRAPH:
	case SHARED_OBJECT_GRAPH:
		//entry->id = subjects_shared.size();
		hashSubject[entry->str] = entry;
		hashObject[entry->str] = entry;
		break;
	case NOT_SHARED_SUBJECT:
	case NOT_SHARED_SUBJECT_GRAPH:
		//entry->id = subjects_shared.size()+subjects_not_shared.size();
		hashSubject[entry->str] = entry;
		break;
	case NOT_SHARED_OBJECT:
	case NOT_SHARED_OBJECT_GRAPH:
		//entry->id = subjects_shared.size()+objects_not_shared.size();
		hashObject[entry->str] = entry;
		break;
	case UNUSED_GRAPH:
		break;
	default:
		throw std::runtime_error("ModifiableReificationDictionary::insert : uknown DictionarySection");
		break;
	}

}


void ModifiableReificationDictionary::split(ProgressListener *listener) {
	triplesModifDict->startProcessing();
	graphsModifDict->startProcessing();

	unsigned int total = hashSubject.size()+hashObject.size();
	unsigned int count = 0;

	for(DictEntryIt subj_it = hashSubject.begin(); subj_it!=hashSubject.end() && subj_it->first; subj_it++) {

		//cout << "Check Graph: " << subj_it->first << endl;
		DictEntryIt grIt = hashGraph.find(subj_it->first);
		if(grIt == hashGraph.end()){
			//cout << "Check Subj: " << subj_it->first << endl;
			DictEntryIt obj_it = hashObject.find(subj_it->first);
	
			if(obj_it == hashObject.end()) {
				// Only subject in triples dictionary
				triplesModifDict->push_back(subj_it->second, NOT_SHARED_SUBJECT);
			} else {
				// subject+object in triples dictionary
				triplesModifDict->push_back(subj_it->second, SHARED_SUBJECT);
			}
		}else{
			//cout << "Check Subj: " << subj_it->first << endl;
			DictEntryIt other = hashObject.find(subj_it->first);
	
			if(other==hashObject.end()) {
				// Only subject in graphs dictionary
				graphsModifDict->push_back(subj_it->second, NOT_SHARED_SUBJECT_GRAPH);
			} else {
				// subject+object in graphs dictionary
				graphsModifDict->push_back(subj_it->second, SHARED_SUBJECT_GRAPH);
			}
			NOTIFYCOND(listener, "Extracting shared subjects", count, total);
		}
		count++;
	}

	for(DictEntryIt obj_it = hashObject.begin(); obj_it!=hashObject.end(); ++obj_it) {
		//cout << "Check Obj: " << obj_it->first << endl;
		DictEntryIt other = hashSubject.find(obj_it->first);

		if(other==hashSubject.end()) {
			DictEntryIt grIt = hashGraph.find(obj_it->first);
			if(grIt==hashGraph.end()){
				// Only object in triples dictionary
				triplesModifDict->push_back(obj_it->second, NOT_SHARED_OBJECT);
			}else{
				// Only object in graphs dictionary
				graphsModifDict->push_back(obj_it->second, NOT_SHARED_OBJECT_GRAPH);
			}
		}
		count++;
		NOTIFYCOND(listener, "Extracting shared objects", count, total);
	}

	for(DictEntryIt gr_it = hashGraph.begin(); gr_it!=hashGraph.end(); ++gr_it)
	{
		DictEntryIt subjIt = hashSubject.find(gr_it->first);
		DictEntryIt objIt = hashObject.find(gr_it->first);
		if(subjIt==hashSubject.end() && objIt==hashObject.end()){
			// unused graph in graphs dictionary
			graphsModifDict->push_back(gr_it->second, UNUSED_GRAPH);
		}
	}
	count++;
	NOTIFYCOND(listener, "Extracting graph objects", count, total);

}

std::string ModifiableReificationDictionary::getType()const
{return HDTVocabulary::DICTIONARY_TYPE_MODIFIABLEREIFICATION;}

}
