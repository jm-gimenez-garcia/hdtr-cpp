
namespace hdt{

ReificationDictionary::ReificationDictionary(HDTSpecification &specification) : triplesDict(new TriplesFourSectionDictionary(spec)), GraphsDictionary(new GraphsFourSectionDictionary(spec)),maxTriplesDictID(0){}


size_t ReificationDictionary::getNumberOfElements()const
{return triplesDict->getNumberOfElements() + graphsDict->getNumberOfElements();}

 uint64_t ReificationDictionary::size()
{return triplesDict->size() + graphsDict->size();}

unsigned int getMaxID()
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

IteratorUCharString* ReificationDictionary::getSubjects()const {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getSubjects());
	vectIt.push_back(graphsDict->getSubjects());
	CompositeIteratorUCharString* compositeSubjects = new CompositeIteratorUCharString(vectIt);
	return compositeSubjects;
}

IteratorUCharString* ReificationDictionary::getObjects()const {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getObjects());
	vectIt.push_back(graphsDict->getObjects());
	CompositeIteratorUCharString* compositeObjects= new CompositeIteratorUCharString(vectIt);
	return compositeObjects;
}

IteratorUCharString* ReificationDictionary::getShared()const {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(triplesDict->getShared());
	vectIt.push_back(graphsDict->getShared());
	CompositeIteratorUCharString* compositeShared= new CompositeIteratorUCharString(vectIt);
	return compositeShared;
}

IteratorUCharString* ReificationDictionary::getPredicates()const {
	return triplesDict->getPredicates();
}
IteratorUCharString* ReificationDictionary::getGraphs()const {
	return graphsDict->getGraphs();
}

unsigned int ReificationDictionary::getMaxSubjectID()
{return triplesDict->getMaxSubjectID() + graphsDict->getMaxSubjectID();}

unsigned int ReificationDictionary::getMaxObjectID()
{return triplesDict->getMaxObjectID() + graphsDict->getMaxObjectID();}

unsigned int ReificationDictionary::getMaxPrediacteID()
{return triplesDict->getMaxPrediacteID();}

unsigned int ReificationDictionary::getMaxGraphID()
{return graphsDict->getMaxGraphID();}

string ReificationDictionary::getType()
{return HDTVocabulary::DICTIONARY_TYPE_REIFICATION;}




unsigned int ReificationDictionary::stringToId(std::string &key, TripleComponentRole position)
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


std::string ReificationDictionary::idToString(unsigned int id, TripleComponentRole position)
{       
		
        switch (position) {
        	case PREDICATE:
        	        return ret = triplesDict->idToString(id, position);
        	case GRAPH:
        	        return ret = graphsDict->idToString(id, position);
		default:
			if(id > maxTriplesDictID)
				return graphsDict->idToString(id-maxTriplesDictID, position);
			else
				return triplesDict->idToString(id, position);

	}
        
}




}
