#!/bin/bash
export LD_LIBRARY_PATH=/home/thomas/local/serd/lib:/home/thomas/local/kyotocabinet/lib:/home/thomas/local/zlib/lib

INPUT_DIR=/home/thomas/hdt-2-cpp/testfiles
for i in `(cd $INPUT_DIR && find . -type f)`
do 
	example_name=${i%.*}
	output_dir=/home/thomas/hdt-2-cpp/libhdt/dict_tests/${example_name}
	mkdir -p ${output_dir}
	cd ${output_dir}
	/home/thomas/hdt-2-cpp/libhdt/dict_tests/testNewDictionary $INPUT_DIR/$i  > ${example_name}_new.log 2>&1
done
