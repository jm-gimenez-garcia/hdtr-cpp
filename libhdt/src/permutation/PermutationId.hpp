#ifndef __PERMUTATIONID_HPP__
#define __PERMUTATIONID_HPP__

#include <stdint.h>
#include "Permutation.h"

namespace hdt{
class ProgressListener;
class PermutationMRRR;

class PermutationId{

	public:
		void load(std::istream & in, ProgressListener *listener=NULL);
		void save(std::ostream & f);

	private:
		static const uint8_t TYPE_PERMUTATION;
		PermutationMRRR* permu;

};
}

#endif
