#ifndef __PERMUTATIONID_HPP__
#define __PERMUTATIONID_HPP__

class ProgressListener;
class permutation;

class PermutationId{

	public:
		void load(std::istream & in, ProgressListener *listener=NULL);
		void save(std::ostream & f);

	private:
		static const uint8_t TYPE_PERMUTATION = 1;
		permutation* perm;

};


#endif
