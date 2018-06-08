#ifndef __TRIPLES_KYOTO_DICTIONARY_H__
#define __TRIPLES_KYOTO_DICTIONARY_H__ 


namespace hdt {

class TriplesKyotoDictionary: public BaseKyotoDictionary {
public:
	TreeDB predicates;

public:
	TriplesKyotoDictionary(){};
	~TriplesKyotoDictionary();

    IteratorUCharString *getPredicates()const;
	size_t getNumberOfElements()const;
	unsigned int getNpredicates()const;
	unsigned int getMaxPredicateID()const;

	unsigned int stringToId(const std::string &key, const TripleComponentRole position)const;

	void startProcessing(ProgressListener *listener);
	unsigned int insert(const std::string & str, const TripleComponentRole pos);

	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const;
	unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const;

	void dumpSizes(std::ostream &out);







};
}















#endif
