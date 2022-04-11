#include "BaseReificationDictionary.hpp"
#include "TriplesFourSectionDictionary.hpp"
#include "GraphsFourSectionDictionary.hpp"
#include <HDTSpecification.hpp>
#include "../hdt/HDTFactory.hpp"

namespace hdt{

BaseReificationDictionary::BaseReificationDictionary(){toGlobalID=roleIdToGlobalId; toRoleID=globalIdToRoleId;}
BaseReificationDictionary::BaseReificationDictionary(HDTSpecification &spec){
	toGlobalID=roleIdToGlobalId;
	toRoleID=globalIdToRoleId;

	string map ="";
	try{
		map = spec.get("dictionary.mapping");
	}catch(exception& e){}
	if(map=="mapping1") {
		this->mapping = MAPPING1;
	} else {
		this->mapping = MAPPING2;
	}
	}

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
	// getGlobalId(MAPPING1, id, pos);
	return getGlobalId(this->getMapping(), id, pos);
}

//locid is the local ID in the Triples/Graphs Dictionary ; the return value is the global ID in the reification dictionary 
// mapping_type is not used ; only the mapping of TriplesDictionary or GraphsDictionary is used
unsigned int BaseReificationDictionary::getGlobalId(unsigned int mapping_type, unsigned int locid, DictionarySection pos)const{
	unsigned int ret;
	switch(pos){
		case SHARED:
		case SHARED_SUBJECTS:
		case SHARED_OBJECTS:
			if (locid<=Tsh)
				ret=locid;
			else
				ret=locid+Tsh;
			break;
		case SHARED_TRIPLES:
		case SHARED_SUBJECTS_TRIPLES:
		case SHARED_OBJECTS_TRIPLES:
		case NOT_SHARED_PREDICATES:
			ret=locid;
			break;
		case SHARED_GRAPHS:
		case SHARED_SUBJECTS_GRAPHS:
		case SHARED_OBJECTS_GRAPHS:
			ret=Tsh;
			break;
		case NOT_SHARED_SUBJECTS:
			if (locid<=Tsubj)
				ret=locid+Tsh+Gsh;
			else
				ret=locid+Tsh+Gsh+Tsubj;
			break;
		case NOT_SHARED_SUBJECTS_TRIPLES:
			ret=locid+Tsh+Gsh;
			break;
		case NOT_SHARED_SUBJECTS_GRAPHS:
			ret=locid+Tsh+Gsh+Tsubj;
			break;
		case NOT_SHARED_OBJECTS:
			if (locid<=Tobj) {
				if (mapping_type==MAPPING2) {
					ret=locid+Tsh+Gsh;
				} else if (mapping_type==MAPPING1) {
					ret=locid+Tsh+Gsh+Tsubj+Gsubj;
				} else throw std::logic_error("Unkown type of mapping");
			}
			else {
				if (mapping_type==MAPPING2) {
				ret=locid+Tsh+Gsh+Tobj;
			} else if (mapping_type==MAPPING1) {
				ret=locid+Tsh+Gsh+Tsubj+Gsubj+Tobj;
			} else throw std::logic_error("Unkown type of mapping");
			}
			break;
		case NOT_SHARED_OBJECTS_TRIPLES:
			if (mapping_type==MAPPING2) {
				ret=locid+Tsh+Gsh;
			} else if(mapping_type==MAPPING1) {
				ret=locid+Tsh+Gsh+Tsubj;
			} else throw std::logic_error("Unkown type of mapping");
			break;
		case NOT_SHARED_OBJECTS_GRAPHS:
			if (mapping_type==MAPPING2) {
				ret=locid+Tsh+Gsh+Tobj;
			} else if (mapping_type==MAPPING1) {
				ret=locid+Tsh+Gsh+Tsubj+Gsubj+Tobj;
			} else throw std::logic_error("Unkown type of mapping");
			break;
		case GRAPHS:
		case GRAPHS_SHARED:
			if (mapping_type==MAPPING2) ret=locid;
			else if (mapping_type==MAPPING1) ret=locid+Tsh;
			else throw std::logic_error("Unkown type of mapping");
			break;
		case GRAPHS_SUBJECTS:
			if (mapping_type==MAPPING2) ret=locid+Gsh;
			else if (mapping_type==MAPPING1) ret=locid+Tsh+Gsh+Tsubj;
			else throw std::logic_error("Unkown type of mapping");
			break;
		case GRAPHS_OBJECTS:
			if (mapping_type==MAPPING2) ret=locid+Gsh+Gsubj;
			else if (mapping_type==MAPPING1) ret=locid+Tsh+Gsh+Tsubj+Gsubj+Tobj;
			else throw std::logic_error("Unkown type of mapping");
			break;
		case GRAPHS_UNUSED:
			if (mapping_type==MAPPING2) ret=locid+Gsh+Gsubj+Gobj;
			else if (mapping_type==MAPPING1) ret=locid+Tsh+Gsh+Tsubj+Gsubj+Tobj+Gobj;
			else throw std::logic_error("Unkown type of mapping");
			break;
		default:
			throw std::logic_error("Unkown type of DictionarySection");
	}

	return 0;
}

