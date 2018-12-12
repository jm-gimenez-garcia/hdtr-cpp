#ifndef MODIFIABLE_GRAPHS_DICTIONARY_H_
#define MODIFIABLE_GRAPHS_DICTIONARY_H_
#include "Dictionary.hpp"
#include "GraphsDictionary.hpp"
namespace hdt{
	class ModifiableGraphsDictionary : virtual public ModifiableDictionary, virtual public GraphsDictionary{
	};
}

#endif
