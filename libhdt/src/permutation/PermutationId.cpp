
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <string.h>

#include "PermutationId.hpp"
#include "PermutationMRRR.h"
#include "HDTListener.hpp"


#include "../util/bitutil.h"
#include "../libdcs/VByte.h"

#include "../util/crc8.h"
#include "../util/crc32.h"
#include <sstream>
using namespace std;
using namespace cds_static;

namespace hdt{

const uint8_t PermutationId::TYPE_PERMUTATION = 1;

void PermutationId::save(ostream & out)
{
	CRC8 crch;

	// Write type
	unsigned char type=TYPE_PERMUTATION;
	crch.writeData(out, (unsigned char*)&type, sizeof(unsigned char));

	stringstream oss;
	// Write permutation	
	permu->save(oss);
	const string& tmpString = oss.str();   
	const char* cstr = tmpString.c_str();

	size_t tmp_stream_len=oss.tellp();
	crch.writeData(out, (unsigned char*)&tmp_stream_len, sizeof(size_t));
	crch.writeCRC(out);

	CRC32 crcd;
	crcd.writeData(out, (unsigned char*)&cstr[0], tmp_stream_len);
	crcd.writeCRC(out);

}



#define CHECKPTR(base, max, size) if(((base)+(size))>(max)) throw std::runtime_error("Could not read completely the HDT from the file.");

void PermutationId::load(std::istream & in, ProgressListener *listener/*=NULL*/)
{

	CRC8 crch;
	unsigned char type;
	crch.readData(in, (unsigned char*)&type, sizeof(unsigned char));
	if(type!=TYPE_PERMUTATION) {
		//throw std::runtime_error("Trying to read a PermutationId but data is not PermutationId");
	}
	
	size_t tmp_stream_len;
	// Read numbits
	crch.readData(in, (unsigned char*)&tmp_stream_len, sizeof(size_t));

	// Validate Checksum Header
	crc8_t filecrch = crc8_read(in);
	if(crch.getValue()!=filecrch) {
		throw std::runtime_error("Checksum error while reading PermutationId header.");
	}

	char* cstr = new char[tmp_stream_len];

	CRC32 crcd;
	crcd.readData(in, (unsigned char*)&cstr[0], tmp_stream_len);

	stringstream oss;
	oss.write(cstr,tmp_stream_len);
	delete[]cstr;cstr=NULL;
	
	permu = PermutationMRRR::load(oss);
	
	// Validate Checksum
	crc32_t filecrch2 = crc32_read(in);
	if(crcd.getValue()!=filecrch2) {
		throw std::runtime_error("Checksum error while reading LogSequence2 Data");
	}


}
#undef CHECKPTR
}
