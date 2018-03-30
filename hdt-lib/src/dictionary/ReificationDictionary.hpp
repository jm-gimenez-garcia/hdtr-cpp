#ifndef HDT_REIFICATION_DICTIONARY_HPP_
#define HDT_REIFICATION_DICTIONARY_HPP_

namespace hdt{

class ReificationDictionary : public TriplesDictionary, public GraphsDictionary{
private:
	TriplesDictionary* triplesDict;
	GraphsDictionary* graphsDict;

public:
	ReificationDictionary(HDTSpecification &specification);
private:
	unsigned int maxTriplesDictID; 

};

}

#endif
