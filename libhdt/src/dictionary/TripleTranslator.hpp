#ifndef __TRIPLETRANSLATOR_HPP__
#define __TRIPLETRANSLATOR_HPP__
#include <functional>
#include "HDTEnums.hpp"

namespace hdt{

class TripleTranslator{
	protected:
		std::function<unsigned int(unsigned int,TripleComponentRole)> toGlobalID;
		std::function<unsigned int(unsigned int,TripleComponentRole)> toRoleID;
	public:
		TripleTranslator():toGlobalID(nullptr),toRoleID(nullptr){}
		std::function<unsigned int(unsigned int,TripleComponentRole)> getToGlobalIDFunction(){return toGlobalID;}
		std::function<unsigned int(unsigned int,TripleComponentRole)> getToRoleIDFunction(){return toRoleID;}
		void setToGlobalIDFunction(std::function<unsigned int(unsigned int,TripleComponentRole)> fcn){toGlobalID=fcn;}
		void setToRoleIDFunction(std::function<unsigned int(unsigned int,TripleComponentRole)> fcn){toRoleID=fcn;}
		virtual ~TripleTranslator(){}
};

}
#endif
