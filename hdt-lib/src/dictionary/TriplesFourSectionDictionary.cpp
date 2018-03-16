namespace hdt {


TriplesFourSectionDictionary::TriplesFourSectionDictionary() 
{
	predicates = new csd::CSD_PFC();
}

TriplesFourSectionDictionary::TriplesFourSectionDictionary(HDTSpecification & spec)
{
	predicates = new csd::CSD_PFC();
}

TriplesFourSectionDictionary::~TriplesFourSectionDictionary()
{
	delete predicates;
}

unsigned int TriplesFourSectionDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{
	if (position!=PREDICATE)
		return ThreeSectionDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
        	if(key.length()==0)
			return 0;
		ret = predicates->locate((const unsigned char *)key.c_str(), key.length());
		if(ret!=0) 
			return getGlobalId(ret, NOT_SHARED_PREDICATE);
        	return 0;
	}
}


void TriplesFourSectionDictionary::loadFourthSection((std::istream & input, const IntermediateListener& iListener){

	iListener.setRange(50,75);
	iListener.notifyProgress(0, "Dictionary read predicates.");
	delete predicates;
	predicates = csd::CSD::load(input);
	if(predicates==NULL){
		predicates = new csd::CSD_PFC();
		throw std::runtime_error("Could not read predicates.");
	}
	predicates = new csd::CSD_Cache2(predicates);
}

void TriplesFourSectionDictionary::loadFourthSection(unsigned char *ptr, int& count, const IntermediateListener& iListener){

    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read predicates.");
    delete predicates;
    predicates = csd::CSD::create(ptr[count]);
    if(predicates==NULL){
        predicates = new csd::CSD_PFC();
        throw std::runtime_error("Could not read predicates.");
    }
    count += predicates->load(&ptr[count], ptrMax);
    predicates = new csd::CSD_Cache2(predicates);

}

void ThreeSectionDictionary::importFourthSection(Dictionary *other, IntermediateListener& iListener) {
	try {
		NOTIFY(listener, "DictionaryPFC loading predicates", 25, 30);
		iListener.setRange(20, 21);
		IteratorUCharString *itPred = other->getPredicates();
		delete predicates;
		predicates = loadSection(itPred, blocksize, &iListener);
		delete itPred;
	} catch (std::exception& e) {
		delete predicates;
		predicates = new csd::CSD_PFC();
		throw;
	}
}

IteratorUCharString *TriplesFourSectionDictionary::getPredicates()const {
	return predicates->listAll();
}

void TriplesFourSectionDictionary::saveFourthSection(std::ostream & output, IntermediateListener& listener){

	iListener.setRange(45,60);
	iListener.notifyProgress(0, "Dictionary save predicates.");
	predicates->save(output);
}
unsigned int TriplesFourSectionDictionary::getNpredicates()const
{return predicates->getLength();}
unsigned int TriplesFourSectionDictionary::getMaxPredicateID()const
{return predicates->getLength();}

unsigned int TriplesFourSectionDictionary::getFourthSectionLength()const{
return predicates->getLength();
}
unsigned int TriplesFourSectionDictionary::getFourthSectionSize()const{
return predicates->getSize();
}



csd::CSD *TriplesFourSectionDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
	return (position==PREDICATE) ? predicates : ThreeSectionDictionary::getDictionarySection(id, position);
}

unsigned int TriplesFourSectionDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	return (position==NOT_SHARED_PREDICATE) ? id : ThreeSectionDictionary::getGlobalId(mapping, id, position);
}
unsigned int TriplesFourSectionDictionary::getGlobalId(unsigned int id, DictionarySection position)const
{return getGlobalId(mapping, id, position);}


unsigned int TriplesFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	return (position==PREDICATE) ? id : ThreeSectionDictionary::getLocalId(mapping, id, position);
}
unsigned int TriplesFourSectionDictionary::getLocalId(unsigned int id, TripleComponentRole position)const
{return getLocalId(mapping,id,position);}


void TriplesFourSectionDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{
	if(role==PREDICATE) 
		predicates->fillSuggestions(base, out, maxResults);
	else
		ThreeSectionDictionary::getSuggestions(base, role, out, maxResults);	
}


}
