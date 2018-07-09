/*
 * File: HDTEnums.cpp
 * Last modified: $Date: 2012-08-13 23:00:07 +0100 (lun, 13 ago 2012) $
 * Revision: $Revision: 222 $
 * Last modified by: $Author: mario.arias $
 *
 * Copyright (C) 2012, Mario Arias, Javier D. Fernandez, Miguel A. Martinez-Prieto
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */

#include <HDTVersion.hpp>
#include <HDT.hpp>

#include "../src/permutation/PermutationMRRR.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <fstream>
#include "../src/util/fileUtil.hpp"

using namespace hdt;
using namespace std;

vector<size_t> input;

void initTest() {
	input.push_back(2);
	input.push_back(3);
	input.push_back(4);
	input.push_back(5);
	input.push_back(7);
	input.push_back(9);
	input.push_back(6);
	input.push_back(1);
	input.push_back(8);
}

void initTestLong() {
	input.push_back(2);
	input.push_back(3);
	input.push_back(4);
	input.push_back(5);
	input.push_back(7);
	input.push_back(9);
	input.push_back(6);
	input.push_back(10);
	input.push_back(8);
	input.push_back(11);
	input.push_back(12);
	input.push_back(13);
	input.push_back(14);
	input.push_back(15);
	input.push_back(16);
	input.push_back(17);
	input.push_back(1);
}

string testpi(PermutationMRRR* perm, size_t input, size_t expected) {
	std::stringstream message;
	if (perm->pi(input) == expected) {
		message << "[PASSED]";
	} else {
		message << "[ERROR!]";
	}
	message << " Test expected " << expected << ", pi(" << input << ")="
			<< perm->pi(input);
	return message.str();
}

string testrevpi(PermutationMRRR* perm, size_t input, size_t expected) {
	std::stringstream message;
	if (perm->revpi(input) == expected) {
		message << "[PASSED]";
	} else {
		message << "[ERROR!]";
	}
	message << " Test expected " << expected << ", revpi(" << input << ")="
			<< perm->revpi(input);
	return message.str();
}

void testPermLongCycle(PermutationMRRR* perm) {

	cout << testpi(perm, 2, 3) << endl;
	cout << testpi(perm, 3, 4) << endl;
	cout << testpi(perm, 8, 1) << endl;

	cout << testrevpi(perm, 7, 5) << endl;
	cout << testrevpi(perm, 8, 9) << endl;
	cout << testrevpi(perm, 4, 3) << endl;
	cout << testrevpi(perm, 1, 8) << endl;

}

int main(int argc, char **argv) {
	try {

		initTest();

		cout << "Test - Step 1" << endl;
		PermutationMRRR* perm = new PermutationMRRR(input, 1, true);
		testPermLongCycle(perm);


		cout << "Test - Step 4" << endl;
		 perm = new PermutationMRRR(input, 4, true);
		testPermLongCycle(perm);

		cout << endl<<"Test - Save and Load Step 4" << endl;

		 ofstream out("test.perm", ios::binary | ios::out | ios::trunc);
		if(!out.good()){
			throw std::runtime_error("Error opening file to save HDT.");
		}
		perm->save(out);
		out.close();

		PermutationMRRR* permLoad = new PermutationMRRR();
		std::filebuf fb2;
		if (fb2.open("test.perm", std::ios::in)) {
			std::istream is(&fb2);
			permLoad->load(is);
			testPermLongCycle(permLoad);

			fb2.close();
		}
/*
		cout << endl<<"Test - Load From Java" << endl;
		PermutationMRRR* permLoadJava = new PermutationMRRR();
		DecompressStream stream("test_fromJava.perm");
		istream *in = stream.getStream();
		permLoadJava->load(*in);
			testPermLongCycle(permLoadJava);

*/


	} catch (std::exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}
}
