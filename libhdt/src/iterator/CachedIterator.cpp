
//#include "CachedIterator.hpp"
#include "Iterator.hpp"

namespace hdt{

bool CachedIterator::hasNext()
{itUCharStr->hasNext();}

unsigned char *next() {
	unsigned char* cache_tmp = cachedValue;
	cachedValue = itUCharStr->next();
	return cache_tmp;
}

int compare(const CachedIterator& cachedIt)const{
	return strcmp(cachedValue, cachedIt.getCurrent());
}


}
