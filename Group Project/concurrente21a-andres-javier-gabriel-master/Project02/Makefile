CXX=mpic++ -std=c++11
FLAGS=$(strip -Wall -Wextra -std=c++11 -fopenmp)
LIBS=-pthread
LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-runtime/references

BIN_DIR=bin
OBJ_DIR=build
SRC_DIR=src
POS_OUTPUT=positive_output
NEG_OUTPUT=negative_output

DIRS=$(shell find $(SRC_DIR) -type d)
APPNAME=$(shell basename $(shell pwd))
HEADERS=$(wildcard $(DIRS:%=%/*.hpp))
SOURCES=$(wildcard $(DIRS:%=%/*.cpp))
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)
INCLUDE=$(DIRS:%=-I%)
IGNORES=$(BIN_DIR) $(OBJ_DIR)
EXEFILE=$(BIN_DIR)/$(APPNAME)
APPARGS=10 3 100 200 50

default: debug

debug: FLAGS += -g
debug: $(EXEFILE)
release: FLAGS += -O3 -DNDEBUG
release: $(EXEFILE)
asan: FLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: debug
msan: FLAGS += -fsanitize=memory
msan: CXX = clang++
msan: debug
tsan: FLAGS += -fsanitize=thread
tsan: debug
ubsan: FLAGS += -fsanitize=undefined
ubsan: debug

.SECONDEXPANSION:

$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(CXX) $(FLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $$(@D)/.
	$(CXX) -c $(FLAGS) $(INCLUDE) $< -o $@ $(LIBS)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

all: bin/$(APPNAME) lint memcheck helgrind

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)

.PHONY: memcheck
memcheck:
	valgrind --tool=memcheck bin/$(APPNAME)

.PHONY: helgrind
helgrind:
	valgrind --quiet --tool=helgrind bin/$(APPNAME)

.PHONY: callgrind
callgrind:
	valgrind --quiet --tool=callgrind bin/$(APPNAME)

.PHONY: gitignore
gitignore:
	echo $(IGNORES) > .gitignore

.PHONY: clean
clean:
	rm -rf $(IGNORES)

.PHONY: trash
trash:
	rm -rf $(POS_OUTPUT) $(NEG_OUTPUT)
