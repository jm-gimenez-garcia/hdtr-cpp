include ../Makefile.inc

SRC = testHdt2rdf_batch.cpp
#SRC = testRdf2hdt.cpp
#SRC = testNewBitmapQuadsBasic.cpp
#SRC = testNewTriple.cpp
OBJ = $(patsubst %.cpp,%.o,$(SRC))
EXE = $(patsubst %.cpp,%,$(SRC))

LIBS=$(LIBUTIL) $(LIBCDS) $(LIBCSD) $(LIBHDT)  $(LIBRDF) $(LIBDICT) $(LIBTRIPLES) $(LIBPERMUTATION) $(LIBHEADER) $(LIBSEQUENCE) $(LIBBITSEQUENCE) $(LIBHUFFMAN)

all: $(EXE) 

hdt2rdf_batch: testHdt2rdf_batch.o $(LIBS) 
	$(CXX) $<  $(LDFLAGS)  -o $@ 
hdt2rdf: testHdt2rdf.o $(LIBS) 
	$(CXX) $<  $(LDFLAGS)  -o $@ 
rdf2hdt: testRdf2hdt.o $(LIBS) 
	$(CXX) $<  $(LDFLAGS)  -o $@ 
quads: testNewBitmapQuads.o $(LIBS)
	$(CXX) $<  $(LDFLAGS)  -o $@ 
quadsbasic: testNewBitmapQuadsBasic.o $(LIBS)
	$(CXX) $<  $(LDFLAGS)  -o $@ 

$(EXE) : $(OBJ) $(LIBS)
	$(CXX) $<  $(LDFLAGS)  -o $@ 

%.o: %.cpp 
	$(CXX) -c $^ $(CXXFLAGS)  -o $@ 


$(LIBCSD): $(LIBCSD_OBJ)
	ar rcs $@ $^
$(LIBCDS):
	(cd $(LIBCDS_DIR) && make)
$(LIBHDT):
	(cd $(LIBHDT_DIR) && make)
$(LIBRDF):
	(cd $(LIBRDF_DIR) && make)
$(LIBHEADER):
	(cd $(LIBHEADER_DIR) && make)
$(LIBSEQUENCE):
	(cd $(LIBSEQUENCE_DIR) && make)
$(LIBBITSEQUENCE):
	(cd $(LIBBITSEQUENCE_DIR) && make)
$(LIBHUFFMAN):
	(cd $(LIBHUFFMAN_DIR) && make)
$(LIBUTIL):
	(cd $(LIBUTIL_DIR) && make)
$(LIBDICT):
	(cd $(LIBDICT_DIR) && make)
$(LIBTRIPLES):
	(cd $(LIBTRIPLES_DIR) && make)
$(LIBPERMUTATION):
	(cd $(LIBPERMUTATION_DIR) && make)
clean:
	rm -f $(OBJ) $(EXE)
cleanall:
	make clean  && (cd $(LIBCDS_DIR) && make clean) && (cd $(LIBCSD_DIR) && make clean) && (cd $(LIBHDT_DIR) && make clean) && (cd $(LIBHEADER_DIR) && make clean) && (cd $(LIBSEQUENCE_DIR) && make clean) && (cd $(LIBHUFFMAN_DIR) && make clean) && (cd $(LIBBITSEQUENCE_DIR) && make clean)  && (cd $(LIBUTIL_DIR) && make clean) && (cd $(LIBRDF_DIR) && make clean) && (cd $(LIBDICT_DIR) && make clean) && (cd $(LIBPERMUTATION_DIR) && make clean) && (cd $(LIBTRIPLES_DIR) && make clean)
