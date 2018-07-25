CXX=g++

CXXFLAGS:= $(CXXFLAGS) -std=c++11 -I. -I../../include -I../hdt

SRC := $(wildcard ./Base*Dictionary.cpp ./Triples*Dictionary.cpp ./Graphs*Dictionary.cpp ./*ReificationDictionary.cpp) ../hdt/HDTFactory_dictionary.cpp TripleTranslator.cpp
OBJ := $(patsubst %.cpp,%.o,$(SRC)) 

STATIC_LIB := libdict.a

all: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ)
	ar rcs $@ $^ 
.o: .cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@
clean:
	rm -f $(OBJ) $(STATIC_LIB)
