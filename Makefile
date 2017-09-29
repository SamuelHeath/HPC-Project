# A Makefile to build the percolation project.
# author: Abrar Amin (21518928@student.uwa.edu.au)
# author: Samuel Heath (2.....@student.uwa.edu.au)

#--------------------------------------------
EXE = percolation
HEADERS = $(EXE).h
CPP11 = g++ -std=c++11
CPPFLAGS = -Wall -Werror -pedantic
LIBFLAGS = -fopenmp -lm
LIB_LAT = lattice
LIB_ALG = algorithms
OBJ = $(LIB_LAT).o $(LIB_ALG).o
RM = /bin/rm
#---------------------------------------------

main : $(OBJ)
	$(CPP11) $(CPPFLAGS) -o $(EXE) $(LIB_LAT).o main.cpp $(LIBFLAGS)

%.o : %.cpp $(HEADERS) 
	$(CPP11) $(CPPFLAGS) -c $< $(LIBFLAGS)

clean:
	$(RM) -f $(EXE) $(OBJ)
