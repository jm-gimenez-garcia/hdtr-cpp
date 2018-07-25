CXX=g++

SRC = $(wildcard ./src/utils/*.cpp ./src/static/*/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
LIBCSD=libcsd.a

CXXFLAGS := -I./include


$(info $$SRC is [${SRC}])

all: $(LIBCSD)

$(LIBCSD): $(OBJ)
	ar rcs $@ $^

.o: .cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
