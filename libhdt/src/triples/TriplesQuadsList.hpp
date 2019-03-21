#ifndef TRIPLESQUADSLIST_HPP_
#define TRIPLESQUADSLIST_HPP_

#include <Triples.hpp>
#include "TripleIterators.hpp"
#include "TriplesList.hpp"
#include <SingleQuad.hpp>
#include "QuadsList.hpp"

namespace hdt{
class HDTSpecification;

class TriplesQuadsList : public ModifiableTriples {
	private:
		TriplesList tList;
		QuadsList qList;
	public :
		TriplesQuadsList();
		TriplesQuadsList(HDTSpecification &spec);
		TriplesQuadsList(const TriplesList& triplesList, const QuadsList& quadsList);
		TriplesQuadsList(const std::vector<TripleID>& tripleArray, const std::vector<QuadID>& quadArray, const TripleComponentOrder& ord);
		~TriplesQuadsList(){}


	//From Triples
	public :
		IteratorTripleID *search(TripleID &pattern);
		IteratorTripleID *searchJoin(TripleID &a, TripleID &b, unsigned short conditions);
		float cost(TripleID &triple) const{throw std::logic_error("Not Implemented");return 0.0f;}
		size_t getNumberOfElements() const;
		size_t size() const;
		void save(std::ostream &output, ControlInformation &ctrlInfo, ProgressListener *listener = NULL);
		void load(std::istream &input, ControlInformation &ctrlInfo, ProgressListener *listener);
		size_t load(unsigned char *char_ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
		void load(ModifiableTriples &input, ProgressListener *listener = NULL);
		void generateIndex(ProgressListener *listener){}
		void saveIndex(std::ostream &output, ControlInformation &controlInfo, ProgressListener *listener){}
		void loadIndex(std::istream &input, ControlInformation &controlInfo, ProgressListener *listener){}
		size_t loadIndex(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener){return 0;}
		bool isIndexed() const {return false;}
		void populateHeader(Header &header, string rootNode);
		void startProcessing(ProgressListener *listener=NULL){}
		void stopProcessing(ProgressListener *listener=NULL){}
		string getType() const;
		TripleComponentOrder getOrder() const;

	// From ModifiableTriples
	public :
		void insert(TripleID &triple);
		void insert(IteratorTripleID *triples);
		bool remove(TripleID &pattern);
		bool remove(IteratorTripleID *pattern);
		void sort(TripleComponentOrder ord, ProgressListener *listener = NULL);
		void removeDuplicates(ProgressListener *listener = NULL);
		void setOrder(TripleComponentOrder order);


	// New methods
	public:
		TriplesList* getTriplesList(){return &tList;}
		QuadsList* getQuadsList(){return &qList;}


};


class TriplesQuadsListIterator : public IteratorTripleID {
private:
	enum FutureLocation{
		TQLI_FUTURELOCATION_NONE,
		TQLI_FUTURELOCATION_IN_TRIPLESLIST,
		TQLI_FUTURELOCATION_IN_QUADSLIST
    };
	TriplesListIterator* tListIt;
	QuadsListIterator* qListIt;
	TripleID* futureNextTid;
	TripleID* futurePreviousTid;
	FutureLocation futureNext;
	FutureLocation futurePrevious;
	uint64_t posTriples;
	uint64_t posQuads;

public:
	TriplesQuadsListIterator();
	//TriplesQuadsListIterator(TriplesListIterator* tLIt, QuadsListIterator* qLIt);
	TriplesQuadsListIterator(TriplesList *triple_list, QuadsList *quad_list, TripleID& patt);
	TriplesQuadsListIterator(TriplesQuadsList *tq_list, TripleID& patt);
	~TriplesQuadsListIterator();
	// IteratorTripleID methods
	bool hasNext();
	TripleID* next();
	bool hasPrevious();
	TripleID* previous();
	void goToStart();
};



}

#endif
