CXX = xcrun clang++
OBJ = obj/
SRC = src/
BIN = bin/
POINTCPP = $(wildcard $(SRC)*.cpp) $(wildcard $(SRC)*/*.cpp)
POINTOP := $(POINTCPP:.cpp=.o)
POINTO = $(patsubst src/%,$(OBJ)%,$(POINTOP)) #$(POINTOP:src=obj)

OPT := -Wall -pedantic -Wno-long-long -O2 -g -std=c++11 -stdlib=libc++ -I "$(SRC)"

ifeq ($(SHELL), sh.exe) 
OS := Win
else
OS := $(shell uname)
endif

ifeq ($(OS), Win)
EXEC := MangaDown
else
EXEC := MangaDown
endif

ifeq ($(OS), Linux)
RM = rm
LIBS := -lsfml-network -lsfml-system
endif
ifeq ($(OS), Darwin)
RM = rm
LIBS := -framework sfml-system -framework sfml-network
endif
ifeq ($(OS), Win)
RM = del
LIBS :=-lsfml-network -lsfml-system
endif


all : dirs $(EXEC)

dirs : 
ifeq ($(OS), Darwin)
	@./configure.sh bin obj src
endif
ifeq ($(OS), Linux)
	@./configure.sh bin obj src
endif
	
ifeq ($(OS), Win)
	@echo "You need to manually create subdirectories"
endif
.PHONY : dirs

clean: 
	@$(RM) -vf $(OBJ)*.o $(OBJ)*/*.o $(BIN)*
.PHONY : clean


$(OBJ)%.o : $(SRC)%.cpp
	@echo "Compiling $^"
	@$(CXX) $(OPT) $^ -c -o $@


$(EXEC) : $(POINTO)
	@echo "Linking $@"
	@$(CXX) $(OPT) $^ -o $(BIN)$(EXEC) $(LIBS)

valgrind : dirs Test
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$(EXEC)
