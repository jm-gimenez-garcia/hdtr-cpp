
#include "GraphsPlainDictionary.hpp"
#include <HDTVocabulary.hpp>
#include <HDTListener.hpp>
#include <Header.hpp>

namespace hdt {

string GraphsPlainDictionary::getType()const {
	return HDTVocabulary::DICTIONARY_TYPE_PLAIN_GRAPH;
}

GraphsPlainDictionary::~GraphsPlainDictionary() {
	for(unsigned int i=0;i<graphs.size();i++) {
		delete [] graphs[i]->str;
		delete graphs[i];
	}
}

unsigned int GraphsPlainDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{	

	if(position!=GRAPH)
		return BasePlainDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
		if(key.length()==0 || key.at(0) == '?') 
			return 0;
		return idFromString(graphs, key);
	}
}

size_t GraphsPlainDictionary::getNumberOfElements()const
{return BasePlainDictionary::getNumberOfElements() + graphs.size();}



void GraphsPlainDictionary::startProcessing(ProgressListener *listener)
{
	graphs.clear();
	BasePlainDictionary::startProcessing(listener);
}

void GraphsPlainDictionary::push_back(DictionaryEntry* entry, DictionarySection pos){
	if (!entry)
		return;
	
	switch(pos)
	{
		case UNUSED_GRAPH:
			graphs.push_back(entry);
			sizeStrings+=strlen(entry->str);
			break;
		case NOT_SHARED_SUBJECT_GRAPH:
		case NOT_SHARED_OBJECT_GRAPH:
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			BasePlainDictionary::push_back(entry, pos);
			break;
		default:
			throw runtime_error("Invalid DictionarySection in GraphsDictionary");
	}

}

void GraphsPlainDictionary::populateHeaderFourthElementNum(Header &header, string rootNode){
	header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMGRAPHS, getNgraphs());
}

void GraphsPlainDictionary::populateHeaderFourthElementMaxId(Header &header, string rootNode){
	header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXGRAPHID, getMaxGraphID());
}

void GraphsPlainDictionary::saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//graphs
	for (unsigned int i = 0; i < graphs.size(); i++) {
		output << graphs[i]->str;
		output.put(marker); //character  to split file
		counter++;
		NOTIFYCOND(listener, "GraphsPlainDictionary saving graphs", counter, getNumberOfElements());
	}
}

void GraphsPlainDictionary::insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements)
{
	NOTIFYCOND(&iListener, "Dictionary loading graphs.", numLine, numElements);
	insert(line, UNUSED_GRAPH);	
}



IteratorUCharString *GraphsPlainDictionary::getGraphs() {
	if (dict_sorted)
		return new DictIterator(graphs);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}
IteratorUCharString *GraphsPlainDictionary::getGraphs() const{
	if (dict_sorted)
		return new DictIterator_const(graphs);
	else
		throw std::runtime_error("The Plain dictionary has not been sorted !");
}

unsigned int GraphsPlainDictionary::getNunused()const
{return graphs.size();}


void GraphsPlainDictionary::updateIDs() {
	for (unsigned int i = 0; i < shared.size(); i++) 
		shared[i]->id = getGlobalId(i, SHARED_SUBJECT_GRAPH);
	for (unsigned int i = 0; i < subjects.size(); i++) 
		subjects[i]->id = getGlobalId(i, NOT_SHARED_SUBJECT_GRAPH);
	for (unsigned int i = 0; i < objects.size(); i++) 
		objects[i]->id = getGlobalId(i, NOT_SHARED_OBJECT_GRAPH);
	for (unsigned int i = 0; i < graphs.size(); i++) 
		graphs[i]->id = getGlobalId(mapping, i, UNUSED_GRAPH);
}

const vector<DictionaryEntry*>& GraphsPlainDictionary::getDictionaryEntryVector(unsigned int id, TripleComponentRole position) const{
	return (position==GRAPH) ? graphs : BasePlainDictionary::getDictionaryEntryVector(id, position);
}

unsigned int GraphsPlainDictionary::getGlobalId(unsigned int mapping_type, unsigned int id, DictionarySection position) const
{	

	switch(position)
	{
		case UNUSED_GRAPH:
			if(mapping_type==MAPPING1)
				return shared.size()+subjects.size()+objects.size()+id+1;
			else if(mapping_type==MAPPING2)
			{
				unsigned int max_s_o = (subjects.size()>objects.size()) ? subjects.size() : objects.size();
				return shared.size()+max_s_o+id+1;
			}
			else
				throw std::runtime_error("Unknown mapping");
			break;
		case NOT_SHARED_SUBJECT_GRAPH:
		case NOT_SHARED_OBJECT_GRAPH:
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			return BasePlainDictionary::getGlobalId(mapping_type, id, position);
		default:
			throw runtime_error("Invalid DictionarySection in GraphsDictionary");
	}
}
unsigned int GraphsPlainDictionary::getLocalId(unsigned int mapping_type, unsigned int id, TripleComponentRole position) const{

	const unsigned int sh_length = shared.size();
	const unsigned int sub_length = subjects.size();
	const unsigned int obj_length = objects.size();
	const unsigned int gr_length = graphs.size();
	unsigned int last_obj_sub_glob_id;
	unsigned int last_gr_glob_id;
	const unsigned int shift = 1; //shift between

	if(mapping_type==MAPPING1)
	{
		last_obj_sub_glob_id = sh_length + sub_length+obj_length;
		last_gr_glob_id  = last_obj_sub_glob_id + gr_length;
	}
	else if (mapping_type==MAPPING2)
	{
		const unsigned int max_sub_obj_length = (obj_length>sub_length) ? obj_length : sub_length;
		last_obj_sub_glob_id = sh_length + max_sub_obj_length ;
		last_gr_glob_id = last_obj_sub_glob_id + gr_length;
	}
	else
		throw std::runtime_error("Unknown mapping");

	if(position==GRAPH)
		if(id>last_obj_sub_glob_id && id<=last_gr_glob_id)
			return id-last_obj_sub_glob_id-shift;
		else
			throw std::runtime_error("This globalID does not correspond to an unused graph");
	else
		return BasePlainDictionary::getLocalId(mapping_type, id, position);



}
void GraphsPlainDictionary::updateID(unsigned int oldid, unsigned int newid, DictionarySection position) {
	switch (position)
	{
		case UNUSED_GRAPH:
			graphs[oldid]->id = newid;
			break;
		case NOT_SHARED_SUBJECT_GRAPH:
		case NOT_SHARED_OBJECT_GRAPH:
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			BasePlainDictionary::updateID(oldid, newid, position);
			break;
		default:
			throw runtime_error("Invalid DictionarySection in GraphsDictionary");
	}
}



}
