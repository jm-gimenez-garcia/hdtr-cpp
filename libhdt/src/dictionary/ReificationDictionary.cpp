#include "ReificationDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include "HDTSpecification.hpp"

namespace hdt{

ReificationDictionary::ReificationDictionary(HDTSpecification &spec) : triplesDict(new TriplesFourSectionDictionary(spec)), graphsDict(new GraphsFourSectionDictionary(spec)),maxTriplesDictID(0){}


size_t ReificationDictionary::getNumberOfElements()const
{return triplesDict->getNumberOfElements() + graphsDict->getNumberOfElements();}

 uint64_t ReificationDictionary::size()const
{return triplesDict->size() + graphsDict->size();}

unsigned int ReificationDictionary::getMaxID()const
{return triplesDict->getMaxID() + graphsDict->getMaxID();}

unsigned int ReificationDictionary::getNshared()const
{return triplesDict->getNshared() + graphsDict->getNshared();}

unsigned int ReificationDictionary::getNsubjects()const
{return triplesDict->getNsubjects() + graphsDict->getNsubjects();}

unsigned int ReificationDictionary::getNobjects()const
{return triplesDict->getNobjects() + graphsDict->getNobjects();}

unsigned int ReificationDictionary::getNpredicates()const
{return triplesDict->getNpredicates();}

unsigned int ReificationDictionary::getNgraphs()const
{return graphsDict->getNgraphs();}

IteratorUCharString* ReificationDictionary::getSubjects() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getSubjects());
	vectIt.push_back(graphsDict->getSubjects());
	CompositeIteratorUCharString* compositeSubjects = new CompositeIteratorUCharString(vectIt);
	return compositeSubjects;
}

IteratorUCharString* ReificationDictionary::getObjects() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getObjects());
	vectIt.push_back(graphsDict->getObjects());
	CompositeIteratorUCharString* compositeObjects= new CompositeIteratorUCharString(vectIt);
	return compositeObjects;
}

IteratorUCharString* ReificationDictionary::getShared() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getShared());
	vectIt.push_back(graphsDict->getShared());
	CompositeIteratorUCharString* compositeShared= new CompositeIteratorUCharString(vectIt);
	return compositeShared;
}

IteratorUCharString* ReificationDictionary::getPredicates() {
	return triplesDict->getPredicates();
}
IteratorUCharString* ReificationDictionary::getGraphs() {
	return graphsDict->getGraphs();
}

unsigned int ReificationDictionary::getMaxSubjectID()const
{return triplesDict->getMaxSubjectID() + graphsDict->getMaxSubjectID();}

unsigned int ReificationDictionary::getMaxObjectID()const
{return triplesDict->getMaxObjectID() + graphsDict->getMaxObjectID();}

unsigned int ReificationDictionary::getMaxPredicateID()const
{return triplesDict->getMaxPredicateID();}

unsigned int ReificationDictionary::getMaxGraphID()const
{return graphsDict->getMaxGraphID();}

std::string ReificationDictionary::getType()const
{return HDTVocabulary::DICTIONARY_TYPE_REIFICATION;}




unsigned int ReificationDictionary::stringToId(const std::string &key, TripleComponentRole position)const
{
        unsigned int ret;

        if(key.length()==0) {
                return 0;
        }

        switch (position) {


        case PREDICATE:
                return ret = triplesDict->stringToId(key, position);
        case GRAPH:
                return ret = graphsDict->stringToId(key, position);
	default:
                ret = triplesDict->stringToId(key, position);
                if( ret == 0) 
                	return ret = graphsDict->stringToId(key,position );
        	return ret;

        }
}


std::string ReificationDictionary::idToString(const unsigned int id, const TripleComponentRole position)const
{       
		
        switch (position) {
        	case PREDICATE:
        	        return triplesDict->idToString(id, position);
        	case GRAPH:
        	        return graphsDict->idToString(id, position);
		default:
			if(id > maxTriplesDictID)
				return graphsDict->idToString(id-maxTriplesDictID, position);
			else
				return triplesDict->idToString(id, position);

	}
        
}

unsigned int ReificationDictionary::getMapping()const{
	if(triplesDict->getMapping() == graphsDict->getMapping())
		return triplesDict->getMapping();
	else
		throw std::logic_error("triplesDict and graphsDict have different mapping");
	return 77;
}

void ReificationDictionary::import(Dictionary *other, ProgressListener *listener/*=NULL*/)
{throw std::logic_error("Not implemented");}

void ReificationDictionary::populateHeader(Header &header, std::string rootNode)
{throw std::logic_error("Not implemented");}

void ReificationDictionary::save(std::ostream &output, ControlInformation &ci, ProgressListener *listener/*=NULL*/)
{throw std::logic_error("Not implemented");}

void ReificationDictionary::load(std::istream &input, ControlInformation &ci, ProgressListener *listener/*=NULL*/)
{throw std::logic_error("Not implemented");}

size_t ReificationDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/)
{throw std::logic_error("Not implemented");}

IteratorUCharString *ReificationDictionary::getSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}

IteratorUInt *ReificationDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}




}
