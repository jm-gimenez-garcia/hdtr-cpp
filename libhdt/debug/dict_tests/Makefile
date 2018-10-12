include ../Makefile.inc

SRC = testNewDictionary_reif.cpp  
OBJ = $(patsubst %.cpp,%.o,$(SRC))
EXE = $(patsubst %.cpp,%,$(SRC))


all: $(EXE) 
$(EXE) : $(OBJ) $(LIBUTIL) $(LIBCDS) $(LIBCSD) $(LIBHDT) $(LIBSEQUENCE)  $(LIBRDF) $(LIBDICT) $(LIBTRIPLES) 
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
$(LIBSEQUENCE):
	(cd $(LIBSEQUENCE_DIR) && make)
$(LIBUTIL):
	(cd $(LIBUTIL_DIR) && make)
$(LIBDICT):
	(cd $(LIBDICT_DIR) && make)
$(LIBTRIPLES):
	(cd $(LIBTRIPLES_DIR) && make)
clean:
	rm -f $(OBJ) $(EXE)
cleanall:
	make clean  && (cd $(LIBCDS_DIR) && make clean) && (cd $(LIBCSD_DIR) && make clean) && (cd $(LIBHDT_DIR) && make clean) && (cd $(LIBSEQUENCE_DIR) && make clean) && (cd $(LIBUTIL_DIR) && make clean) && (cd $(LIBRDF_DIR) && make clean) && (cd $(LIBDICT_DIR) && make clean) && (cd $(LIBTRIPLES_DIR) && make clean)
