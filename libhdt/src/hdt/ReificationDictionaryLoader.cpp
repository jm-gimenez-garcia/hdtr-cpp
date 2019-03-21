#include "ReificationDictionaryLoader.hpp"
#include <SingleTriple.hpp>
#include <SingleQuad.hpp>
#include <HDTListener.hpp>
#include "../dictionary/ModifiableReificationDictionary.hpp"
#include "../dictionary/ModifiableTriplesDictionary.hpp"
using namespace hdt;

ReificationDictionaryLoader::ReificationDictionaryLoader(ModifiableReificationDictionary *reif_dictionary, ProgressListener *list) : DictionaryLoader(list), reifDict(reif_dictionary) { }

ModifiableTriplesDictionary* ReificationDictionaryLoader::getDictionary()
{return reifDict;}

