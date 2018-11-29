CXX=g++

CXXFLAGS := $(CXXFLAGS) -std=c++11 -I. -I../../include -I../../../extern_libraries/include/serd-0 

SRC :=  $(wildcard ./*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC)) 

STATIC_LIB := librdf.a

all: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ)
	ar rcs $@ $^ 
.o: .cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@
clean:
	rm -f $(OBJ) $(STATIC_LIB)
