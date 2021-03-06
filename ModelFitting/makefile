# Inlcude general makefile settings
include ../makefile-common.make

# define any directories containing header files other than /usr/include
INCLUDES += -I ../Vesselness
INCLUDES += -I ../core
INCLUDES += -I ..

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS += -L../libs/Release
LFLAGS += `pkg-config --libs opencv` -Wl,--no-as-needed -pthread

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS += -lSparseMatrixCV -lSparseMatrix
#LIBS += -lGL -lGLU -lglut -lGLEW

# define the cpp source files
SRCS  = Line3D.cpp Line3DTwoPoint.cpp LevenbergMarquardt.cpp EnergyFunctions.cpp init_models.cpp
SRCS += ModelSet.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
# $(SRCS:.cpp=.o) 
OBJS = $(SRCS:%.cpp=./obj/%.o) 

# define the executable file 
TARGET = ../bin/modelfitting

all: $(CBLAS_OBJS) $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) $(OBJS) main_nvis.cpp -o $(TARGET) $(LFLAGS) $(LIBS)

# Yuchen: these following command will compile the other cpp files in the project
# For example, if there is a file SparseMatrix.cpp in the current directory, it will 
#   be compiled to SparseMatrix.o. That is equivalent to the following two lines of code. 
# SparseMatrix.o: SparseMatrix.cpp
#	$(CC) $(CFLAGS) $(INCLUDES) -c SparseMatrix.cpp -o SparseMatrix.o
./obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Yuchen: Removes all .o files and the excutable file, so that the next make rebuilds them
clean: 
	$(RM) ./obj/*.o $(TARGET)

