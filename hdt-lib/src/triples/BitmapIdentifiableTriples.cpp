#include "BitmapIdentifiableTriples.hpp"

void BitmapIdentifiableTriples::load(ModifiableTriples &triples, ProgressListener *listener) 
{
	triples.sort(order);

	IteratorTripleID *it = triples.searchAll();

	bitmapY = new BitSequence375(triples.getNumberOfElements()/2);
	bitmapZ = new BitSequence375(triples.getNumberOfElements());
	bitmapId = new BitSequence375(triples.getNumberOfElements());

	LogSequence2* permId_tmp = vector<unsigned int>;
	permId_tmp.reserve(triples.getNumberOfElements()/2);

	LogSequence2 *vectorY = new LogSequence2(bits(triples.getNumberOfElements()));
	LogSequence2 *vectorZ = new LogSequence2(bits(triples.getNumberOfElements()),triples.getNumberOfElements());

	unsigned int lastX, lastY, lastZ;
	unsigned int x, y, z, gr;

	unsigned int numTriples=0;

	while(it->hasNext()) {
		TripleID *triple = it->next();

		swapComponentOrder(triple, SPO, order);

		x = triple->getSubject();
		y = triple->getPredicate();
		z = triple->getObject();

		QuadID* quad = dynamic_cast<QuadID*>(triple);
		if (quad==NULL)
			gr = 0;
		else
			gr = triple->getGraph();


		if(x==0 || y==0 || z==0) {
			cerr << "ERROR: Triple with at least one component zero." << endl;
			continue;
		}

		if(numTriples==0){
			vectorY->push_back(y);
			vectorZ->push_back(z);


		} else if(x!=lastX) {
            if(x!=lastX+1) {
                throw std::runtime_error("Error: The subjects must be correlative.");
            }
			bitmapY->append(true);
			vectorY->push_back(y);

			bitmapZ->append(true);
			vectorZ->push_back(z);
		} else if(y!=lastY) {
            if(y<lastY) {
                throw std::runtime_error("Error: The predicates must be in increasing order.");
            }
            bitmapY->append(false);
			vectorY->push_back(y);

			bitmapZ->append(true);
			vectorZ->push_back(z);
		} else {
            if(z<=lastZ) {
                throw std::runtime_error("Error, The objects must be in increasing order.");
            }
            bitmapZ->append(false);
			vectorZ->push_back(z);
		}
		
		if (gr == 0)
			bitmapId->append(false);
		else
		{
			bitmapId->append(true);
			permId_tmp->push_back(gr);
		}

		lastX = x;
		lastY = y;
		lastZ = z;



        NOTIFYCOND(listener, "Converting to BitmapTriples", numTriples, triples.getNumberOfElements());
		numTriples++;
	}

	bitmapY->append(true);
	bitmapZ->append(true);

	delete it;

	vectorY->reduceBits();

	delete arrayY;
	arrayY = vectorY;

	delete arrayZ;
	arrayZ = vectorZ;


	BitSequenceBuilder* bit_seq_build = new BitSequenceBuilderRG(4);
	permId =new PermutationMRRR(&permId_tmp[0],permId_tmp.size(),bit_seq_build);

	//PermutationBuilder* permBuild = new PermutationBuilderMRRR(ceil(log2((double)permId_tmp.size())),bit_seq_build);
	//permId = permBuild->build(&permId_tmp[0],permId_tmp.size());

	delete bit_seq_build ; bit_seq_build=NULL;


#if 0
	AdjacencyList adjY(arrayY, bitmapY);
	AdjacencyList adjZ(arrayZ, bitmapZ);
	adjY.dump();
	adjZ.dump();
#endif
}




void BitmapIdentifiableTriples::save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener)
{
	CHECK_BITMAPTRIPLES_INITIALIZED

	controlInformation.setFormat(getType());
	controlInformation.setUint("order", order);
	controlInformation.save(output);

	IntermediateListener iListener(listener);
	iListener.setRange(0,5);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Bitmap Y");
	bitmapY->save(output);

	iListener.setRange(5,15);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Bitmap Z");
	bitmapZ->save(output);

	iListener.setRange(15,30);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Stream Y");
	arrayY->save(output);

	iListener.setRange(30,70);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Stream Z");
	arrayZ->save(output);

	iListener.setRange(70,80);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Bitmap Id");
	bitmapId->save(output);


	
	iListener.setRange(80,100);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples saving Permutation");

	CRC32 crcd;
	std::stringstream oss;
	permId->save(oss);
	const std::string& tmpString = oss.str();   
	const char* cstr = tmpString.c_str();
	crcd.writeData(out, cstr, oss.tellp());

	crcd.writeCRC(out);



}
void BitmapIdentifiableTriples::load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener = NULL)
{
	std::string format = controlInformation.getFormat();
	if(format!=getType()) {
		throw std::runtime_error("Trying to read a BitmapIdentifiableTriples but the data is not BitmapIdentifiableTriples");
	}

	order = (TripleComponentOrder) controlInformation.getUint("order");

	IntermediateListener iListener(listener);

	iListener.setRange(0,5);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples loading Bitmap Y");
	bitmapY = BitSequence375::load(input);
	if(bitmapY==NULL){
		throw std::runtime_error("Could not read bitmapY.");
	}

	iListener.setRange(5,10);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples loading Bitmap Z");
	bitmapZ = BitSequence375::load(input);
	if(bitmapZ==NULL){
		throw std::runtime_error("Could not read bitmapZ.");
	}

	iListener.setRange(10,20);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples loading Array Y");
	delete arrayY;
	arrayY = IntSequence::getArray(input);
	arrayY->load(input);

	iListener.setRange(20,35);
	iListener.notifyProgress(0, "BitmapIdentifiableTriples loading Array Z");
	delete arrayZ;
	arrayZ = IntSequence::getArray(input);
    arrayZ->load(input);

	iListener.setRange(35,40);
	iListener.notifyProgress(0, "BitmapTriples loading Bitmap Id");
	delete bitmapId;
	bitmapId = BitSequence375::load(input);
	if(bitmapZ==NULL){
		throw std::runtime_error("Could not read Bitmap Id.");
	}




	iListener.setRange(40,50);
	iListener.notifyProgress(0, "BitmapTriples loading Permutation");
	delete permId;
	permId = IntSequence::getArray(input);

	CRC32 crcd;
	std::stringstream iss;

    permId->load(iss);

	const std::string& tmpString = iss.str();   
	const char* cstr = tmpString.c_str();
	crcd.readData(input, cstr, iss.tellp());

	// Validate checksum for permutation data
	crc32_t filecrcd = crc32_read(input);
	if(crcd.getValue()!=filecrcd)
        throw std::runtime_error("Checksum error while reading Permutation Data.");



}



