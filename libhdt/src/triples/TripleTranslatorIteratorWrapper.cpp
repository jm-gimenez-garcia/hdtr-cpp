#include "TripleTranslatorIteratorWrapper.hpp"
#include "SingleQuad.hpp"
#include "Triples.hpp"

using namespace hdt;

TripleTranslatorIteratorWrapper::TripleTranslatorIteratorWrapper(IteratorTripleID* it_tid):iterator_tid(it_tid){}
TripleTranslatorIteratorWrapper::TripleTranslatorIteratorWrapper(IteratorTripleID* it_tid, std::function<unsigned int (unsigned int,TripleComponentRole)> toGlobId):iterator_tid(it_tid){toGlobalID = toGlobId;}

TripleTranslatorIteratorWrapper::TripleTranslatorIteratorWrapper(Triples* triples, IteratorTripleID* it_tid):iterator_tid(it_tid){toGlobalID = triples->getToGlobalIDFunction();}


void TripleTranslatorIteratorWrapper::convertToRoleIDs(TripleID& tid){
	const unsigned int subj = toRoleID(tid.getSubject(), SUBJECT);
	const unsigned int pred = toRoleID(tid.getPredicate(), PREDICATE);
	const unsigned int obj = toRoleID(tid.getObject(), OBJECT);
	const QuadID qid(tid.to_QuadID());
	if(qid.hasGraph()){
		const unsigned int gr = toRoleID(qid.getGraph(), GRAPH);
		tid = QuadID(subj, pred, obj, gr);
	}
	else
		tid = TripleID(subj, pred, obj);
}

void TripleTranslatorIteratorWrapper::convertToGlobalIDs(TripleID& tid){
	const unsigned int subj = toGlobalID(tid.getSubject(), SUBJECT);
	const unsigned int pred = toGlobalID(tid.getPredicate(), PREDICATE);
	const unsigned int obj = toGlobalID(tid.getObject(), OBJECT);
	const QuadID qid(tid.to_QuadID());
	if(qid.hasGraph()){
		const unsigned int gr = toGlobalID(qid.getGraph(), GRAPH);
		tid = QuadID(subj, pred, obj, gr);
	}
	else
		tid = TripleID(subj, pred, obj);
}

