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
			return getGlobalId(ret, UNUSED_GRAPH);
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

	if(position==UNUSED_GRAPH)
	{
		if(mapping==MAPPING1)
			return id+shared->getLength()+subjects->getLength()+objects->getLength();
		else if (mapping==MAPPING2)
		{
			unsigned int max_s_o = (subjects->getLength() > objects->getLength()) ? subjects->getLength() : objects->getLength();
			return id+shared->getLength()+max_s_o;
		}
		else
			throw std::runtime_error("Unknown mapping");
		return shared->getLength()+max_s_o+id;
	}
	else
		return BaseFourSectionDictionary::getGlobalId(mapping, id, position);
}

unsigned int GraphsFourSectionDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{

	if(position==GRAPH)
	{
		const unsigned int sh_length = shared->getLength();
		const unsigned int sub_length = subjects->getLength();
		const unsigned int obj_length = objects->getLength();
		const unsigned int gr_length = graphs->getLength();
		unsigned int last_obj_sub_glob_id;
		unsigned int locid_shift = 0;

		if(mapping==MAPPING1)
		{
			locid_shift = 2;
			last_obj_sub_glob_id = sh_length + sub_length+obj_length;
		}
		else if (mapping==MAPPING2)
		{
			const unsigned int max_sub_obj_length = (obj_length>sub_length) ? obj_length : sub_length;
			last_obj_sub_glob_id = sh_length + max_sub_obj_length ;
		}
		else
			throw std::runtime_error("Unknown mapping");


		const unsigned int last_gr_glob_id  = last_obj_sub_glob_id + gr_length;
		if(id>last_obj_sub_glob_id && id<=last_gr_glob_id)
			return locid_shift + id-last_obj_sub_glob_id;
		else
			throw std::runtime_error("This globalID does not correspond to an unused graph");
	}
	else
		return BaseFourSectionDictionary::getLocalId(mapping, id, position);
}

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