// id is the global ID in the ReificationDictionary and the return value is the local id in the Triples/Graphs Dictionary
unsigned int BaseReificationDictionary::getLocalId(unsigned int id, TripleComponentRole position)const
{
	return getLocalId(this->getMapping(), id, position);
}


// unsigned int BaseReificationDictionary::getTrGrGlobalIdFromGlobalId(unsigned int globid, TripleComponentRole position)const
// {
// 	if(globid==0)
// 		return 0;

// 	switch(position){
// 		case SUBJECT:
// 			if(globid <= Tsh)
// 				return globid;
// 			else if (globid <= Tsh + Gsh)
// 				return globid-Tsh;
// 			else if (globid <= Tsh + Gsh + Tsubj)
// 				return globid-Gsh;
// 			else if (globid <= Tsh + Gsh + Tsubj + Gsubj)
// 				return globid-Tsh-Tsubj;
// 			else
// 				throw std::logic_error("Id too high to be a subject");
// 			break;
// 		case OBJECT:
// 			if(globid <= Tsh)
// 				return globid;
// 			else if (globid <= Tsh + Gsh)
// 				return globid-Tsh;
// 			if(globid <= Tsh + Gsh + Tsubj + Gsubj || globid > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj)
// 			{
				
// 				cerr << "globid = " << globid <<endl; 
// 				cerr << "Tsh + Gsh + Tsubj + Gsubj = " << Tsh + Gsh + Tsubj + Gsubj<< endl;
// 				cerr << " Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj = " << Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj << endl;
// 				throw std::logic_error("Id doesn't correspond to an object");
// 				return 0;
// 			}
// 			if (globid <= Tsh + Gsh + Tsubj + Gsubj +Tobj) // if object in triplesdictionary
// 			{
// 				if (getTriplesDictionaryPtr()->getMapping() == MAPPING1)
// 					return globid - Gsh - Gsubj;
// 				else if (getTriplesDictionaryPtr()->getMapping() == MAPPING2)
// 					return globid - Gsh - Tsubj - Gsubj;
// 				else
// 					throw std::logic_error("Unkown type of mapping");
// 			}
// 			else // if object in graphsdictionary 
// 			{
// 				if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
// 					return globid - Tsh - Tsubj - Tobj;
// 				else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
// 					return globid - Tsh - Tsubj - Gsubj - Tobj;
// 				else
// 					throw std::logic_error("Unkown type of mapping");
// 			}
// 			break;
// 		case PREDICATE:
// 			if (globid <= getTriplesDictionaryPtr()->getNpredicates())
// 				return globid;
// 			else
// 				throw std::logic_error("Id too high to be a predicate");
// 			break;
// 		case GRAPH:
// 			const unsigned int Gun = getGraphsDictionaryPtr()->getNunused();
// 			if(globid <= Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj || globid > Tsh + Gsh + Tsubj + Gsubj +Tobj + Gobj + Gun)
// 			{
// 				throw std::logic_error("Id doesn't correspond to an unused graph");
// 				return 0;
// 			}
// 			if (getGraphsDictionaryPtr()->getMapping() == MAPPING1)
// 					return globid - Tsh - Tsubj - Tobj;
// 			else if  (getGraphsDictionaryPtr()->getMapping() == MAPPING2)
// 			{
// 				const unsigned int lower_subj_obj_size = (Gobj < Gsubj) ? Gobj : Gsubj ;
// 					return globid - Tsh - Tsubj - Tobj - lower_subj_obj_size;
// 			}
// 			else
// 				throw std::logic_error("Unkown type of mapping");
// 			break;
// 	}
// 	return 0;
// }


