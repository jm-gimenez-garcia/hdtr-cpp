namespace hdt {

GraphsFourSectionDictionary::GraphsFourSectionDictionary() : graphs(new csd::CSD_PFC()){}

GraphsFourSectionDictionary::GraphsFourSectionDictionary(HDTSpecification & spec) : graphs(new csd::CSD_PFC()){}

GraphsFourSectionDictionary::~GraphsFourSectionDictionary()
{clear();}
void GraphsFourSectionDictionary::clear()
{
	if (graphs!=NULL)
		{delete graphs; graphs=NULL;}	
}
void GraphsFourSectionDictionary::create()
{
	clear();
	if (graphs==NULL)
		{graphs = new csd::CSD_PFC();}
}


uint64_t GraphsFourSectionDictionary::size()const{
	return BaseFourSectionDictionary::size()+graphs->getLength();
}

size_t GraphsFourSectionDictionary::getNumberOfElements()const{
	return BaseFourSectionDictionary::getNumberOfElements()+graphs->getLength();
}


unsigned int GraphsFourSectionDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{
	if (position!=GRAPH)
		return BaseFourSectionDictionary::stringToId(key, position);
	else
	{
		unsigned int ret;
        	if(key.length()==0)
			return 0;
		ret = graphs->locate((const unsigned char *)key.c_str(), key.length());
		if(ret!=0) 
			return getGlobalId(ret, NOT_SHARED_GRAPH);
        	return 0;
	}
}

void GraphsFourSectionDictionary::loadFourthSection(std::istream & input, IntermediateListener& iListener)
{
	iListener.setRange(50,75);
	iListener.notifyProgress(0, "Dictionary read graphs.");
	delete graphs;
	graphs = csd::CSD::load(input);
	if(graphs==NULL){
		graphs = new csd::CSD_PFC();
		throw std::runtime_error("Could not read graphs.");
	}
	graphs = new csd::CSD_Cache2(graphs);
}



void GraphsFourSectionDictionary::loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)
{

    iListener.setRange(50,75);
    iListener.notifyProgress(0, "Dictionary read graphs.");
    delete graphs;
    graphs = csd::CSD::create(ptr[count]);
    if(graphs==NULL){
        graphs = new csd::CSD_PFC();
        throw std::runtime_error("Could not read graphs.");
    }
    count += graphs->load(&ptr[count], ptrMax);
    graphs = new csd::CSD_Cache2(graphs);
}


void GraphsFourSectionDictionary::importFourthSection(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener) 
{
	NOTIFY(listener, "DictionaryPFC loading graphs", 25, 30);
	iListener.setRange(20, 21);
	IteratorUCharString *itPred = other->getGraphs();
	delete predicates;
	graphs = loadSection(itPred, blocksize, &iListener);
}


IteratorUCharString* GraphsFourSectionDictionary::getGraphs()const {
	return graphs->listAll();
}

void GraphsFourSectionDictionary::saveFourthSection(std::ostream& output, IntermediateListener& iListener){

	iListener.setRange(45,60);
	iListener.notifyProgress(0, "Dictionary save graphs.");
	graphs->save(output);
}

unsigned int GraphsFourSectionDictionary::getNgraphs()const
{return graphs->getLength();}
unsigned int GraphsFourSectionDictionary::getMaxGraphID()const
{return graphs->getLength();}




csd::CSD *GraphsFourSectionDictionary::getDictionarySection(unsigned int id, TripleComponentRole position) const{
	if(position==GRAPH) 
                return graphs;
	else
		return BaseFourSectionDictionary::getDictionarySection(id, position);
}

unsigned int GraphsFourSectionDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	switch (position) {
	case NOT_SHARED_SUBJECT:
		return graphs->getLength()+shared->getLength()+id;
	case NOT_SHARED_OBJECT:
		return (mapping==MAPPING2) ? graphs->getLength()+shared->getLength()+id : graphs->getLength()+shared->getLength()+subjects->getLength()+id;
	case SHARED_SUBJECT:
	case SHARED_OBJECT:
		return graphs->getLength()+id;
	case NOT_SHARED_GRAPH:
		return id;
	}

	throw std::runtime_error("Item not found");
	return (position == NOT_SHARED_GRAPH) ? id : BaseFourSectionDictionary::getGlobalId(mapping, id, position);
}
unsigned int GraphsFourSectionDictionary::getGlobalId(unsigned int id, DictionarySection position)const
{return getGlobalId(mapping, id, position);}

unsigned int GraphsFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	switch (position) {
	case SUBJECT:
		return (id<=shared->getLength()) ? id- graphs-getLength() : id - graphs-getLength() - shared->getLength();
		break;
	case OBJECT:
		if(id<=shared->getLength()) 
			return id - graphs->getLength();
		else 
			return (mapping==MAPPING2) ? id- graphs-getLength() - shared->getLength() : 2+id- graphs-getLength() - shared->getLength() - subjects->getLength();
		break;
	case GRAPH:
		return id;
	}

	throw std::runtime_error("Item not found");
	return (position==GRAPH) ? id : BaseFourSectionDictionary::getLocalId(mapping, id, position);
}
unsigned int GraphsFourSectionDictionary::getLocalId(unsigned int id, TripleComponentRole position)const
{return getLocalId(mapping,id,position);}

void GraphsFourSectionDictionary::getSuggestions(const char *base, hdt::TripleComponentRole role, std::vector<std::string> &out, int maxResults)
{
	if(role==GRAPH)
	{ 
		//graphs->fillSuggestions(base, out, maxResults);
		return;
	}
	else
		BaseFourSectionDictionary::getSuggestions(base, role, out, maxResults);	
}





}
