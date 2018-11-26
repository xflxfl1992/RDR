CC = gcc
CXX = g++
CFLAGS = -O3 -march=native -fPIC -fomit-frame-pointer -Wall -pipe -minline-stringops-dynamically #-mfpmath=sse -ftracer
CXXFLAGS = $(CFLAGS)
CPPFLAGS = -Wp,-DNDEBUG,-D_FILE_OFFSET_BITS=64
ASFLAGS = -Wa,
LDFLAGS = -s -Wl,-O3


INCLDIR = -I. -I./include

LIBDIR = -L. -L./lib

LIBS = -lm -lz -lpthread -lalvcomm -lNJUST_MC_Decode -lNJUST_MO_LS_proc -lRmwLidar64Proc

EXE = 3D64

OBJS=\
	$(patsubst %.c,%.o,$(wildcard *.c)) \
	$(patsubst %.cpp,%.o,$(wildcard *.cpp)) \
	$(patsubst %.cxx,%.o,$(wildcard *.cxx)) \
	$(patsubst %.cc,%.o,$(wildcard *.cc))


.SUFFIXES: .c .cpp .cxx .cc .o

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(ASFLAGS) -c $*.c $(INCLDIR)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ASFLAGS) -c $*.cpp $(INCLDIR) 

.cxx.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ASFLAGS) -c $*.cxx $(INCLDIR)

.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ASFLAGS) -c $*.cc $(INCLDIR)



all : $(EXE)

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBDIR) $(LIBS)


clean:
	rm -f $(EXE) $(OBJS) bin/*.bin bin/*.txt /tmp/3D64.log
