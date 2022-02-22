#include <istream>
#include <ostream>
#include <string>
#include "BaseFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include <Iterator.hpp>
#include "../libdcs/CSD_PFC.h"
#include "../libdcs/CSD_Cache2.h"
#include <HDTListener.hpp>


using namespace std;
namespace hdt {

GraphsFourSectionDictionary::GraphsFourSectionDictionary() : graphs(new csd::CSD_PFC()){}

GraphsFourSectionDictionary::GraphsFourSectionDictionary(HDTSpecification & spec) : graphs(new csd::CSD_PFC()){}

GraphsFourSectionDictionary::~GraphsFourSectionDictionary()
{clear_loc();}

void GraphsFourSectionDictionary::clear_loc()
{
	if (graphs!=NULL){
		delete graphs; graphs=NULL;
	}	
}

void GraphsFourSectionDictionary::create()
{
	clear_loc();
	graphs = new csd::CSD_PFC();
	BaseFourSectionDictionary::create();
}


uint64_t GraphsFourSectionDictionary::size()const{
	return BaseFourSectionDictionary::size()+graphs->getLength();
}

size_t GraphsFourSectionDictionary::getNumberOfElements()const{
	return BaseFourSectionDictionary::getNumberOfElements()+graphs->getLength();
}


unsigned int GraphsFourSectionDictionary::stringToId(const string &key, const TripleComponentRole position)const
{
	unsigned int ret;
	if(key.length()==0)
		return 0;

	switch (position)
	{
	case GRAPHS:
	case GRAPHS_SHARED:
	case GRAPHS_SUBJECTS:
	case GRAPHS_OBJECTS:
	case GRAPHS_UNUSED:
		ret = shared->locate((const unsigned char *)key.c_str(), key.length());
		if (ret!=0) return ret;
		ret = subjects->locate((const unsigned char *)key.c_str(), key.length());
		if (ret!=0) return ret + shared->getLength();
		ret = objects->locate((const unsigned char *)key.c_str(), key.length());
		if (ret!=0) return ret + shared->getLength() + subjects->getLength();
		ret = graphs->locate((const unsigned char *)key.c_str(), key.length());
		if(ret!=0) return ret + shared->getLength() + subjects->getLength() + objects->getLength();
		break;
	default:
		return BaseFourSectionDictionary::stringToId(key, position);
		break;
	}

	return 0;
}

void GraphsFourSectionDictionary::loadFourthSection(istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50,75);
	iListener.notifyProgress(0, "Dictionary read graphs.");
	graphs = csd::CSD::load(input);
	if(graphs==NULL){
		graphs = new csd::CSD_PFC();
		throw runtime_error("Could not read graphs.");
	}
	graphs = new csd::CSD_Cache2(graphs);
}



void GraphsFourSectionDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{

    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read graphs.");
    graphs = csd::CSD::create(ptr[count]);
    if(graphs==NULL){
        graphs = new csd::CSD_PFC();
        throw runtime_error("Could not read graphs.");
    }
    count += graphs->load(&ptr[count], ptrMax);
    graphs = new csd::CSD_Cache2(graphs);
}


void GraphsFourSectionDictionary::importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	if(GraphsDictionary* other_gd = dynamic_cast<GraphsDictionary*>(other))
	{
		NOTIFY(listener, "DictionaryPFC loading graphs", 25, 30);
		iListener.setRange(20, 21);
		IteratorUCharString *itGr = other_gd->getGraphs();
		graphs = loadSection(itGr, blocksize, &iListener);
		delete itGr;
	}
	else
		throw runtime_error("Downcast error from Dictionary to GraphsDictionary.");
}


IteratorUCharString* GraphsFourSectionDictionary::getGraphs() {
	return graphs->listAll();
}
IteratorUCharString* GraphsFourSectionDictionary::getGraphs() const{
	return graphs->listAll();
}

void GraphsFourSectionDictionary::saveFourthSection(ostream& output, IntermediateListener& iListener){

	iListener.setRange(45,60);
	iListener.notifyProgress(0, "Dictionary save graphs.");
	graphs->save(output);
}

unsigned int GraphsFourSectionDictionary::getNunused()const
{return graphs->getLength();}

unsigned int GraphsFourSectionDictionary::getMaxID()const
{return BaseFourSectionDictionary::getMaxID() + GraphsFourSectionDictionary::getNunused();}

csd::CSD *GraphsFourSectionDictionary::getDictionarySection(DictionarySection section) const{
	if(section==GRAPHS_UNUSED)
        return graphs;
	else
		return BaseFourSectionDictionary::getDictionarySection(section);
}

csd::CSD *GraphsFourSectionDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
	if(position==GRAPH) 
        return graphs;
	else
		return BaseFourSectionDictionary::getDictionarySection(id, position);
}

unsigned int GraphsFourSectionDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch(position)
	{
		case SHARED_SUBJECTS:
		case SHARED_SUBJECTS_GRAPHS:
		case SHARED_OBJECTS:
		case SHARED_OBJECTS_GRAPHS:
		case GRAPHS_SHARED:
			return id;
			break;
		case NOT_SHARED_SUBJECTS:
		case NOT_SHARED_SUBJECTS_GRAPHS:
		case GRAPHS_SUBJECTS:
			return id+shared->getLength();
			break;
		case NOT_SHARED_OBJECTS:
		case NOT_SHARED_OBJECTS_GRAPHS:
		case GRAPHS_OBJECTS:
			return id+shared->getLength()+subjects->getLength();
			break;
		case GRAPHS_UNUSED:
			return id+shared->getLength()+subjects->getLength()+objects->getLength();
			break;
		default:
			return BaseFourSectionDictionary::getGlobalId(mapping, id, position);
		}
}

unsigned int GraphsFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{

	switch(position)
	{
		case SHARED_SUBJECTS:
		case SHARED_SUBJECTS_GRAPHS:
		case SHARED_OBJECTS:
		case SHARED_OBJECTS_GRAPHS:
		case GRAPHS_SHARED:
			return id;
			break;
		case NOT_SHARED_SUBJECTS:
		case NOT_SHARED_SUBJECTS_GRAPHS:
		case GRAPHS_SUBJECTS:
			return id-shared->getLength();
			break;
		case NOT_SHARED_OBJECTS:
		case NOT_SHARED_OBJECTS_GRAPHS:
		case GRAPHS_OBJECTS:
			return id-shared->getLength()-subjects->getLength();
			break;
		case GRAPHS_UNUSED:
			return id-shared->getLength()-subjects->getLength()-objects->getLength();
			break;
		default:
			return BaseFourSectionDictionary::getLocalId(mapping, id, position);
		}
}

void GraphsFourSectionDictionary::getSuggestions(const char *base, TripleComponentRole role, vector<string> &out, int maxResults)
{
	if(role==GRAPH)
		graphs->fillSuggestions(base, out, maxResults);
	else
		BaseFourSectionDictionary::getSuggestions(base, role, out, maxResults);	
}

hdt::IteratorUCharString* GraphsFourSectionDictionary::getSuggestions(const char *prefix, TripleComponentRole role){
	if(role==GRAPH) 
		return graphs->getSuggestions(prefix);
	else
		return BaseFourSectionDictionary::getSuggestions(prefix, role);
}


IteratorUInt* GraphsFourSectionDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role){
	if(role==GRAPH) 
		return graphs->getIDSuggestions(prefix);

	return BaseFourSectionDictionary::getIDSuggestions(prefix, role);
}



}
