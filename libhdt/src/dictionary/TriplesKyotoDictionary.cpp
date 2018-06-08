
namespace hdt {

TriplesKyotoDictionary::~TriplesKyotoDictionary() {
	string ppath = predicates.path();
	// predicates.clear();
	predicates.close();

#if 0
	if(ppath.length()>0)
		unlink(ppath.c_str());
#endif
}

IteratorUCharString *TriplesKyotoDictionary::getPredicates()const
{return new KyotoDictIterator(&predicates);}

size_t TriplesKyotoDictionary::getNumberOfElements()const
{return BaseKyotoDictionary::getNumberOfElements()+predicates.count();}

unsigned int TriplesKyotoDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{
	if(position != PREDICATE)
		return 	BaseKyotoDictionary::stringToId(key, TripleComponentRole);
	else
	{
		unsigned int ret;
		if(key.length()==0 || key.at(0) == '?') 
			return 0;
		return predicates.get((const char *)key.c_str(),(size_t)key.length(), (char *) &ret, sizeof(ret)) ? getGlobalId(ret, NOT_SHARED_PREDICATE) : 0;
	}
}


void TriplesKyotoDictionary::startProcessing(ProgressListener *listener)
{
	BaseKyotoDictionary::startProcessing(listener);
	// TODO: Add some random suffix to the filenames
#if 1
	unlink("predicates.kct");
#endif

	if (!predicates.open("predicates.kct", ProtoTreeDB::OWRITER | ProtoTreeDB::OCREATE))
		cerr << "predicates db open error: " << predicates.error().name() << endl;
}


unsigned int TriplesKyotoDictionary::insert(const std::string & str, const TripleComponentRole pos)
{
	if(pos!=PREDICATE)
		return BaseKyotoDictionary::insert(str,pos);
	else
	{
		if(!predicates.set(str.c_str(), str.length(), (const char*)&value, sizeof(value))) cerr << "set error: " << predicates.error().name() << endl;
		return 0;
	}

}	

unsigned int TriplesKyotoDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const{
	if(position == NOT_SHARED_PREDICATE)
			return id+1;
	else
			return BaseKyotoDictionary::getGlobalId(mapping, id, position);
}


unsigned int TriplesKyotoDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{
	if(position==PREDICATE) 
		return id-1;
	else
		return BaseKyotoDictionary::getLocalId(mapping,id, position);
}

unsigned int TriplesKyotoDictionary::getNpredicates()const
{return predicates.count();}

unsigned int TriplesKyotoDictionary::getMaxPredicateID()const 
{return predicates.count();}

void TriplesKyotoDictionary::dumpSizes(std::ostream &out){
	BaseKyotoDictionary::dumpSizes(out);
	cerr << "\t total predicates:" << getNpredicates() << " ]\n\n";
	cerr << endl;
}





}
















}
