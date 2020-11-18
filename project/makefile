#compiler used
CXX := g++
#compiler flags
CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion -fsanitize=address

#target to build
TARGET := main
#source files
SRCS := main.cpp matvecops.cpp CGSolver.cpp COO2CSR.cpp heat.cpp sparse.cpp
#header files
HDRS := matvecops.hpp CGSolver.hpp COO2CSR.hpp heat.hpp sparse.hpp
#object files
OBJS := *.o 
#text files
TXTS := wildcard sol*.txt

INPF := input0.txt
SOLF := solution0_0.txt
SOLP := solution0_

PYTH := python3
PPRC := postprocess.py
BONS := bonus.py

#dependencies and build command
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

#used to clean up all object files and main when called 
.PHONY : clean
clean:
	$(RM) $(TARGET) $(OBJS) $(TXTS)

#used to execute target
.PHONY : run
run:
	./$(TARGET)  $(INPF) $(SOLP)
	
.PHONY : postprocess
postprocess:
	$(PYTH) $(PPRC) $(INPF) $(SOLF)
	
.PHONY : bonus
bonus:
	$(PYTH) $(BONS) $(INPF) $(SOLP)
