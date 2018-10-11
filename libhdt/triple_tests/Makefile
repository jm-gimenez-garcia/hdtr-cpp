CXX := g++

HDT_CPP_ROOT ?= /home/gt78520h/fslahc.ujmse.local_ZRR/hdt-2-cpp_github_merged
# LOCAL_LIB_DIR := $(HDT_CPP_ROOT)/extern_libraries
LOCAL_LIB_DIR := /home/gt78520h/local

LIBCDS_DIR := $(HDT_CPP_ROOT)/libhdt/src/libdcs
LIBCSD_DIR := $(HDT_CPP_ROOT)/libcds
LIBHDT_DIR := $(HDT_CPP_ROOT)/libhdt/src/hdt
LIBRDF_DIR := $(HDT_CPP_ROOT)/libhdt/src/rdf
LIBHEADER_DIR := $(HDT_CPP_ROOT)/libhdt/src/header
LIBSEQUENCE_DIR := $(HDT_CPP_ROOT)/libhdt/src/sequence
LIBBITSEQUENCE_DIR := $(HDT_CPP_ROOT)/libhdt/src/bitsequence
LIBHUFFMAN_DIR := $(HDT_CPP_ROOT)/libhdt/src/huffman
LIBUTIL_DIR := $(HDT_CPP_ROOT)/libhdt/src/util
LIBDICT_DIR = $(HDT_CPP_ROOT)/libhdt/src/dictionary
LIBTRIPLES_DIR = $(HDT_CPP_ROOT)/libhdt/src/triples


LIBCDS := $(LIBCDS_DIR)/libcds.a
LIBCSD := $(LIBCSD_DIR)/libcsd.a
LIBHDT := $(LIBHDT_DIR)/libhdt.a
LIBRDF := $(LIBRDF_DIR)/librdf.a
LIBHEADER := $(LIBHEADER_DIR)/libheader.a
LIBSEQUENCE := $(LIBSEQUENCE_DIR)/libsequence.a
LIBBITSEQUENCE := $(LIBBITSEQUENCE_DIR)/libbitsequence.a
LIBHUFFMAN := $(LIBHUFFMAN_DIR)/libhuffman.a
LIBUTIL := $(LIBUTIL_DIR)/libutil.a
LIBDICT := $(LIBDICT_DIR)/libdict.a
LIBTRIPLES := $(LIBTRIPLES_DIR)/libtriples.a


SRC = testNewTriple.cpp  
OBJ = $(patsubst %.cpp,%.o,$(SRC))
EXE = $(patsubst %.cpp,%,$(SRC))


CXXFLAGS := $(CXXFLAGS) -std=c++11 -I$(LOCAL_LIB_DIR)/zlib/include -I$(LOCAL_LIB_DIR)/kyotocabinet/include -I$(HDT_CPP_ROOT)/libhdt/include -I$(HDT_CPP_ROOT)/libhdt/src/dictionary -I$(HDT_CPP_ROOT)/libhdt/src/triples -I$(LIBCDS_DIR) -I$(LIBHDT_DIR) -I$(LIBSEQUENCE_DIR) -I$(LIBBITSEQUENCE_DIR) -I $(LOCAL_LIB_DIR)/serd/include/serd-0 -I$(LIBUTIL_DIR) -I$(LIBRDF_DIR) -I$(LIBDICT_DIR) -I$(LIBCSD_DIR)/include -I$(LIBTRIPLES_DIR) -I$(LIBHEADER_DIR) 

LDFLAGS := -L$(LOCAL_LIB_DIR)/serd/lib -L$(LOCAL_LIB_DIR)/zlib/lib -L$(LOCAL_LIB_DIR)/gzstream/lib -L$(LOCAL_LIB_DIR)/kyotocabinet/lib -L$(LIBCDS_DIR) -L$(LIBCSD_DIR) -L$(LIBHDT_DIR) -L$(LIBRDF_DIR) -L$(LIBHEADER_DIR) -L$(LIBSEQUENCE_DIR) -L$(LIBBITSEQUENCE_DIR) -L$(LIBUTIL_DIR) -L$(LIBDICT_DIR) -L$(LIBTRIPLES_DIR) -L$(LIBHUFFMAN_DIR)  -Wl,-Bstatic -ldict -lcds -lcsd -lhdt -lheader -lrdf  -ltriples -lgzstream -lsequence -lhuffman -lbitsequence -lutil  -Wl,-Bdynamic -lkyotocabinet  -lz -lserd-0 -lpthread 

all: $(EXE) 
$(EXE) : $(OBJ) $(LIBUTIL) $(LIBCDS) $(LIBCSD) $(LIBHDT)  $(LIBRDF) $(LIBDICT) $(LIBTRIPLES) $(LIBHEADER) $(LIBSEQUENCE) $(LIBBITSEQUENCE) $(LIBHUFFMAN)
	$(CXX) $<  $(LDFLAGS)  -o $@ 
%.o: %.cpp 
	$(CXX) -c $^ $(CXXFLAGS)  -o $@ 


$(LIBCSD):
	(cd $(LIBCSD_DIR) && make)
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
clean:
	rm -f $(OBJ) $(EXE)
cleanall:
	make clean  && (cd $(LIBCDS_DIR) && make clean) && (cd $(LIBCSD_DIR) && make clean) && (cd $(LIBHDT_DIR) && make clean) && (cd $(LIBHEADER_DIR) && make clean) && (cd $(LIBSEQUENCE_DIR) && make clean) && (cd $(LIBHUFFMAN_DIR) && make clean) && (cd $(LIBBITSEQUENCE_DIR) && make clean)  && (cd $(LIBUTIL_DIR) && make clean) && (cd $(LIBRDF_DIR) && make clean) && (cd $(LIBDICT_DIR) && make clean) && (cd $(LIBTRIPLES_DIR) && make clean)
