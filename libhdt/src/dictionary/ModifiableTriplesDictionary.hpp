#ifndef MODIFIABLE_TRIPLES_DICTIONARY_H_
#define MODIFIABLE_TRIPLES_DICTIONARY_H_

#include <Dictionary.hpp>
#include "TriplesDictionary.hpp"

namespace hdt{
class ModifiableTriplesDictionary : virtual public ModifiableDictionary, virtual public TriplesDictionary{
};
}


#endif
