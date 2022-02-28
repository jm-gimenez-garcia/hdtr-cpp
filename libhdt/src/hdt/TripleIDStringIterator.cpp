/*
 * TripleIDStringIterator.cpp
 *
 *  Created on: 11/08/2012
 *      Author: mck
 */


#include "TripleIDStringIterator.hpp"
#include "../util/StopWatch.hpp"

namespace hdt {

TripleIDStringIterator::TripleIDStringIterator(Dictionary *dict, IteratorTripleID *iterator) : dict(dict), iterator(iterator){
}

TripleIDStringIterator::~TripleIDStringIterator() {
	delete iterator;
}

bool TripleIDStringIterator::hasNext() {
	return iterator->hasNext();
}

TripleString *TripleIDStringIterator::next() {
	const TripleID *tid = iterator->next();
	if(tid->hasGraph())
	{
		const QuadID qid = tid->to_QuadID();
		// cout << tid->to_QuadID().getSubject() << "," << tid->to_QuadID().getPredicate() << ","  << tid->to_QuadID().getObject() << "," <<  tid->to_QuadID().getGraph() << endl;
		dict->quadIDtoQuadString(&qid, &result_qs);
		// cout << result_qs.getSubject() << "," << result_qs.getPredicate() << ","  << result_qs.getObject() << "," <<  result_qs.getGraph() << endl;
		return &result_qs;
	}
	else
	{
		// cout << tid->to_QuadID().getSubject() << "," << tid->to_QuadID().getPredicate() << ","  << tid->to_QuadID().getObject() << endl;
		dict->tripleIDtoTripleString(tid, &result_ts);
		// cout << result_ts.getSubject() << "," << result_ts.getPredicate() << ","  << result_ts.getObject() << endl;
		return &result_ts;
	}
}

bool TripleIDStringIterator::hasPrevious() {
	return iterator->hasPrevious();
}

TripleString *TripleIDStringIterator::previous() {
	const TripleID *tid = iterator->previous();
	if(tid->hasGraph())
	{
		const QuadID qid = tid->to_QuadID();
		dict->quadIDtoQuadString(&qid, &result_qs);
		return &result_qs;
	}
	else
	{
		dict->tripleIDtoTripleString(tid, &result_ts);
		return &result_ts;
	}
}

void TripleIDStringIterator::goToStart() {
	iterator->goToStart();
}
bool TripleIDStringIterator::canGoTo() {
	return iterator->canGoTo();
}
size_t TripleIDStringIterator::estimatedNumResults() {
	return iterator->estimatedNumResults();
}
ResultEstimationType TripleIDStringIterator::numResultEstimation() {
	return iterator->numResultEstimation();
}
void TripleIDStringIterator::skip(unsigned int pos) {
	iterator->skip(pos);
}

}

