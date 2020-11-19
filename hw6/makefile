#compiler used
CXX := g++
#compiler flags
CXXFLAGS :=  -O3 -std=c++11 -Wall -Wextra -Wconversion -pedantic

CPPFLAGS := -DDEBUG 

LDFLAGS := -ljpeg

#target to build
TARGET := main
#source files
SRCS := hw6.cpp image.cpp main.cpp
#header files
HDRS := hw6.hpp image.hpp
#object files and editor files
OBJS := *.o 
EDTR := *~

#dependencies and build command
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

#used to clean up all object files and main when called 
.PHONY : clean
clean:
	$(RM) $(TARGET) $(OBJS) $(EDTR)

.PHONY : run
run: 
	./$(TARGET)
