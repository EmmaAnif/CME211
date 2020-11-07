#compiler used
CXX := g++
#compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra -Wconversion -fsanitize=address

#target to build
TARGET := main
#source files
SRCS := main.cpp matvecops.cpp CGSolver.cpp COO2CSR.cpp
#header files
HDRS := matvecops.hpp CGSolver.hpp COO2CSR.hpp
#object files
OBJS := *.o 

#dependencies and build command
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

#used to clean up all object files and main when called 
.PHONY : clean
clean:
	$(RM) $(TARGET) $(OBJS)
