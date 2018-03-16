#ifndef HDT_GRAPH_DICTIONARY_HPP_
#define HDT_GRAPH_DICTIONARY_HPP_


namespace hdt {

class GraphDictionary : public virtual Dictionary
{
public:
   virtual ~GraphDictionary(){ }


    /* Return the number of different subjects of the current dictionary. */
    virtual unsigned int getNsubjects()=0;

    /* Return the number of shared subjects-objects of the current dictionary */
    virtual unsigned int getNshared()=0;

    /* Return the number of different objects of the current dictionary */
    virtual unsigned int getNobjects()=0;

    /* Return the number of diferent graphs of the current dictionary */
    virtual unsigned int getNgraphs()=0;



    /* Return the maximum subject ID of the dictionary. */
    virtual unsigned int getMaxSubjectID()=0;

    /* Return the maximum graph ID of the dictionary. */
    virtual unsigned int getMaxGraphID()=0;

    /* Return the maximum object ID of the dictionary. */
    virtual unsigned int getMaxObjectID()=0;



    virtual IteratorUCharString *getSubjects()=0;
    virtual IteratorUCharString *getGraphs()=0;
    virtual IteratorUCharString *getObjects()=0;
    virtual IteratorUCharString *getShared()=0;

};



class GraphModifiableDictionary : public ModifiableDictionary, public GraphDictionary  {
public:
	virtual ~GraphModifiableDictionary(){ }

};

}

#endif // HDT_GRAPH_DICTIONARY_HPP_