unsigned int BaseReificationDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const
{
	// cout << "we reach here" << endl;
	switch(position){
		case SUBJECT:
		// cout << "subject" << endl;
			if(id > 0 && id <= Tsh)
				return id;
			else if (id <= Tsh + Gsh)
				return id-Tsh;
			else if (id <= Tsh + Gsh + Tsubj)
				return id-Tsh-Gsh;
			else if (id <= Tsh + Gsh + Tsubj + Gsubj)
				return id-Tsh-Gsh-Tsubj;
			else
				throw std::logic_error("getLocalId:Id too high to be a subject");
			break;
		case OBJECT:
		// cout << "object=" << id << endl;
			if(id <= Tsh)
				return id;
			else if (id <= Tsh + Gsh)
				return id-Tsh;
			else if (mapping == MAPPING2) {
				if (id <= Tsh + Gsh + Tobj)
					return id-Tsh-Gsh;
				else if (id <= Tsh + Gsh + Tobj + Gobj)
					return id-Tsh-Gsh-Tobj;
			}
			else if (mapping == MAPPING1) {
				if (id <= Tsh + Gsh + Tsubj + Tobj)
					return id-Tsh-Gsh-Tsubj;
				else if (id <= Tsh + Gsh + Tsubj + Tobj + Gsubj + Gobj)
					return id-Tsh-Gsh-Tsubj-Tobj-Gsubj;
			} else {
				throw std::logic_error("getLocalId:Unkown type of mapping");
			}
		case PREDICATE:
		// cout << "predicate" << endl;
			if (id <= getTriplesDictionaryPtr()->getNpredicates())
				return id;
			else
				throw std::logic_error("getLocalId:Id too high to be a predicate");
			break;
		case GRAPH:
		// cout << "graph" << endl;
			if (mapping == MAPPING2) {
				if(id > 0 && id <= Gsh)
					return id;
				else if (id <= Gsh + Gsubj)
					return id-Gsh;
				else if (id <= Gsh + Gsubj + Gobj)
					return id-Gsh-Gsubj;
				else if (id <= Gsh + Gsubj + Gobj + Gun)
					return id-Gsh-Gsubj-Gobj;
				else
					throw std::logic_error("getLocalId:Id too high to be a graph");
			} else if (mapping == MAPPING1) {
				if(id > 0 && id <= Tsh + Gsh)
					return id-Tsh;
				else if (id <= Tsh + Gsh + Tsubj+ Gsubj)
					return id-Tsh-Gsh-Tsubj;
				else if (id <= Tsh + Gsh + Tsubj + Gsubj + Tobj + Gobj)
					return id-Tsh-Gsh-Tsubj-Gsubj-Tobj;
				else if (id <= Tsh + Gsh + Tsubj + Gsubj + Tobj + Gobj + Gun)
					return id-Tsh-Gsh-Tsubj-Gsubj-Tobj-Gobj;
				else
					throw std::logic_error("getLocalId:Id too high to be a graph");
			} else
				throw std::logic_error("getLocalId:Unkown type of mapping");
			break;
	}
	return 0;
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
		bool sub_dict_is_triples;
		
        switch (position) {
			case SUBJECT:
                ret = getTriplesDictionaryPtr()->stringToId(key, position);
				if( ret == 0) {
					ret = getGraphsDictionaryPtr()->stringToId(key,position);
					if (ret != 0) {
						if (ret <= Gsh) ret += Tsh;
						else if (ret <= Gsh+Gsubj) ret += Tsh+Tsubj;
						else throw std::logic_error("stringToId: ID " + std::to_string(ret) + " too high to be a subject.");
					} 
				}
				else if (ret <= Tsh); //do nothing, ret is already the correct ID
				else if (ret <= Tsh+Tsubj) ret += Gsh;
				else throw std::logic_error("stringToId: Subject ID " + std::to_string(ret) + " not found.");
				break;
			case OBJECT:
                ret = getTriplesDictionaryPtr()->stringToId(key, position);
				if( ret == 0) {
					ret = getGraphsDictionaryPtr()->stringToId(key,position);
					if (ret != 0) {
						if (ret <= Gsh) ret += Tsh;
						else if (this->getMapping() == MAPPING2 && ret <= Gsh+Gobj) ret += Tsh+Tobj;
						else if (this->getMapping() == MAPPING1 && ret <= Gsh+Gsubj+Gobj) ret += Tsh+Tsubj+Tobj;
						else throw std::logic_error("stringToId: ID " + std::to_string(ret) + " too high to be an object.");
					}
				}
				else if (ret <= Tsh); //do nothing, ret is already the correct ID
				else if (this->getMapping() == MAPPING2 && ret <= Tsh+Tobj) ret += Gsh;
				else if (this->getMapping() == MAPPING1 && ret <= Tsh+Tsubj+Tobj) ret += Gsh+Gsubj;
				else throw std::logic_error("stringToId: Object ID " + std::to_string(ret) + " not found.");
				break;
			case PREDICATE:
                ret = getTriplesDictionaryPtr()->stringToId(key, position);
				break;
			case GRAPH:
				if (this->getMapping()==MAPPING2) ret = getGraphsDictionaryPtr()->stringToId(key, position);
				else if (this->getMapping() == MAPPING1) {
					ret = getGraphsDictionaryPtr()->stringToId(key,position);
					if(ret <= Gsh) ret += Tsh;
					else if (ret <= Gsh + Gsubj) ret += Tsh+Tsubj;
					// else if (ret <= Gsh + Gsubj + Gobj) ret += Tsh+Tsubj+Tobj;
					else if (ret <= Gsh + Gsubj + Gobj + Gun) ret += Tsh+Tsubj+Tobj;
					else
						throw std::logic_error("stringToId: Graph ID " + std::to_string(ret) + " not found.");
				} else throw std::logic_error("idToString:Unkown type of mapping");
				break;
			default:
                throw std::logic_error("stringToId: Unrecognised role");
				break;
        }
	return ret;
	// return getGlobalIdFromTrGrGlobalId(ret, position, sub_dict);	
}


