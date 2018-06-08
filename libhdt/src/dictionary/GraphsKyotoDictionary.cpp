

namespace hdt {

GraphsKyotoDictionary::~GraphsKyotoDictionary() {
	string gpath = graphs.path();
	// graphs.clear();
	graphs.close();

#if 0
	if(gpath.length()>0)
		unlink(gpath.c_str());
#endif
}

IteratorUCharString *GraphsKyotoDictionary::getGraphs()const
{return new KyotoDictIterator(&graphs);0}

size_t GraphsKyotoDictionary::getNumberOfElements()const
{return BaseKyotoDictionary::getNumberOfElements()+graphs.count();}

unsigned int GraphsKyotoDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{
	if(position != GRAPH)
		return 	BaseKyotoDictionary::stringToId(key, TripleComponentRole);
	else
	{
		unsigned int ret;
		if(key.length()==0 || key.at(0) == '?') 
			return 0;
		return graphs.get((const char *)key.c_str(),(size_t)key.length(), (char *) &ret, sizeof(ret)) ? getGlobalId(ret, NOT_SHARED_GRAPH) : 0;
	}
}


void GraphsKyotoDictionary::startProcessing(ProgressListener *listener)
{
	BaseKyotoDictionary::startProcessing(listener);
	// TODO: Add some random suffix to the filenames
#if 1
	unlink("graphs.kct");
#endif

	if (!graphs.open("graphs.kct", ProtoTreeDB::OWRITER | ProtoTreeDB::OCREATE))
		cerr << "graphs db open error: " << graphs.error().name() << endl;
}

unsigned int GraphsKyotoDictionary::insert(const std::string & str, const TripleComponentRole pos)
{
	if(pos!=GRAPH)
		return BaseKyotoDictionary::insert(str,pos);
	else
	{
		if(!graphs.set(str.c_str(), str.length(), (const char*)&value, sizeof(value))) cerr << "set error: " << graphs.error().name() << endl;
		return 0;
	}
}

unsigned int GraphsKyotoDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	if(position == NOT_SHARED_GRAPH)
	{
		if(mapping==MAPPING1)
			return shared.count()+subjects.count()+objects.count()+id+1;
		else if(mapping==MAPPING2)
		{
			unsigned int max_s_o = (subjects.count()>objects.count()) ? subjects.count() : objects.count();
			return shared.count()+max_s_o+id+1;
		}
		else
			throw std::runtime_error("Unknown mapping");
	}
	else
		return BaseKyotoDictionary::getGlobalId(mapping, id, position);

}


unsigned int GraphsKyotoDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	if(position==GRAPH)
	{
		if(mapping==MAPPING1)
			return id-shared.count()-subjects.count()-objects.count()-1;
		else if(mapping==MAPPING2)
		{
			unsigned int max_s_o = (subjects.count()>objects.count()) ? subjects.count() : objects.count();
			return id-shared.count()-max_s_o-1;
		}
		else
			throw std::runtime_error("Unknown mapping");
	}
	else
		return BaseKyotoDictionary::getLocalId(mapping,id, position);

}

unsigned int getNgraphs()const
{return graphs.count();}

unsigned int GraphsKyotoDictionary::getMaxGraphID()const 
{return graphs.count();}



void GraphsKyotoDictionary::dumpSizes(std::ostream &out){
	BaseKyotoDictionary::dumpSizes(out);
	cerr << "\t total graphs:" << getNgraphs() << " ]\n\n";
	cerr << endl;
}





	
}





















}
