#ifndef __TRIPLESTRANSLATOR_HPP__
#define __TRIPLESTRANSLATOR_HPP__

namespace hdt{

class TripleTranslator {
	protected:
		unsigned int (*toRoleId)(unsigned int global_id, TripleComponentRole position)const;
		unsigned int (*toGlobalId)(unsigned int role_id, TripleComponentRole position)const;
	public:
		TripleTranslator():toRoleId(NULL),toGlobalId(NULL){}
		~TripleTranslator(){}
};
}


#endif