std::string BaseReificationDictionary::idToString(const unsigned int id, const TripleComponentRole pos)const
{
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
	{
		if ( (id>Tsh && id<=Tsh+Gsh) || (id>Tsh+Gsh+Tsubj && id<=Tsh+Gsh+Tsubj+Gsubj) )
			return idToString(id, SUBJECT);
		else if (id>Tsh+Gsh+Tsubj+Gsubj+Tobj && id<=Tsh+Gsh+Tsubj+Gsubj+Tobj+Gobj)
			return idToString(id, OBJECT);
		else if (id>Tsh+Gsh+Tsubj+Gsubj+Tobj+Gobj && id<=Tsh+Gsh+Tsubj+Gsubj+Tobj+Gobj+Gun)
			return getGraphsDictionaryPtr()->getGraphs()->getStr(getLocalId(id, pos));
		//return getGraphsDictionaryPtr()->getGraphs()->getStr(getLocalId(id, pos));
	}

	return string("");

}

unsigned int BaseReificationDictionary::getMapping()const{
	return this->mapping;
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

	initValues();
}



size_t BaseReificationDictionary::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener/*=NULL*/)
{
    size_t count=0;
    IntermediateListener iListener(listener);
 
	loadControlInfo(ptr, ptrMax, count);

	count += getTriplesDictionaryPtr()->load(&ptr[count], ptrMax);
	count += getGraphsDictionaryPtr()->load(&ptr[count], ptrMax);

	initValues();

	return count;
}

void BaseReificationDictionary::getSuggestions(const char *base, TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{throw std::logic_error("Not implemented");}

IteratorUCharString *BaseReificationDictionary::getSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}

IteratorUInt *BaseReificationDictionary::getIDSuggestions(const char *prefix, TripleComponentRole role)
{throw std::logic_error("Not implemented");}


void BaseReificationDictionary::load(std::istream & input, ControlInformation &ci, ProgressListener *listener)
{
	loadControlInfo(input, ci);

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

	initValues();
}

void BaseReificationDictionary::loadControlInfo(istream & input, ControlInformation & ci)
{
	string format = ci.getFormat();
	if(format!=getType()) {
		throw runtime_error("Trying to read a BaseFourSectionDictionary but the data is not BaseFourSectionDictionary");
	}
	this->mapping = ci.getUint("mapping");
	this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseReificationDictionary::loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count){
    ControlInformation ci;
    count += ci.load(&ptr[count], ptrMax);

    this->mapping = ci.getUint("mapping");
    this->sizeStrings = ci.getUint("sizeStrings");
}

void BaseReificationDictionary::save(std::ostream &output, ControlInformation &ci, ProgressListener *listener/*=NULL*/){
	const ControlInformation ci_original = ControlInformation(ci);
	
	saveControlInfo(output, ci);

	Dictionary* t_dict = getTriplesDictionaryPtr();
	Dictionary* g_dict = getGraphsDictionaryPtr();

	// ci.setFormat(getType());
	// ci.save(output);

	ci = ci_original;
	t_dict->save(output,ci);

	ci = ci_original;
	g_dict->save(output,ci);
}

void BaseReificationDictionary::saveControlInfo(ostream& output, ControlInformation & controlInformation)
{
	controlInformation.setFormat(getType());
	controlInformation.setUint("mapping", this->mapping);
	controlInformation.setUint("sizeStrings", this->sizeStrings);
	controlInformation.save(output);
}

void BaseReificationDictionary::initValues(){
	maxTriplesDictID = getTriplesDictionaryPtr()->getMaxID();
	max_subj_id=getGraphsDictionaryPtr()->getNsubjects();
	max_obj_id=getGraphsDictionaryPtr()->getNobjects();
	Tsh = getTriplesDictionaryPtr()->getNshared();
	Gsh = getGraphsDictionaryPtr()->getNshared();
	Tsubj = getTriplesDictionaryPtr()->getNsubjects() - getTriplesDictionaryPtr()->getNshared();
	Gsubj = getGraphsDictionaryPtr()->getNsubjects() - getGraphsDictionaryPtr()->getNshared();
	Tobj = getTriplesDictionaryPtr()->getNobjects() - getTriplesDictionaryPtr()->getNshared();
	Gobj = getGraphsDictionaryPtr()->getNobjects() - getGraphsDictionaryPtr()->getNshared();
	Gun = getGraphsDictionaryPtr()->getNunused();
	min_sub_obj_length = (Gobj>Gsubj) ? Gsubj : Gobj;
	max_sub_obj_length = (Gobj>Gsubj) ? Gobj : Gsubj;
	last_comm_subj_obj_id = (max_subj_id>max_obj_id) ? max_obj_id:max_subj_id;
	
	// cout << "maxTriplesDictID=" << maxTriplesDictID << endl;
	// cout << "max_subj_id=" << max_subj_id << endl;
	// cout << "max_obj_id=" << max_obj_id << endl;
	// cout << "Tsh=" << Tsh << endl;
	// cout << "Gsh=" << Gsh << endl;
	// cout << "Tsubj=" << Tsubj << endl;
	// cout << "Gsubj=" << Gsubj << endl;
	// cout << "Tobj=" << Tobj << endl;
	// cout << "Gobj=" << Gobj << endl;
	// cout << "Gun=" << Gun << endl;
	// cout << "min_sub_obj_length=" << min_sub_obj_length << endl;
	// cout << "max_sub_obj_length=" << max_sub_obj_length << endl;
	// cout << "last_comm_subj_obj_id=" << last_comm_subj_obj_id << endl;
	// cout << "Mapping=" << this->getMapping() << endl;
	// cout << "Triples Mapping=" << getTriplesDictionaryPtr()->getMapping() << endl;
	// cout << "Graphs Mapping=" << getGraphsDictionaryPtr()->getMapping() << endl;
}

}