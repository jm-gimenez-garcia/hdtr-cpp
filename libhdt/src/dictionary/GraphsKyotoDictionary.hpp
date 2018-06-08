#ifndef __GRAPHS_KYOTO_DICTIONARY_H__
#define __GRAPHS_KYOTO_DICTIONARY_H__


namespace hdt {

class GraphsKyotoDictionary: public BaseKyotoDictionary {
public:
	TreeDB graphs;

public:
	GraphsKyotoDictionary(){};
	virtual ~GraphsKyotoDictionary();

    IteratorUCharString *getGraphs()const;
	size_t getNumberOfElements();
	unsigned int getNgraphs()const;
	unsigned int getMaxGraphID()const;

	unsigned int stringToId(const std::string &key, const TripleComponentRole position)const;

	void startProcessing(ProgressListener *listener);
	unsigned int insert(const std::string & str, const TripleComponentRole pos);

	unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const;
	vunsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const

	void dumpSizes(std::ostream &out);










};

}












#endif
