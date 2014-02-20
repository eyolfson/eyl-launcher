CXX = clang++
CXXFLAGS = -std=c++11 -fno-exceptions -fno-rtti
CXXFLAGS += $(shell pkg-config --cflags pangocairo sdl)
LDLIBS = $(shell pkg-config --libs pangocairo sdl2)

.PHONY: all
all: obj bin bin/lnchr

bin/lnchr: obj/lnchr.o
	@echo -e '\e[32;1m[link]\e[22m lnchr\e[m'
	@$(CXX) $(LDLIBS) $^ -o $@

obj/lnchr.o: src/lnchr.cpp
	@echo -e '\e[34;1m[compile]\e[22m lnchr.cpp\e[m'
	@$(CXX) $(CXXFLAGS) $< -c -o $@

obj:
	@mkdir obj

bin:
	@mkdir bin

.PHONY: clean
clean:
	@$(RM) -r obj
	@$(RM) -r bin
