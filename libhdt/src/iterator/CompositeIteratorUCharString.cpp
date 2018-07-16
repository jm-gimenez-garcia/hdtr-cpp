//#include "CompositeIteratorUCharString.hpp"
#include "Iterator.hpp"

namespace hdt{

bool cmpIteratorUCharString(const CachedIterator& c1, const CachedIterator& c2){return c1->compare(c2) <0;}


bool CompositeIteratorUCharString::hasNext()const{
	for (unsigned int i=0 ; i<vecItUCharStr.size() ; i++)
	{
		if (vecItUCharStr[i]->hasNext())
			return true;	
	}
	return false;
}

unsigned char *CompositeIteratorUCharString::next()
{
	return std::min(vecItUCharStr->begin(), vecItUCharStr->end(), cmpIteratorUCharString)->getCurrent();
}


}
