CXX := g++

HDT_CPP_ROOT := ../../..
LOCAL_LIB_DIR := $(HDT_CPP_ROOT)/extern_libraries

LIBCDS_DIR := $(HDT_CPP_ROOT)/libhdt/src/libdcs
LIBCSD_DIR := $(HDT_CPP_ROOT)/libcds
LIBHDT_DIR := $(HDT_CPP_ROOT)/libhdt/src/hdt
LIBRDF_DIR := $(HDT_CPP_ROOT)/libhdt/src/rdf
LIBHEADER_DIR := $(HDT_CPP_ROOT)/libhdt/src/header
LIBPERMUTATION_DIR := $(HDT_CPP_ROOT)/libhdt/src/permutation
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
LIBPERMUTATION := $(LIBPERMUTATION_DIR)/libpermutation.a
LIBHEADER := $(LIBHEADER_DIR)/libheader.a
LIBSEQUENCE := $(LIBSEQUENCE_DIR)/libsequence.a
LIBBITSEQUENCE := $(LIBBITSEQUENCE_DIR)/libbitsequence.a
LIBHUFFMAN := $(LIBHUFFMAN_DIR)/libhuffman.a
LIBUTIL := $(LIBUTIL_DIR)/libutil.a
LIBDICT := $(LIBDICT_DIR)/libdict.a
LIBTRIPLES := $(LIBTRIPLES_DIR)/libtriples.a


CXXFLAGS := $(CXXFLAGS) -g -p -DHAVE_CDS -DHAVE_KYOTO -DHAVE_SERD -DHAVE_ZLIB -std=c++11 -I$(LOCAL_LIB_DIR)/include -I$(HDT_CPP_ROOT)/libhdt/include -I$(HDT_CPP_ROOT)/libhdt/src/dictionary -I$(HDT_CPP_ROOT)/libhdt/src/triples -I$(LIBCDS_DIR) -I$(LIBHDT_DIR) -I$(LIBSEQUENCE_DIR) -I$(LIBBITSEQUENCE_DIR) -I$(LIBPERMUTATION_DIR) -I$(LOCAL_LIB_DIR)/include/serd-0 -I$(LIBUTIL_DIR) -I$(LIBRDF_DIR) -I$(LIBDICT_DIR) -I$(LIBCSD_DIR)/include -I$(LIBTRIPLES_DIR) -I$(LIBHEADER_DIR) 

LDFLAGS := -pg -L$(LOCAL_LIB_DIR)/lib -L$(LIBCDS_DIR) -L$(LIBCSD_DIR) -L$(LIBHDT_DIR) -L$(LIBRDF_DIR) -L$(LIBHEADER_DIR) -L$(LIBSEQUENCE_DIR) -L$(LIBBITSEQUENCE_DIR) -L$(LIBUTIL_DIR) -L$(LIBDICT_DIR) -L$(LIBTRIPLES_DIR) -L$(LIBPERMUTATION_DIR) -L$(LIBHUFFMAN_DIR) -Wl,--start-group -Wl,-Bstatic -ldict -lcds -lcsd -lhdt -lheader -lrdf  -ltriples -lgzstream -lsequence -lhuffman -lbitsequence -lpermutation -lutil -Wl,--end-group -Wl,-Bdynamic -lkyotocabinet  -lz -lserd-0 -lpthread 
