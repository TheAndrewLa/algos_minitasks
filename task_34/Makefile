CXX = clang++
COMPILE_OPTS = -std=c++20 -Wall -DTEST

EXE_CLANG = build/solution_exe

IN = $(wildcard tests/*.in)
ACT = $(IN:.in=.act)
PASSED = $(IN:.in=.passed)

.PHONY: all test clean clean-tests

all: clean-tests test

build:
	mkdir -p build

$(EXE_CLANG): main.cpp | build
	$(CXX) $(COMPILE_OPTS) main.cpp -o $@

test: $(PASSED)
	@echo "All tests passed"

$(PASSED): %.passed: %.in %.out $(EXE_CLANG)
	@echo "Running test $(*:tests/%=%)..."
	./$(EXE_CLANG) < $*.in > $*.act
	diff $*.out $*.act -Z
	@touch $@

clean-tests:
	@rm -f $(PASS)
	@rm -f $(ACT)

clean: clean-tests
	@rm -rf build
