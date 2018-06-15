#ifndef __BASE_LITERAL_DICTIONARY_HPP__
#define __BASE_LITERAL_DICTIONARY_HPP__




#include <Iterator.hpp>
#include <Dictionary.hpp>
#include <HDTSpecification.hpp>

#include "../libdcs/CSD.h"

namespace hdt {

class BaseLiteralDictionary : public Dictionary {
private:
	csd::CSD *subjects;
	csd::CSD *objectsNotLiterals;
	csd::CSD *objectsLiterals;
	csd::CSD *shared;

	unsigned int mapping;
	uint64_t sizeStrings;
	uint32_t blocksize;

	//ControlInformation controlInformation;
	HDTSpecification spec;


protected:
	virtual void clear();
	virtual void create();
	virtual void clear_all()=0;
	virtual void create_all()=0;

public:
	BaseLiteralDictionary();
	BaseLiteralDictionary(HDTSpecification &spec);
	virtual ~BaseLiteralDictionary();

	std::string idToString(const unsigned int id, const TripleComponentRole position)const;
	virtual unsigned int stringToId(const std::string &str, const TripleComponentRole position)const;

	/** Returns the number of IDs that contain s[1,..len] as a substring. It also
	 * return in occs the IDs. Otherwise return 0.
	 *  @s: the substring to be located.
	 *  @len: the length (in characters) of the string s.
	 *  @occs: pointer where the ID located will be stored.
	 * */
	uint32_t substringToId(unsigned char *s, uint32_t len, uint32_t **occs);
    uint32_t substringToId(unsigned char *s, uint32_t len, uint32_t offset, uint32_t limit, bool deduplicate, uint32_t **occs, uint32_t* num_occ);

    virtual size_t getNumberOfElements();

    virtual uint64_t size();

	unsigned int getNsubjects();
	unsigned int getNobjects();
	unsigned int getNshared();

	unsigned int getNobjectsNotLiterals();
	unsigned int getNobjectsLiterals();

	virtual unsigned int getMaxID();
	unsigned int getMaxSubjectID();
	unsigned int getMaxObjectID();

	void populateHeader(Header &header, string rootNode);
	virtual void populateHeaderNumFourthSection(Header & header, string rootNode)=0;
	virtual void populateHeaderMaxFourthSectionId(Header & header, string rootNode)=0;
	
	void save(std::ostream &output, ControlInformation &ci, ProgressListener *listener = NULL);
	void saveControlInfo(std::ostream& output, ControlInformation& controlInformation);
	void saveShared(std::ostream & output, IntermediateListener& iListener);
	void saveSubjects(std::ostream & output, IntermediateListener& iListener);
	virtual void saveFourthSection(std::ostream & output, IntermediateListener& iListener)=0;
	void saveObjects(std::ostream & output, IntermediateListener& iListener);


	void load(std::istream &input, ControlInformation &ci, ProgressListener *listener = NULL);
	void loadControlInfo(std::istream & input, ControlInformation & ci);
	void loadShared(std::istream & input, IntermediateListener& iListener);
	void loadSubjects(std::istream & input, IntermediateListener& iListener);
	virtual void loadFourthSection(std::istream & input, IntermediateListener& iListener)=0;
	void loadObjects(std::istream & input, IntermediateListener& iListener);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);
	void loadControlInfo(unsigned char *ptr, unsigned char *ptrMax, size_t& count);
	void loadShared(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	void loadSubjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
	virtual void loadFourthSection(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener)=0;
	void loadObjects(unsigned char *ptr, unsigned char *ptrMax, size_t& count, IntermediateListener& iListener);
 

    void import(Dictionary *other, ProgressListener *listener=NULL);
	void importSubjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);
	virtual void importFourthSection(Dictionary *other, IntermediateListener& iListener)=0;
	void importObjects(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener);
	void importShared(Dictionary *other, ProgressListener *listener, IntermediateListener& iListener); 



    IteratorUCharString *getSubjects();
    IteratorUCharString *getObjects();
    IteratorUCharString *getShared();

	unsigned int insert(const std::string &str, const TripleComponentRole position);

	void startProcessing(ProgressListener *listener = NULL);
	void stopProcessing(ProgressListener *listener = NULL);

	string getType();
	unsigned int getMapping();

	virtual void getSuggestions(const char *base, TripleComponentRole role, std::vector<string> &out, int maxResults);

    hdt::IteratorUCharString *getSuggestions(const char *prefix, TripleComponentRole role);
    hdt::IteratorUInt *getIDSuggestions(const char *prefix, TripleComponentRole role);


private:
	virtual csd::CSD *getDictionarySection(unsigned int id, TripleComponentRole position)const;
	virtual unsigned int getGlobalId(unsigned int mapping, unsigned int id, DictionarySection position)const;
	unsigned int getGlobalId(unsigned int id, DictionarySection position)const;
	virtual unsigned int getLocalId(unsigned int mapping, unsigned int id, TripleComponentRole position)const;
	unsigned int getLocalId(unsigned int id, TripleComponentRole position)const;


};

}

#endif /* BaseLiteralDictionary_HPP_ */


