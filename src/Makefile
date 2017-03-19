##################################################
# Declare Variables
##################################################
CPP =/usr/bin/g++
CPPFLAGS = -std=c++14 -Wall -W -pedantic
DEBUG = -g
SERVEROBJECTS = wrapper.o server.o
CLIENTOBJECTS = wrapper.o client.o
SERVEREXECUTABLE = server
CLIENTEXECUTABLE = client
EXECUTABLES = $(SERVEREXECUTABLE) $(CLIENTEXECUTABLE)
CPP_COMP = $(CPP) $(CPPFLAGS) $(DEBUG) -c
CPP_LOAD = $(CPP) $(CPPFLAGS) $(DEBUG)

##################################################
# Targets
##################################################
all: $(EXECUTABLES)

.c.o:
	$(CPP_COMP) $*.cpp

server: $(SERVEROBJECTS)
	$(CPP_LOAD) $(SERVEROBJECTS) -o server

client: $(CLIENTOBJECTS)
	$(CPP_LOAD) $(CLIENTOBJECTS) -o client

##################################################
# Clean-up
##################################################
clean:
	rm $(EXECUTABLES)
	rm *.o
cleanobjects:
	rm *.o
