# define the commands we'll use for compilation and library building
AR = ar
ARFLAGS = rcs
CC = gcc
CXX = g++

# gtest
GTEST_DIR = ${HOME}/src/googletest-release-1.8.0/googletest
GOOGLE_TEST_LIB = ${HOME}/lib/gtest/libgtest.a
GOOGLE_TEST_INCLUDE = ${HOME}/src/googletest-release-1.8.0/googletest/include/
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# define useful flags to cc/ld/etc.
CFLAGS = -g -Wall -I. -I.. -Ihtll -I${GOOGLE_TEST_INCLUDE} 
LDFLAGS = -L. -L${GOOGLE_TEST_INCLUDE}
CPPUNITFLAGS = -L${GOOGLE_TEST_INCLUDE} -lgtest

#define common dependencies
OBJS = FileParser.o Movie.o MovieSet.o MovieIndex.o MovieReport.o 
HEADERS = FileParser.h Movie.h MovieSet.h MovieIndex.h MovieReport.h 
TESTOBJS = test_fileparser.o test_movieindex.o

# compile everything
all: test-fileparser test-movieindex example FORCE

test_suite: $(TESTOBJS) $(TESTHEADERS)  FORCE
	$(CXX) $(CFLAGS) -o test_suite $(TESTOBJS) \
	$(CPPUNITFLAGS) $(LDFLAGS) -lpthread $(LDFLAGS)

%.o: %.cc $(HEADERS) FORCE
	$(CXX) $(CFLAGS) -c $<

%.o: %.c $(HEADERS) FORCE
	$(CC) $(CFLAGS) -g -c  $<

example: $(OBJS) example_indexer.o
	@echo ===========================
	@echo Building example program
	@echo ===========================
	gcc -g $(OBJS) example_indexer.o htll/LinkedList.o htll/Hashtable.o Assert007.o \
		-o example
	@echo ===========================
	@echo Run example by running ./example
	@echo ===========================

test-fileparser:  $(GOOGLE_TEST_LIB) test_fileparser.o $(OBJS) Assert007.o
	@echo ===========================
	@echo Building fileparser test
	@echo ===========================
	g++ -o test_fileparser test_fileparser.o $(OBJS) htll/LinkedList.o htll/Hashtable.o Assert007.o  \
		 -L${HOME}/lib/gtest -lgtest -lpthread
	@echo ===========================
	@echo Run tests by running ./test_fileparser
	@echo ===========================



test-movieindex:  $(GOOGLE_TEST_LIB) test_movieindex.o $(OBJS) Assert007.o
	@echo ===========================
	@echo Building movie index test
	@echo ===========================
	g++ -o test_movieindex test_movieindex.o $(OBJS) htll/LinkedList.o htll/Hashtable.o Assert007.o  \
		 -L${HOME}/lib/gtest -lgtest -lpthread
	@echo ===========================
	@echo Run tests by running ./test_movieindex
	@echo ===========================


test_fileparser.o: test_fileparser.cc
	g++ -c -Wall -I $(GOOGLE_TEST_INCLUDE) -Ihtll test_fileparser.cc \
		-o test_fileparser.o 

clean: FORCE
	/bin/rm -f *.o *~ test_movieindex test_fileparser  example

FORCE:
