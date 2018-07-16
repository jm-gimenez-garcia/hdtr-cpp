#include "GraphsDictionary.hpp"

using namespace hdt;

unsigned int GraphsDictionary::getMaxGraphID()const{
	const unsigned int max_subj_obj_id = (getMaxObjectID() > getMaxSubjectID()) ? getMaxObjectID() : getMaxSubjectID();
	return max_subj_obj_id + getNunused();
}

unsigned int GraphsDictionary::getNgraphs()const{
	return getNobjects() + getNsubjects() - getNshared() + getNunused();
}
