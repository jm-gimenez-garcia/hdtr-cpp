#include "BaseReificationDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include "HDTSpecification.hpp"
#include "HDTFactory.hpp"

namespace hdt{

BaseReificationDictionary::BaseReificationDictionary(){}
BaseReificationDictionary::BaseReificationDictionary(HDTSpecification &spec){}

BaseReificationDictionary::~BaseReificationDictionary(){}


size_t BaseReificationDictionary::getNumberOfElements()const
{return getTriplesDictionaryPtr()->getNumberOfElements() + getGraphsDictionaryPtr()->getNumberOfElements();}

 uint64_t BaseReificationDictionary::size()const
{return getTriplesDictionaryPtr()->size() + getGraphsDictionaryPtr()->size();}

unsigned int BaseReificationDictionary::getMaxID()const{
	return getMaxGraphID(); 
}

unsigned int BaseReificationDictionary::getNshared()const
{return getTriplesDictionaryPtr()->getNshared() + getGraphsDictionaryPtr()->getNshared();}

unsigned int BaseReificationDictionary::getNsubjects()const
{return getTriplesDictionaryPtr()->getNsubjects() + getGraphsDictionaryPtr()->getNsubjects();}

unsigned int BaseReificationDictionary::getNobjects()const
{return getTriplesDictionaryPtr()->getNobjects() + getGraphsDictionaryPtr()->getNobjects();}

unsigned int BaseReificationDictionary::getNpredicates()const
{return getTriplesDictionaryPtr()->getNpredicates();}

unsigned int BaseReificationDictionary::getNunused()const
{return getGraphsDictionaryPtr()->getNunused();}

unsigned int BaseReificationDictionary::getNgraphs()const
{return getGraphsDictionaryPtr()->getNgraphs();}

IteratorUCharString* BaseReificationDictionary::getSubjects() {}
IteratorUCharString* BaseReificationDictionary::getSubjects() const{}
IteratorUCharString* BaseReificationDictionary::getObjects() {}
IteratorUCharString* BaseReificationDictionary::getObjects() const{}
IteratorUCharString* BaseReificationDictionary::getShared() {}
IteratorUCharString* BaseReificationDictionary::getShared() const{}
IteratorUCharString* BaseReificationDictionary::getPredicates() {}
IteratorUCharString* BaseReificationDictionary::getPredicates() const{}
IteratorUCharString* BaseReificationDictionary::getGraphs() {}
IteratorUCharString* BaseReificationDictionary::getGraphs()const {}
/*IteratorUCharString* BaseReificationDictionary::getSubjects() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(getTriplesDictionaryPtr()->getSubjects());
	vectIt.push_back(getGraphsDictionaryPtr()->getSubjects());
	CompositeIteratorUCharString* compositeSubjects = new CompositeIteratorUCharString(vectIt);
	return compositeSubjects;
}

IteratorUCharString* BaseReificationDictionary::getObjects() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(getTriplesDictionaryPtr()->getObjects());
	vectIt.push_back(getGraphsDictionaryPtr()->getObjects());
	CompositeIteratorUCharString* compositeObjects= new CompositeIteratorUCharString(vectIt);
	return compositeObjects;
}

IteratorUCharString* BaseReificationDictionary::getShared() {
	std::vector<IteratorUCharString*> vectIt;
	vectIt.push_back(getTriplesDictionaryPtr()->getShared());
	vectIt.push_back(getGraphsDictionaryPtr()->getShared());
	CompositeIteratorUCharString* compositeShared= new CompositeIteratorUCharString(vectIt);
	return compositeShared;
}

IteratorUCharString* BaseReificationDictionary::getPredicates() {
	return getTriplesDictionaryPtr()->getPredicates();
}
IteratorUCharString* BaseReificationDictionary::getGraphs() {
	return getGraphsDictionaryPtr()->getGraphs();
}
*/

//id is the local ID in the Triples/Graphs Dictionary ; the return value is the global ID in the reification dictionary 
// value MAPPING1 is not used afterwards
unsigned int BaseReificationDictionary::getGlobalId(unsigned int id, DictionarySection pos)const{		
	getGlobalId(MAPPING1, id, pos);
}

//locid is the local ID in the Triples/Graphs Dictionary ; the return value is the global ID in the reification dictionary 
// mapping_type is not used ; only the mapping of TriplesDictionary or GraphsDictionary is used
unsigned int BaseReificationDictionary::getGlobalId(unsigned int mapping_type, unsigned int locid, DictionarySection pos)const{
	switch(pos){
		case SHARED_OBJECT:
		case SHARED_SUBJECT:
		case NOT_SHARED_SUBJECT:
		case NOT_SHARED_PREDICATE:
		case NOT_SHARED_OBJECT:
			return getGlobalIdFromTrGrGlobalId(getTriplesDictionaryPtr()->getGlobalId(locid, pos), pos);
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			return getGlobalIdFromTrGrGlobalId(getGraphsDictionaryPtr()->getGlobalId(locid, SHARED_OBJECT), pos);
		case NOT_SHARED_SUBJECT_GRAPH:
			return getGlobalIdFromTrGrGlobalId(getGraphsDictionaryPtr()->getGlobalId(locid, NOT_SHARED_SUBJECT), pos);
		case NOT_SHARED_OBJECT_GRAPH:
			return getGlobalIdFromTrGrGlobalId(getGraphsDictionaryPtr()->getGlobalId(locid, NOT_SHARED_OBJECT), pos);
		case UNUSED_GRAPH:
			cout << "getGraphsDictionaryPtr()->getGlobalId(locid=" << locid<< ", pos=" << pos << ") = " << getGraphsDictionaryPtr()->getGlobalId(locid, pos) << endl;
			return getGlobalIdFromTrGrGlobalId(getGraphsDictionaryPtr()->getGlobalId(locid, pos), pos);
		default:
			throw std::logic_error("Unkown type of DictionarySection");
	}

	return 0;
}

unsigned int BaseReificationDictionary::getGlobalIdFromTrGrGlobalId(unsigned int tr_gr_globid, DictionarySection pos)const{
	const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();

	switch(pos){
		case SHARED_OBJECT:
		case SHARED_SUBJECT:
			return tr_gr_globid;
		case SHARED_OBJECT_GRAPH:
		case SHARED_SUBJECT_GRAPH:
			return Tsh + tr_gr_globid;
		case NOT_SHARED_SUBJECT:
			return Gsh + tr_gr_globid;
		case NOT_SHARED_SUBJECT_GRAPH:
			return Tsh + Tsubj + tr_gr_globid;
		case NOT_SHARED_OBJECT:
			if (getTriplesDictionaryPtr()->getMapping() == MAPPING1)
				return Gsh + Gsubj + tr_gr_globid;
			else if (getTriplesDictionaryPtr()->getMapping() == MAPPING2)
				return Gsh + Tsubj + Gsubj + tr_gr_globid;
			else
				throw std::logic_error("Unkown type of mapping");
			break;
		case NOT_SHARED_OBJECT_GRAPH:
			if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
				return Tsh + Tsubj + Tobj + tr_gr_globid;
			else if (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
				return Tsh + Tsubj + Gsubj + Tobj + tr_gr_globid;
			else
				throw std::logic_error("Unkown type of mapping");
			break;
		case NOT_SHARED_PREDICATE:
			return tr_gr_globid;
		case UNUSED_GRAPH:
			if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
				return Tsh + Tsubj + Tobj + tr_gr_globid;
			else if (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
			{
				const unsigned int min_gr_subj_obj = (Gobj < Gsubj) ? Gobj : Gsubj;
				return Tsh + Tsubj + min_gr_subj_obj + Tobj + tr_gr_globid;
			}
			else
				throw std::logic_error("Unkown type of mapping");
			break;
		default:
			throw std::logic_error("Unkown type of DictionarySection");
	}

	return 0;
}
// id is the global ID in the ReificationDictionary and the return value is the local id in the Triples/Graphs Dictionary
unsigned int BaseReificationDictionary::getLocalId(unsigned int id, TripleComponentRole position)const
{
	const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();


	switch(position){
		case SUBJECT:
			if(id <= Tsh)
				return getTriplesDictionaryPtr()->getLocalId(id, position);
			else if (id <= Tsh + Gsh)
				return getGraphsDictionaryPtr()->getLocalId(id-Tsh, position);
			else if (id <= Tsh + Gsh + Tsubj)
				return getTriplesDictionaryPtr()->getLocalId(id-Gsh, position);
			else if (id <= Tsh + Gsh + Tsubj + Gsubj)
				return getGraphsDictionaryPtr()->getLocalId(id-Tsh-Tsubj, position);
			else
				throw std::logic_error("Id too high to be a subject");
			break;
		case OBJECT:
			if(id <= Tsh)
				return getTriplesDictionaryPtr()->getLocalId(id, position);
			else if (id <= Tsh + Gsh)
				return getGraphsDictionaryPtr()->getLocalId(id-Tsh, position);
			if(id <= Tsh + Gsh + Tsubj + Gsubj || id > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj)
			{
				
				cerr << "id = " << id <<endl; 
				cerr << "Tsh + Gsh + Tsubj + Gsubj = " << Tsh + Gsh + Tsubj + Gsubj<< endl;
				cerr << " Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj = " << Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj << endl;
				throw std::logic_error("Id doesn't correspond to an object");
				return 0;
			}
			if (id <= Tsh + Gsh + Tsubj + Gsubj +Tobj) // if object in triplesdictionary
			{
				if (getTriplesDictionaryPtr()->getMapping() == MAPPING1)
					return getTriplesDictionaryPtr()->getLocalId(MAPPING1, id - Gsh - Gsubj, position);
				else if (getTriplesDictionaryPtr()->getMapping() == MAPPING2)
					return getTriplesDictionaryPtr()->getLocalId(MAPPING2, id - Gsh - Tsubj - Gsubj, position);
				else
					throw std::logic_error("Unkown type of mapping");
			}
			else // if object in graphsdictionary 
			{
				if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
					return getGraphsDictionaryPtr()->getLocalId(MAPPING1, id - Tsh - Tsubj - Tobj, position);
				else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
					return getGraphsDictionaryPtr()->getLocalId(MAPPING2, id - Tsh - Tsubj - Gsubj - Tobj, position);
				else
					throw std::logic_error("Unkown type of mapping");
			}
			break;
		case PREDICATE:
			if (id <= getTriplesDictionaryPtr()->getNpredicates())
				return getTriplesDictionaryPtr()->getLocalId(id, position);
			else
				throw std::logic_error("Id too high to be a predicate");
			break;
		case GRAPH:
			const unsigned int Gu = getGraphsDictionaryPtr()->getNunused();
			if(id <= Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj || id > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj + Gu)
			{
				cerr << "Id doesn't correspond to an unused graph" << endl;;
				return 0;
			}
			if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
			{
					return getGraphsDictionaryPtr()->getLocalId(MAPPING1, id - Tsh - Tsubj - Tobj, position);
			}
			else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
			{
				const unsigned int lower_subj_obj_size = (Gobj < Gsubj) ? Gobj : Gsubj ;
					return getGraphsDictionaryPtr()->getLocalId(MAPPING2,id - Tsh - Tsubj - Tobj - lower_subj_obj_size, position);
			}
			else
				throw std::logic_error("Unkown type of mapping");
			break;
	}
	return 0;
}



unsigned int BaseReificationDictionary::getTrGrGlobalIdFromGlobalId(unsigned int globid, TripleComponentRole position)const
{
	const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();


	switch(position){
		case SUBJECT:
			if(globid <= Tsh)
				return globid;
			else if (globid <= Tsh + Gsh)
				return globid-Tsh;
			else if (globid <= Tsh + Gsh + Tsubj)
				return globid-Gsh;
			else if (globid <= Tsh + Gsh + Tsubj + Gsubj)
				return globid-Tsh-Tsubj;
			else
				throw std::logic_error("Id too high to be a subject");
			break;
		case OBJECT:
			if(globid <= Tsh)
				return globid;
			else if (globid <= Tsh + Gsh)
				return globid-Tsh;
			if(globid <= Tsh + Gsh + Tsubj + Gsubj || globid > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj)
			{
				
				cerr << "globid = " << globid <<endl; 
				cerr << "Tsh + Gsh + Tsubj + Gsubj = " << Tsh + Gsh + Tsubj + Gsubj<< endl;
				cerr << " Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj = " << Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj << endl;
				throw std::logic_error("Id doesn't correspond to an object");
				return 0;
			}
			if (globid <= Tsh + Gsh + Tsubj + Gsubj +Tobj) // if object in triplesdictionary
			{
				if (getTriplesDictionaryPtr()->getMapping() == MAPPING1)
					return globid - Gsh - Gsubj;
				else if (getTriplesDictionaryPtr()->getMapping() == MAPPING2)
					return globid - Gsh - Tsubj - Gsubj;
				else
					throw std::logic_error("Unkown type of mapping");
			}
			else // if object in graphsdictionary 
			{
				if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
					return globid - Tsh - Tsubj - Tobj;
				else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
					return globid - Tsh - Tsubj - Gsubj - Tobj;
				else
					throw std::logic_error("Unkown type of mapping");
			}
			break;
		case PREDICATE:
			if (globid <= getTriplesDictionaryPtr()->getNpredicates())
				return globid;
			else
				throw std::logic_error("Id too high to be a predicate");
			break;
		case GRAPH:
			const unsigned int Gu = getGraphsDictionaryPtr()->getNunused();
			if(globid <= Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj || globid > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj + Gu)
			{
				throw std::logic_error("Id doesn't correspond to an unused graph");
				return 0;
			}
			if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
					return globid - Tsh - Tsubj - Tobj;
			else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
			{
				const unsigned int lower_subj_obj_size = (Gobj < Gsubj) ? Gobj : Gsubj ;
					return globid - Tsh - Tsubj - Tobj - lower_subj_obj_size;
			}
			else
				throw std::logic_error("Unkown type of mapping");
			break;
	}
	return 0;
}


unsigned int BaseReificationDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const{
	return BaseReificationDictionary::getLocalId(id, position);
}

unsigned int BaseReificationDictionary::getMaxSubjectID()const{
	return getNsubjects(); 
}
unsigned int BaseReificationDictionary::getMaxObjectID()const
{return getNsubjects() + getNobjects() - getNshared();}

unsigned int BaseReificationDictionary::getMaxPredicateID()const
{return getNpredicates();}

unsigned int BaseReificationDictionary::getMaxGraphID()const
{
	return  getNsubjects() + getNobjects() - getNshared() + getNunused();}


unsigned int BaseReificationDictionary::stringToId(const std::string &key, TripleComponentRole position)const
{
        unsigned int ret;

        if(key.length()==0) {
			return 0;
        }

        switch (position) {
			case PREDICATE:
                ret = getTriplesDictionaryPtr()->stringToId(key, position);
				break;
			case GRAPH:
                ret = getGraphsDictionaryPtr()->stringToId(key, position);
				break;
			default:
                ret = getTriplesDictionaryPtr()->stringToId(key, position);
                if( ret == 0) 
                	ret = getGraphsDictionaryPtr()->stringToId(key,position );
				break;
        }
		
}




std::string BaseReificationDictionary::idToString(const unsigned int id, const TripleComponentRole pos)const
{       
	const unsigned int Tsh = getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsh = getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
	const unsigned int Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	const unsigned int Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();

	if(pos==SUBJECT || pos==OBJECT)
	{
		if (id <= Tsh )
			return getTriplesDictionaryPtr()->getShared()->getStr(getLocalId(id, pos));
		else if (id <= Tsh + Gsh)
			return getGraphsDictionaryPtr()->getShared()->getStr(getLocalId(id, pos));
		else if (id <= Tsh + Gsh + Tsubj && pos==SUBJECT)
			return getTriplesDictionaryPtr()->getSubjects()->getStr(getLocalId(id, pos));
		else if (id <= Tsh + Gsh + Tsubj + Gsubj && pos==SUBJECT)
			return getGraphsDictionaryPtr()->getSubjects()->getStr(getLocalId(id, pos));
		else if (id <= Tsh + Gsh + Tsubj + Gsubj + Tobj && pos==OBJECT)
		{
			return getTriplesDictionaryPtr()->getObjects()->getStr(getLocalId(id, pos));
		}
		else if (id <= Tsh + Gsh + Tsubj + Gsubj + Tobj + Gobj && pos==OBJECT)
			return getGraphsDictionaryPtr()->getObjects()->getStr(getLocalId(id, pos));
		else
			throw std::runtime_error("Incorrect id for subject/object");
	}
	else if (pos==PREDICATE)
		return getTriplesDictionaryPtr()->getPredicates()->getStr(getLocalId(id, pos));
	else if (pos==GRAPH)
		return getGraphsDictionaryPtr()->getGraphs()->getStr(getLocalId(id, pos));

	return string("");

}

unsigned int BaseReificationDictionary::getMapping()const{
	return MAPPING1;
}

void BaseReificationDictionary::import(Dictionary *other, ProgressListener *listener/*=NULL*/){
	if(BaseReificationDictionary* reif_dict = dynamic_cast<BaseReificationDictionary*>(other))
		import(reif_dict, listener);
	else
		throw std::logic_error("Dictionary to import should be a BaseReificationDictionary");
}
void BaseReificationDictionary::import(BaseReificationDictionary *other, ProgressListener *listener/*=NULL*/)
{
	//Dictionary* trDict = other->getTriplesDictionaryPtr();
	getTriplesDictionaryPtr()->import(other->getTriplesDictionaryPtr());

	//Dictionary* grDict = other->getGraphsDictionaryPtr();
	getGraphsDictionaryPtr()->import(other->getGraphsDictionaryPtr());
}


void BaseReificationDictionary::populateHeader(Header &header, std::string rootNode)
{throw std::logic_error("Not implemented");}



size_t BaseReificationDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/)
{throw std::logic_error("Not implemented");}

void BaseReificationDictionary::getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{throw std::logic_error("Not implemented");}

IteratorUCharString *BaseReificationDictionary::getSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}

IteratorUInt *BaseReificationDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}


void BaseReificationDictionary::load(std::istream & input, ControlInformation &ci, ProgressListener *listener)
{

	Dictionary* t_dict = getTriplesDictionaryPtr();
	Dictionary* g_dict = getGraphsDictionaryPtr();

	ci.load(input);
	delete t_dict ; t_dict=NULL;
	t_dict = HDTFactory::readDictionary(ci);
	t_dict->load(input, ci, listener);

	ci.load(input);
	delete g_dict ; g_dict=NULL;
	g_dict = HDTFactory::readDictionary(ci);
	g_dict->load(input, ci, listener);

	maxTriplesDictID = t_dict->getMaxID();
}

void BaseReificationDictionary::save(std::ostream &output, ControlInformation &ci, ProgressListener *listener/*=NULL*/){
	const ControlInformation ci_original = ControlInformation(ci);
	
	Dictionary* t_dict = getTriplesDictionaryPtr();
	Dictionary* g_dict = getGraphsDictionaryPtr();

	ci.setFormat(getType());
	ci.save(output);

	ci = ci_original;
	t_dict->save(output,ci);

	ci = ci_original;
	g_dict->save(output,ci);
}

}
