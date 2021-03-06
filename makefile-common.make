# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -pg   adds profiling information to the executable file
#  -m64  compile for 64-bit
#  -Wno-comment disable warnings about multiline comments
CFLAGS  = -Wall -Wno-reorder -m64 -fopenmp -Wno-comment -O2 -Wl,--no-as-needed
# Add OpenCV include path
CFLAGS +=`pkg-config --cflags opencv`


# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS  = -L ./
LFLAGS += -Wl,--no-as-needed -pthread
LFLAGS += `pkg-config --libs opencv` # OpenCV libs
# LFLAGS += `python3-config --ldflags`  # Python libs, replace this line by the following 9 lines if you compile python yourself and have it under '../python'

# [IMPORTANT] The following assume that there is a local python intall under '../python'. 
# Include Path 
# INCLUDES += -I ../../python/include/
# define library paths
# LFLAGS += -L../../python/lib/
# Additiona libraries
# LIBS += -lpython3.4m -lpthread -ldl  -lutil -lm -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions



# for C++ define  CC = g++
#  -m64  compile for 64 bits
CC = g++ -std=c++0x 

# for c compiler
CCC = gcc -std=c99 

# archive (compile to lib*.a)
AR = ar -r -s

# Yuchen: The remove command.
RM = rm -f
