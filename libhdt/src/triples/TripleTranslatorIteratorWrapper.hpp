#ifndef __TRIPLE_TRANSLATOR_ITERATOR_WRAPPER_HPP__
#define __TRIPLE_TRANSLATOR_ITERATOR_WRAPPER_HPP__

#include "TripleTranslator.hpp"
#include "Iterator.hpp"
#include "HDTEnums.hpp"

namespace hdt{
class TripleID;
class Triples;

class TripleTranslatorIteratorWrapper : public TripleTranslator, public IteratorTripleID{

	private:
		IteratorTripleID* iterator_tid;

	public:
		TripleTranslatorIteratorWrapper(IteratorTripleID* it_tid);
		TripleTranslatorIteratorWrapper(IteratorTripleID* it_tid, std::function<unsigned int (unsigned int,TripleComponentRole)> toGlobId);
		TripleTranslatorIteratorWrapper(Triples* triples, IteratorTripleID* it_tid);

	// virtual methods inherited from IteratorTripleID
	public:
		bool hasNext();
		TripleID *next();
		bool hasPrevious();
		TripleID *previous();
		void goToStart();
		size_t estimatedNumResults();
		ResultEstimationType numResultEstimation();
		bool canGoTo();
		void goTo(unsigned int pos);
		void skip(unsigned int pos);
		bool findNextOccurrence(unsigned int value, unsigned char component);
		TripleComponentOrder getOrder();
		bool isSorted(TripleComponentRole role);

	// new methods
	private:
		void convertToRoleIDs(TripleID& tid);
		void convertToGlobalIDs(TripleID& tid);

};

inline bool TripleTranslatorIteratorWrapper::hasNext()
{return iterator_tid->hasNext();}

inline TripleID* TripleTranslatorIteratorWrapper::next()
{TripleID* tid = iterator_tid->next();convertToGlobalIDs(*tid); return tid; }

inline bool TripleTranslatorIteratorWrapper::hasPrevious()
{return iterator_tid->hasPrevious();}

inline TripleID* TripleTranslatorIteratorWrapper::previous()
{TripleID* tid = iterator_tid->previous();convertToGlobalIDs(*tid); return tid; }

inline void TripleTranslatorIteratorWrapper::goToStart()
{return iterator_tid->goToStart();}

inline size_t TripleTranslatorIteratorWrapper::estimatedNumResults()
{return iterator_tid->estimatedNumResults();}

inline ResultEstimationType TripleTranslatorIteratorWrapper::numResultEstimation()
{return iterator_tid->numResultEstimation();}

inline bool TripleTranslatorIteratorWrapper::canGoTo()
{return iterator_tid->canGoTo();}

inline void TripleTranslatorIteratorWrapper::goTo(unsigned int pos)
{return iterator_tid->goTo(pos);}

inline void TripleTranslatorIteratorWrapper::skip(unsigned int pos)
{return iterator_tid->skip(pos);}

inline bool TripleTranslatorIteratorWrapper::findNextOccurrence(unsigned int value, unsigned char component)
{return iterator_tid->findNextOccurrence(value, component);}

inline TripleComponentOrder TripleTranslatorIteratorWrapper::getOrder()
{return iterator_tid->getOrder();}

inline bool TripleTranslatorIteratorWrapper::isSorted(TripleComponentRole role)
{return iterator_tid->isSorted(role);}


}


#endif
