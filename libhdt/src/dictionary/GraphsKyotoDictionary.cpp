

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
		return graphs.get((const char *)key.c_str(),(size_t)key.length(), (char *) &ret, sizeof(ret)) ? getGlobalId(ret, UNUSED_GRAPH) : 0;
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
	if(position == UNUSED_GRAPH)
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


	const unsigned int sh_length = shared.count();
	const unsigned int sub_length = subjects.count();
	const unsigned int obj_length = objects.count();
	const unsigned int gr_length = graphs.count();
	unsigned int last_obj_sub_glob_id;
	unsigned int last_gr_glob_id;


	if(mapping==MAPPING1)
	{
		last_obj_sub_glob_id = sh_length + sub_length+obj_length;
		last_gr_glob_id  = last_obj_sub_glob_id + gr_length;
	}
	else if (mapping==MAPPING2)
	{
		const unsigned int max_sub_obj_length = (obj_length>sub_length) ? obj_length : sub_length;
		last_obj_sub_glob_id = sh_length + max_sub_obj_length ;
		last_gr_glob_id = last_obj_sub_glob_id + gr_length;
	}
	else
		throw std::runtime_error("Unknown mapping");

	if(position==GRAPH)
		if(id>last_obj_sub_glob_id && id<=last_gr_glob_id)
			return id-last_obj_sub_glob_id-1;
		else
			throw std::runtime_error("This globalID does not correspond to an unused graph");
	else
		return BaseFourSectionDictionary::getLocalId(mapping, id, position);

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
