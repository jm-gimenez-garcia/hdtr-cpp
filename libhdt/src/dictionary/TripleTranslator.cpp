#include "TripleTranslator.hpp"
#include "SingleQuad.hpp"
using namespace std;
using namespace hdt;

void TripleTranslator::toRoleIDs(TripleID*& role_tid, const TripleID& glob_tid){
	if(role_tid == &glob_tid)
	{
		throw runtime_error("original global tripleID and local tripleID to set must be different");
		return;
	}
	if(role_tid)
		delete role_tid;

	const unsigned int subj = glob_tid.getSubject() > 0 ? toRoleID(glob_tid.getSubject(),SUBJECT) : 0;
	const unsigned int pred = glob_tid.getPredicate() > 0 ? toRoleID(glob_tid.getPredicate(),PREDICATE) : 0;
	const unsigned int obj = glob_tid.getObject() > 0 ? toRoleID(glob_tid.getObject(),OBJECT) : 0;
	if(glob_tid.hasGraph())
	{
		QuadID glob_qid= glob_tid.to_QuadID();
		const unsigned int gr = glob_qid.getGraph() > 0 ? toRoleID(glob_qid.getGraph(),GRAPH) : 0;
		role_tid = new QuadID(subj, pred, obj, gr);
	}
	else
	{
		role_tid = new TripleID(subj, pred, obj);
	}
		
}
