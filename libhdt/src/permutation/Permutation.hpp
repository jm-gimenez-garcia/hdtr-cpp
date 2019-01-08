/*
 * Permutation.hpp
 *
 *  Created on: Jul 6, 2018
 *      Author: javi
 */

#ifndef LIBHDT_SRC_PERMUTATION_PERMUTATION_HPP_
#define LIBHDT_SRC_PERMUTATION_PERMUTATION_HPP_

#include <Iterator.hpp>
#include <vector>
#include <iostream>

#include <HDTListener.hpp>


namespace hdt {


class Permutation {

public:
	virtual ~Permutation() {
	}

	/**
	 * Compute the value at the given position
	 *
	 * @param position
	 *            The position of the element to be returned
	 * @return int
	 */
	virtual size_t pi(size_t position)=0;

	/**
	 * Gets the position where the value i appears
	 *
	 * @param i
	 * @return int
	 */
	virtual size_t revpi(size_t i)=0;

	/**
	 * Gets the total number of elements in the stream
	 *
	 * @return int
	 */
	virtual size_t getNumberOfElements()=0;

	/**
	 * Returns the size of the stream in bytes
	 *
	 * @return int
	 */
	virtual size_t size()=0;

	/**
	 * Saves the stream to an OutputStream
	 *
	 * @param output
	 *            The OutputStream to be saved to
	 * @throws IOException
	 */
	virtual void save(std::ostream &output)=0;

	/**
	 * Loads a stream from an InputStream
	 *
	 * @param input
	 *            The InputStream to load from
	 */
	virtual void load(std::istream &input)=0;

	virtual size_t load(const unsigned char *ptr, const unsigned char *ptrMax, ProgressListener *listener=NULL)=0;

	virtual std::string getType()=0;

};



} /* namespace hdt */
#endif /* LIBHDT_SRC_PERMUTATION_PERMUTATION_HPP_ */

