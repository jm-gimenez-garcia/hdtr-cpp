#include "TriplesDictionaryLoader.hpp"
#include <SingleTriple.hpp>
#include <SingleQuad.hpp>
#include <HDTListener.hpp>
#include "../dictionary/ModifiableTriplesDictionary.hpp"
using namespace hdt;

TriplesDictionaryLoader::TriplesDictionaryLoader(ModifiableTriplesDictionary* triples_dict, ProgressListener *list) : DictionaryLoader(list), trDict(triples_dict) { }

ModifiableTriplesDictionary* TriplesDictionaryLoader::getDictionary()
{return trDict;}
