APPNAME=$(shell basename $(shell pwd))
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.cpp=build/%.o)
EXEFILE=bin/$(APPNAME)

CC=mpicc
CFLAGS=-Wall -Wextra -fopenmp -std=c99
LIBS= -lm

LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

.SECONDEXPANSION:

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

$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

build/%.o: %.cpp | $$(@D)/.
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

.PHONY: memcheck
memcheck:
	valgrind $(EXEFILE)

.PHONY: helgrind
helgrind:
	valgrind --tool=helgrind --quiet $(EXEFILE)

.PHONY: rebuild
rebuild: clean $(EXEFILE)

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)

.PHONY: gitignore
gitignore:
	echo "bin/ build/" | tr " " "\n" > .gitignore

.PHONY: clean
clean:
	rm -rf bin/ build/
