

namespace hdt {

GraphsPlainDictionary::~GraphsPlainDictionary() {
	for(unsigned int i=0;i<graphs.size();i++) {
		delete [] graphs[i]->str;
		delete graphs[i];
	}
}

unsigned int GraphsPlainDictionary::stringToId(const std::string &key, const TripleComponentRole position)const
{	

	if(position!=GRAPH)
		return BasePlainDictionary::stringToId(key, position);
	else
	{
		DictEntryIt ret;
		if(key.length()==0)
			return 0;
		ret = hashPredicate.find(key.c_str());
   		return ret==hashGraph.end() ? 0 : ret->second->id;
	}
}

void GraphsPlainDictionary::getNumberOfElements()const
{return BasePlainDictionary::getNumberOfElements + graphs.size();}



void GraphsPlainDictionary::startProcessing(ProgressListener *listener)
{
	graphs.clear();
	BasePlainDictionary::startProcessing(listener);
}

void GraphsPlainDictionary::saveFourthSection(std::ostream &output, ProgressListener *listener, unsigned int& counter, const char marker)
{
	//graphs
	for (unsigned int i = 0; i < graphs.size(); i++) {
		output << graphs[i]->str;
		output.put(marker); //character  to split file
		counter++;
		NOTIFYCOND(listener, "GraphsPlainDictionary saving predicates", counter, getNumberOfElements());
	}
}

void GraphsPlainDictionary::insertFourthRegion(IntermediateListener& iListener, const std::string& line, unsigned int& numLine, unsigned int& numElements)
{
	NOTIFYCOND(&iListener, "Dictionary loading graphs.", numLine, numElements);
	insert(line, UNUSED_GRAPH);	
}

void GraphsPlainDictionary::insertFourthElement(const std::string & str, const TripleComponentRole& pos)
{
	if(pos==GRAPH) {
		DictEntryIt it = hashGraph.find(str.c_str());
		if(it!=hashGraph.end()) {
			//cout << "  existing graph: " << str << endl;
			return it->second->id;
		} else {
			DictionaryEntry *entry = new DictionaryEntry;
            		entry->str = new char [str.length()+1];
			strcpy(entry->str, str.c_str());
			entry->id = graphs.size()+1;
			sizeStrings += str.length();
			//cout << " Add new graph: " << str.c_str() << endl;

			hashGraph[entry->str] = entry;
			graphs.push_back(entry);
			return entry->id;
		}
	}
}
void GraphsPlainDictionary::insert(const string& str, const DictionarySection& pos) {

	if(pos!=UNUSED_GRAPH)
		BasePlainDictionary::insert(str, pos);
	else
	{
		if(str=="") return;
		DictionaryEntry *entry = new DictionaryEntry;
		entry->str = new char [str.length()+1];
		strcpy(entry->str, str.c_str());
		graphs.push_back(entry);
		//entry->id = graphs.size();
		hashGraph[entry->str] = entry;
	}
}

void GraphsPlainDictionary::updateIDs() {
	for (unsigned int i = 0; i < predicates.size(); i++) 
		graphs[i]->id = getGlobalId(i, UNUSED_GRAPH);
	BasePlainDictionary::updateIDs();
}

const vector<DictionaryEntry*>& GraphsPlainDictionary::getDictionaryEntryVector(unsigned int id, TripleComponentRole position) const{
	return (position==GRAPH) ? graphs : BasePlainDictionary::getDictionaryEntryVector(id, position);
}

unsigned int GraphsPlainDictionary::getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position) const
{	
	if(position==UNUSED_GRAPH)
	{
		if(mapping==MAPPING1)
			return shared.size()+subjects.size()+objects.size()+id+1;
		else if(mapping==MAPPING2)
		{
			unsigned int max_s_o = (subjects.size()>objects.size()) ? subjects.size() : objects.size();
			return shared.size()+max_s_o+id+1;
		}
		else
			throw std::runtime_error("Unknown mapping");
	}
	else
		return BasePlainDictionary::getGlobalId(mapping, id, position);
}
unsigned int GraphsPlainDictionary::getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position) const{

	const unsigned int sh_length = shared.size();
	const unsigned int sub_length = subjects.size();
	const unsigned int obj_length = objects.size();
	const unsigned int gr_length = graphs.size();
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
			return id-last_obj_sub_glob_id;
		else
			throw std::runtime_error("This globalID does not correspond to an unused graph");
	else
		return BasePlainDictionary::getLocalId(mapping, id, position);



}
void GraphsPlainDictionary::updateID(unsigned int oldid, unsigned int newid, DictionarySection position) {
	if (position==UNUSED_GRAPH)
		graphs[oldid]->id = newid;
	else
		BasePlainDictionary::updateID(oldid, newid, position);
}




}
