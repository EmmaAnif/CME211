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

#object files and editor files
OBJS := *.o 
EDTR := *~

#default commandline arguments
INPF := input0.txt
SOLF := solution0_0.txt
SOLP := solution0_

#python interpreter
PYTH := python3

#python files
PPRC := postprocess.py
BONS := bonus.py

#dependencies and build command
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

#remove main, object files, solution files and editor files 
.PHONY : clean
clean:
	$(RM) $(TARGET) $(OBJS) $(EDTR)

#execute main
.PHONY : run
run:
	./$(TARGET)  $(INPF) $(SOLP)

#execute postprocess
.PHONY : postprocess
postprocess:
	$(PYTH) $(PPRC) $(INPF) $(SOLF)

#execute bonus
.PHONY : bonus
bonus:
	$(PYTH) $(BONS) $(INPF) $(SOLP)
