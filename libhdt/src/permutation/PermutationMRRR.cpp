/*
 * PermutationMRRR.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: javi
 */

#include "PermutationMRRR.hpp"
#include "HDTVocabulary.hpp"
#include "../util/crc8.h"
#include <map>

namespace hdt {



PermutationMRRR::PermutationMRRR(){
	step =8;
	sequence = new LogSequence2();
	bitmap = new BitSequence375();
	backwardPointers = new LogSequence2();
	IDsStartsAtOne=true;
}
PermutationMRRR::PermutationMRRR(vector<size_t> elements,size_t step){
	IDsStartsAtOne=true;
	this->step =step;
	createPerm(elements);
}

PermutationMRRR::PermutationMRRR(vector<size_t> elements,size_t step, bool IDsStartsAtOne){
	this->IDsStartsAtOne=IDsStartsAtOne;
	this->step =step;
	createPerm(elements);
}

void PermutationMRRR::createPerm(vector<size_t> elements){
	if (IDsStartsAtOne==true){
		elements.insert(elements.begin(), 0); //insert a fakeID=0
	}
	sequence= new LogSequence2(bits(elements.size()),elements.size());
	//Auxiliary structures
	map<size_t,size_t> aux_backwardPointers; //auxiliary for backward pointers (key=position, value=element value), sorted by key

	if (step==1){ // if the step is one, create the sequence from the list and add all backward pointers. In this case we don't need the bitmap as all values are present

		backwardPointers =   new LogSequence2(bits(elements.size()),elements.size());

		bitmap = new BitSequence375(); //bitmap will be empty but we need to create it in order to save it afterwards

		long bitIndex=0;
		for (int i=0;i<elements.size();i++){
			long value = elements[i]; // get the element value
			sequence->push_back(value); // insert the value in the final sequence
			aux_backwardPointers[value] = bitIndex; // insert the reverse in the backward pointers
			bitIndex++;
		}
		// we iterate in order of keys in order to add the aux pointers to the final pointers (it could be done with the add function of the sequence, but it has a bug
		// backwardPointers.add(aux_backwardPointers.values().iterator()); // this has a bug

		for (int i=0;i<aux_backwardPointers.size();i++){
			backwardPointers->push_back(aux_backwardPointers[i]);
		}
	}
	else{ // implement permutation with a step

		BitSequence375* aux_markVisited = new BitSequence375(elements.size()); // bitmap to mark the visited elements
		bitmap = new BitSequence375(elements.size());
		for (int i=0;i<elements.size();i++){
			if (aux_markVisited->access(i)==false){ // if not visited
				aux_markVisited->set(i, true); // mark as visited
				long aux=0;
				long nextElement = i;
				long j=i; // j is the index of the iteration of the cycle.
				while(nextElement!=elements[j]){
					j=elements[j]; // get next element in the cycle
					aux_markVisited->set(j, true); // mark as visited
					aux++;
					// mark the future backward pointer if it's in accordance with the step
					if (aux>=step){
						bitmap->set(j,true);
						aux=0; // restart the count, it is like in theory doing the sample every given "steps"
					}
				}

			}
		}
		delete aux_markVisited;
		// in the second iteration we store the sequence and the backward pointers
		aux_markVisited = new BitSequence375(elements.size()); // restart the visited nodes
		for (int i=0;i<elements.size();i++){
			long value = elements[i];
			long pointer=0;
			sequence->push_back(value); // insert the value in the final sequence
			if (aux_markVisited->access(i)==false){ // if not visited

				aux_markVisited->set(i, true); // mark as visited
				pointer=i; //prepare pointer
				long aux=0;
				long nextElement = i;
				long j=i; // j is the index of the iteration of the cycle.
				long cyclesize=0; // number of total elements in the cycle
				while(nextElement!=elements[j]){
					j=elements[j]; // get next element in the cycle
					aux_markVisited->set(j, true); // mark as visited
					aux++;
					if (aux>=step){
						aux_backwardPointers[j] = pointer; // insert the reverse in the backward pointers
						pointer = j; // prepare the pointer for the next step
						aux=0; // restart he count, it is like in theory doing the sample every given "steps"
					}
					cyclesize++;
				}
				// add the last pointer

				if (cyclesize>=step){
					aux_backwardPointers[nextElement] = pointer; // insert the reverse in the backward pointers
					bitmap->set(nextElement,true);
				}
			}
		}
		delete aux_markVisited;
		// create the sequence of backwardPointers

		backwardPointers =  new LogSequence2(bits(elements.size()-1),aux_backwardPointers.size());//-1 because one of them is 0

		// we iterate in order of keys in order to add the aux pointers to the final pointers (it could be done with the add function of the sequence, but it has a bug
		// backwardPointers.add(aux_backwardPointers.values().iterator()); // this has a bug
		for( std::map<size_t, size_t>::iterator iter = aux_backwardPointers.begin();
		     iter != aux_backwardPointers.end();
		     ++iter ){
			backwardPointers->push_back(iter->second);
		}

		bitmap->trimToSize();
	}

	sequence->reduceBits();
	backwardPointers->reduceBits();
}

/**
 * Compute the value at the given position
 *
 * @param position
 *            The position of the element to be returned
 * @return int
 */
size_t PermutationMRRR::pi(size_t position){
	if (position>=sequence->getNumberOfElements()){
		throw std::runtime_error("The given position is bigger than the size of the permutation");
	}
	return sequence->get(position); // use -1 if we disregard the 0
}

/**
 * Gets the position where the value i appears
 *
 * @param i
 * @return int
 */
size_t PermutationMRRR::revpi(size_t i){

//	cout<<"Print current status"<<endl;
//	cout<<"Sequence:"<<endl;
//	for (int i=0;i<sequence->getNumberOfElements();i++){
//		cout<<i<<": "<<sequence->get(i)<<endl;
//	}
//	cout<<endl;
//	cout<<"bitmap :"<<endl;
//	for (int i=0;i<bitmap->getNumBits();i++){
//		cout<<i<<": "<<bitmap->access(i)<<endl;
//	}
//	cout<<endl;
//	cout<<"Pointers:"<<endl;
//	for (int i=0;i<backwardPointers->getNumberOfElements();i++){
//		cout<<i<<": "<<backwardPointers->get(i)<<endl;
//	}
//	cout<<endl;

	if (i>=sequence->getNumberOfElements()){
		throw std::runtime_error("The given value is bigger than the maximum permutation ID");
	}
	if (step==1){
		return backwardPointers->get(i); // use -1 if we disregard the 0
	}
	else{

		size_t j=i;
		size_t value=sequence->get(j);
		while ((bitmap->access(j)==false) && value!=i){ // follow the sequence until a pointer or a cycle is found
			j = value;
			value = sequence->get(j);
		}
		if (value!=i){ // follow backward pointer
			j = backwardPointers->get(bitmap->rank1(j-1)); //get the pointer for the current position, marked with the ranks of the bitmap
			value=sequence->get(j);
			while (value!=i){
				j = value;
				value = sequence->get(j);
			}

		}
		return j;

	}
}

/**
 * Gets the total number of elements in the stream
 *
 * @return int
 */
size_t PermutationMRRR::getNumberOfElements(){
	return sequence->getNumberOfElements();
}

/**
 * Returns the size of the stream in bytes
 *
 * @return int
 */
size_t PermutationMRRR::size(){
	return sequence->size()+bitmap->getSizeBytes()+backwardPointers->size();
}

/**
 * Saves the stream to an OutputStream
 *
 * @param output
 *            The OutputStream to be saved to
 * @throws IOException
 */
void PermutationMRRR::save(std::ostream &output){

	CRC8 crch;
	// Write type
	uint8_t type = TYPE_PERMUTATION_MRRR;
	crch.writeData(output, &type, sizeof(uint8_t));
	crch.writeData(output, (unsigned char*)&step, sizeof(uint8_t));

	// Write Header CRC
	crch.writeCRC(output);

	sequence->save(output);
	bitmap->save(output);
	backwardPointers->save(output);
}

/**
 * Loads a stream from an InputStream
 *
 * @param input
 *            The InputStream to load from
 */
void PermutationMRRR::load(std::istream &input){

	CRC8 crch;

	// Read type
	uint8_t type;
	crch.readData(input, (unsigned char*)&type, sizeof(type));
	if(type!=TYPE_PERMUTATION_MRRR) {
		throw std::runtime_error("Trying to read a Permutation MRRR but data is not");
	}
	crch.readData(input, (unsigned char*)&step, sizeof(uint8_t));
	// Validate Checksum Header
	crc8_t filecrch = crc8_read(input);
	if(crch.getValue()!=filecrch) {
		throw std::runtime_error("Checksum error while reading Permutation MRRR header.");
	}

	sequence->load(input);
	bitmap = BitSequence375::load(input);
	backwardPointers->load(input);

}

#define CHECKPTR(base, max, size) if(((base)+(size))>(max)) throw std::runtime_error("Could not read completely the HDT from the file.");
size_t PermutationMRRR::load(const unsigned char *ptr, const unsigned char *ptrMax, ProgressListener *listener/*=NULL*/){
	size_t count=0;

//cout << __FILE__ << ":" << __LINE__ <<" : values to read: "
//for(unsigned int i=0; i<5;i++)
//	cout << static_cast<unsigned int>(ptr[i]) << " ";
//cout << endl;

    // Check type
	CHECKPTR(&ptr[count], ptrMax, 1);
    if(ptr[count++]!=TYPE_PERMUTATION_MRRR) 
        throw std::runtime_error("Trying to read a PermutationMRRR but the type does not match");

	uint8_t step_u8;
	// Read Step
	CHECKPTR(&ptr[count], ptrMax, sizeof(uint8_t));
	memcpy((void*)&step_u8,(void*)(ptr+count), sizeof(uint8_t));
	count += sizeof(uint8_t);
	step = static_cast<size_t>(step_u8);

    // Validate Checksum Header
    CRC8 crch;
    crch.update(&ptr[0], count);
    CHECKPTR(&ptr[count], ptrMax, 1);
	/*unsigned int check_val_m1 = static_cast<unsigned int>(ptr[count]);
	unsigned int check_val = static_cast<unsigned int>(ptr[count++]);
	unsigned int check_val_p1 = static_cast<unsigned int>(ptr[count++]);
	cout << __FILE__ << ":" << __LINE__ << " : crch.getValue()=" << static_cast<unsigned int>(crch.getValue()) << endl;
	cout << __FILE__ << ":" << __LINE__ << " : check_val_m1=" <<  check_val_m1 << endl;
	cout << __FILE__ << ":" << __LINE__ << " : check_val=" <<  check_val << endl;
	cout << __FILE__ << ":" << __LINE__ << " : check_val_p1=" <<  check_val_p1 << endl;*/
    if(crch.getValue()!=ptr[count++])
        throw std::runtime_error("Checksum error while reading PermutationMRRR header.");

	//Read sequence
	count += sequence->load(&ptr[count], ptrMax, listener);

	//Read bitmap
	count += bitmap->load(&ptr[count], ptrMax, listener);

	//Read backwardPointers
	count += backwardPointers->load(&ptr[count], ptrMax, listener);

	return count;

}
#undef CHECKPTR

std::string PermutationMRRR::getType(){
	return HDTVocabulary::PERM_TYPE_MRRR;
}

PermutationMRRR::~PermutationMRRR() {
	// TODO Auto-generated destructor stub
}

} /* namespace hdt */
