
#include "BaseFourSectionDictionary.hpp"
#include "GraphsLiteralDictionary.hpp"

#if HAVE_CDS

#include "HDTVocabulary.hpp"
#include "Iterator.hpp"
#include "../libdcs/CSD_PFC.h"
#include "../libdcs/CSD_Cache.h"
#include "../libdcs/CSD_Cache2.h"
#include "Header.hpp"

namespace hdt {
GraphsLiteralDictionary::GraphsLiteralDictionary() : graphs(new csd::CSD_PFC()){}

GraphsLiteralDictionary::~GraphsLiteralDictionary(){
	clear_loc();
}

void GraphsLiteralDictionary::clear_loc()
{
	if (graphs!=NULL){
		delete graphs; graphs=NULL;
	}	
}
void GraphsLiteralDictionary::create()
{
	clear_loc();
	graphs = new csd::CSD_PFC();
	BaseLiteralDictionary::create();
}


unsigned int GraphsLiteralDictionary::getNunused()const
{return graphs->getLength();}

unsigned int GraphsLiteralDictionary::stringToId(const std::string &key, const TripleComponentRole position)const {

	if (key.length() == 0) 
		return 0;

	if(position != GRAPH)
		return BaseLiteralDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
		ret = graphs->locate((const unsigned char *) key.c_str(), key.length());
		if (ret != 0) 
			return BaseLiteralDictionary::getGlobalId(ret, UNUSED_GRAPH);
	}
    return 0;
}

void GraphsLiteralDictionary::loadFourthSection(std::istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50, 75);
	iListener.notifyProgress(0, "Dictionary read graphs.");

	graphs = csd::CSD::load(input);
	if (graphs == NULL) {
		graphs = new csd::CSD_PFC();
		throw std::runtime_error("Could not read graphs.");
	}
	graphs = new csd::CSD_Cache2(graphs);
}


void GraphsLiteralDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read graphs.");
    graphs = csd::CSD::create(ptr[count]);
    if(graphs==NULL){
        graphs = new csd::CSD_PFC();
        throw std::runtime_error("Could not read graphs.");
    }
    count += graphs->load(&ptr[count], ptrMax);
    graphs = new csd::CSD_Cache(graphs);
}

void GraphsLiteralDictionary::importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener)
{
	if(GraphsDictionary* other_gd = dynamic_cast<GraphsDictionary*>(other))
	{
		//NOTIFY(listener, "DictionaryPFC loading graphs", 25, 30);
		IteratorUCharString *itPred = other_gd->getGraphs();
		iListener.setRange(20, 21);
		graphs = loadSectionPFC(itPred, blocksize, &iListener);
		graphs = new csd::CSD_Cache2(graphs);
		delete itPred;
	}
	else
		throw std::runtime_error("Downcast error from Dictionary to GraphsDictionary.");
}

void GraphsLiteralDictionary::saveFourthSection(std::ostream & output, IntermediateListener& iListener)
{
	iListener.setRange(45, 60);
	iListener.notifyProgress(0, "Dictionary save graphs.");
	graphs->save(output);
}

IteratorUCharString *GraphsLiteralDictionary::getGraphs() 
{throw std::logic_error("Not implemented");}
IteratorUCharString *GraphsLiteralDictionary::getGraphs() const
{throw std::logic_error("Not implemented");}

void GraphsLiteralDictionary::populateHeaderNumFourthSection(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNgraphs());}

void GraphsLiteralDictionary::populateHeaderMaxFourthSectionId(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXGRAPHID, getMaxGraphID());}

uint64_t GraphsLiteralDictionary::size()const
{return BaseLiteralDictionary::size() + graphs->getSize();}


unsigned int GraphsLiteralDictionary::getMaxID()const
{return BaseLiteralDictionary::getMaxID() + graphs->getLength();}

size_t GraphsLiteralDictionary::getNumberOfElements()const {
	return BaseLiteralDictionary::getNumberOfElements() + graphs->getLength();
}

csd::CSD *GraphsLiteralDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const
{

	if(position==GRAPH)
	{
		const unsigned int sh_length = shared->getLength();
		const unsigned int sub_length = subjects->getLength();
		const unsigned int obj_length = objectsNotLiterals->getLength()+objectsLiterals->getLength();
		const unsigned int gr_length = graphs->getLength();
		unsigned int last_obj_sub_glob_id;

		if(mapping==MAPPING1)
			last_obj_sub_glob_id = sh_length + sub_length+obj_length;
		else if (mapping==MAPPING2)
		{
			const unsigned int max_sub_obj_length = (obj_length>sub_length) ? obj_length : sub_length;
			last_obj_sub_glob_id = sh_length + max_sub_obj_length ;
		}
		else
			throw std::runtime_error("Unknown mapping");


		const unsigned int last_gr_glob_id  = last_obj_sub_glob_id + gr_length;
		if(id>last_obj_sub_glob_id && id<=last_gr_glob_id)
			return graphs;
		else
			throw std::runtime_error("This globalID does not correspond to a graph");
	}
	else
		return BaseLiteralDictionary::getDictionarySection(id, position);

}

unsigned int GraphsLiteralDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch(position)
	{
		case UNUSED_GRAPH:
			return id;
		case NOT_SHARED_SUBJECT_GRAPH:
		case NOT_SHARED_OBJECT_GRAPH:
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			return BaseLiteralDictionary::getGlobalId(mapping, id, position);
		default:
			throw runtime_error("Invalid DictionarySection in GraphsDictionary");
	}
}


unsigned int GraphsLiteralDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	
	if(position==GRAPH)
	{
		const unsigned int sh_length = shared->getLength();
		const unsigned int sub_length = subjects->getLength();
		const unsigned int obj_length = objectsNotLiterals->getLength()+objectsLiterals->getLength();
		const unsigned int gr_length = graphs->getLength();
		unsigned int last_obj_sub_glob_id;
		unsigned int locid_shift = 0; 

		if(mapping==MAPPING1)
		{
			locid_shift = 2;
			last_obj_sub_glob_id = sh_length + sub_length+obj_length;
		}
		else if (mapping==MAPPING2)
		{
			const unsigned int max_sub_obj_length = (obj_length>sub_length) ? obj_length : sub_length;
			last_obj_sub_glob_id = sh_length + max_sub_obj_length ;
		}
		else
			throw std::runtime_error("Unknown mapping");


		const unsigned int last_gr_glob_id  = last_obj_sub_glob_id + gr_length;
		if(id>last_obj_sub_glob_id && id<=last_gr_glob_id)
			return locid_shift+id-last_obj_sub_glob_id;
		else
			throw std::runtime_error("This globalID does not correspond to an unused graph");
	}
	else
		return BaseLiteralDictionary::getLocalId(mapping, id, position);

}

void GraphsLiteralDictionary::getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults)
{
	if (role == GRAPH) 
		//graphs->fillSuggestions(base, out, maxResults);
		return;
	else
		BaseLiteralDictionary::getSuggestions(base, role, out, maxResults);
}


}
#endif
