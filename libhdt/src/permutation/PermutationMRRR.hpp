/*
 * PermutationMRRR.hpp
 *
 *  Created on: Jul 6, 2018
 *      Author: javi
 */

#ifndef LIBHDT_SRC_PERMUTATION_PERMUTATIONMRRR_HPP_
#define LIBHDT_SRC_PERMUTATION_PERMUTATIONMRRR_HPP_

#include <stddef.h>
#include <iostream>
#include <string>

#include "Permutation.hpp"
#include "../sequence/LogSequence2.hpp"
#include "../bitsequence/BitSequence375.h"

namespace hdt {

class PermutationMRRR: public Permutation {
public:

	PermutationMRRR(vector<size_t> elements,size_t step, bool IDsStartsAtOne);
	PermutationMRRR(vector<size_t> elements,size_t step);

	PermutationMRRR();

	/**
	 * Destructor.
	 */
	~PermutationMRRR();

	/**
	 * Compute the value at the given position
	 *
	 * @param position
	 *            The position of the element to be returned
	 * @return int
	 */
	size_t pi(size_t position);

	/**
	 * Gets the position where the value i appears
	 *
	 * @param i
	 * @return int
	 */
	size_t revpi(size_t i);

	/**
	 * Gets the total number of elements in the stream
	 *
	 * @return int
	 */
	size_t getNumberOfElements();

	/**
	 * Returns the size of the stream in bytes
	 *
	 * @return int
	 */
	size_t size();

	/**
	 * Saves the stream to an OutputStream
	 *
	 * @param output
	 *            The OutputStream to be saved to
	 * @throws IOException
	 */
	void save(std::ostream &output);

	/**
	 * Loads a stream from an InputStream
	 *
	 * @param input
	 *            The InputStream to load from
	 */
	void load(std::istream &input);

	size_t load(const unsigned char *ptr, const unsigned char *ptrMax, ProgressListener *listener=NULL);

	std::string getType();

private:
	static const uint8_t TYPE_PERMUTATION_MRRR = 1;

	LogSequence2* sequence;  // elements of the permutation
	BitSequence375* bitmap; // bitmap to mark the backward pointers
	LogSequence2* backwardPointers; // backward pointers
	size_t step; // the sampling step for backward pointers
	bool IDsStartsAtOne; // marks if the IDs starts at one, true by default

	void createPerm(vector<size_t> elements);
};

} /* namespace hdt */

#endif /* LIBHDT_SRC_PERMUTATION_PERMUTATIONMRRR_HPP_ */
