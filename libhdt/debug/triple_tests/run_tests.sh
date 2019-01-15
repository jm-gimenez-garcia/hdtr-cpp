#!/bin/bash

NB_TESTS='50'

(cd ../../src/rdf && make  all) && (cd ../../src/triples && make all) && (cd ../../src/hdt && make all) && (cd ../../src/sequence && make  all) && (cd ../../src/util && make  all)  && (cd ../../src/dictionary && make  all) && (cd ../../src/permutation/ && make clean all)

target=rdf2hdt
make $target && ./$target ../../../testfiles/example_graphs_1+2+3+4+5.nq

target=hdt2rdf_batch
make $target && ./$target $NB_TESTS

for f in $(ls -1 ../../../testfiles/example_graphs_1+2+3+4+5.nq.hdt.rdf_*)
do 
	echo "$f :"
	cat $f
	echo -e '\n'
done

rm ../../../testfiles/example_graphs_1+2+3+4+5.nq.hdt.rdf_*
