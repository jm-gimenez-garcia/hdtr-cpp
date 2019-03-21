
//#include "CachedIterator.hpp"
#include <Iterator.hpp>

namespace hdt{

bool CachedIterator::hasNext() const
{itUCharStr->hasNext();}

unsigned char *CachedIterator::next() {
    unsigned char* cache_tmp = cachedValue;
    cachedValue = itUCharStr->next();
    return cache_tmp;
}

int CachedIterator::compare(const CachedIterator& cachedIt)const{
    return strcmp(cachedValue, cachedIt.getCurrent());
}


}
