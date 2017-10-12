
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <string.h>

#include "PermutationId.h"


#include "../util/bitutil.h"
#include "../libdcs/VByte.h"

#include "../util/crc8.h"
#include "../util/crc32.h"

using namespace std;

void PermutationId::save(ostream & out)
{
	CRC8 crch;
	CRC32 crcd;
	unsigned char arr[9];

	// Write type
	unsigned char type=TYPE_PERMUTATION;
	crch.writeData(out, &type, sizeof(type));

	// Write header CRC
	crch.writeCRC(out);

	// Write data
	perm->save(out);

	// Write Data CRC
	crcd.writeCRC(out);

}




void PermutationId::load(std::istream & in)
{
	size_t count=0;

    // Check type
	CHECKPTR(&ptr[count], maxPtr, 1);
    if(ptr[count++]!=TYPE_PERMUTATION) {
        throw std::runtime_error("Trying to read a PermutationId but the type does not match");
    }


    // CRC
    CRC8 crch;
    crch.update(&ptr[0], count);
    CHECKPTR(&ptr[count], maxPtr, 1);
    if(ptr[count++]!=crch.getValue()) {
        throw std::runtime_error("Wrong checksum in PermutationId Header.");
    }

	
	perm->load(in);
	


	// Validate checksum data
	crc32_t filecrcd = crc32_read(in);
	if(crcd.getValue()!=filecrcd) {
		throw std::runtime_error("Checksum error while reading PermutationId Data");
	}



}

