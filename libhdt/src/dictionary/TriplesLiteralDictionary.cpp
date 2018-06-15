


TriplesLiteralDictionary::TriplesLiteralDictionary(){
	create();
}


TriplesLiteralDictionary::~TriplesLiteralDictionary()
{clear();}

void TriplesLiteralDictionary::clear()
{
	if (predicates!=NULL)
		{delete predicates; predicates=NULL;}	
}
void TriplesLiteralDictionary::create()
{
	clear();
	if (predicates==NULL)
		{predicates = new csd::CSD_PFC();}
}

void TriplesLiteralDictionary::clear_all()
{
	clear();
	BaseLiteralDictionary::clear();
}
void TriplesLiteralDictionary::create_all()
{
	clear_all();
	BaseLiteralDictionary::create();
	create();
}

unsigned int TriplesLiteralDictionary::stringToId(const std::string &key, const TripleComponentRole position)const {

	if (key.length() == 0) 
		return 0;

	if(position != PREDICATE)
		return BaseLiteralDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
		ret = predicates->locate((const unsigned char *) key.c_str(), key.length());
		if (ret != 0) 
			return getGlobalId(ret, NOT_SHARED_PREDICATE);
	}
    return 0;
}


void TriplesLiteralDictionary::loadFourthSection(std::istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50, 75);
	iListener.notifyProgress(0, "Dictionary read predicates.");

	delete predicates;
	predicates = csd::CSD::load(input);
	if (predicates == NULL) {
		predicates = new csd::CSD_PFC();
		throw std::runtime_error("Could not read predicates.");
	}
	predicates = new csd::CSD_Cache2(predicates);
	
}

void TriplesLiteralDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{
    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read predicates.");
    delete predicates;
    predicates = csd::CSD::create(ptr[count]);
    if(predicates==NULL){
        predicates = new csd::CSD_PFC();
        throw std::runtime_error("Could not read predicates.");
    }
    count += predicates->load(&ptr[count], ptrMax);
    predicates = new csd::CSD_Cache(predicates);
}

void TriplesLiteralDictionary::importFourthSection(Dictionary *other, IntermediateListener& iListener)
{
		//NOTIFY(listener, "DictionaryPFC loading predicates", 25, 30);
		IteratorUCharString *itPred = other->getPredicates();
		delete predicates;
		iListener.setRange(20, 21);
		predicates = loadSectionPFC(itPred, blocksize, &iListener);
		subjects = new csd::CSD_Cache2(subjects);
		delete itPred;
}
void TriplesLiteralDictionary::saveFourthSection(std::ostream & output, IntermediateListener& iListener);
{
	iListener.setRange(45, 60);
	iListener.notifyProgress(0, "Dictionary save predicates.");
	predicates->save(output);
}

IteratorUCharString *TriplesLiteralDictionary::getPredicates() {
	throw std::logic_error("Not implemented");
}



void TriplesLiteralDictionary::populateHeaderNumFourthSection(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_NUMPREDICATES, getNpredicates());}


void TriplesLiteralDictionary::populateHeaderMaxFourthSectionId(Header & header, string rootNode)
{header.insert(rootNode, HDTVocabulary::DICTIONARY_MAXPREDICATEID, getMaxPredicateID());}

uint64_t TriplesLiteralDictionary::size()
{return BaseLiteralDictionary::size() + predicates->getSize();}

unsigned int TriplesLiteralDictionary::getMaxID()
{return BaseLiteralDictionary::getMaxID();}


size_t TriplesLiteralDictionary::getNumberOfElements() {
	return BaseLiteralDictionary::getNumberOfElements() + predicates->getLength();
}

csd::CSD *TriplesLiteralDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const
{
	if(position!=PREDICATE)
		return BaseLiteralDictionary::getDictionarySection(id, position);
	else
		return predicates;
}


unsigned int TriplesLiteralDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	if(position==PREDICATE)
		return id;
	else
		return LiteralDictionary::getGlobalId(mapping, id, position);
}



unsigned int TriplesLiteralDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	if(position==PREDICATE)
		if (id <= predicates->getLength())
			return id;
		else
			throw std::runtime_error("This globalID does not correspond to a PREDICATE");
	else
		return LiteralDictionary::getLocalId(mapping, id, position);
}

void TriplesLiteralDictionary::getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults)
{
	if (role == PREDICATE) 
		predicates->fillSuggestions(base, out, maxResults);
	else
		BaseLiteralDictionary::getSuggestions(base, role, out, maxResults);
}




