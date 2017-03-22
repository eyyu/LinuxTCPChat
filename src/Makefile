##################################################
# Declare Variables
##################################################
CPP =/usr/bin/g++
CPPFLAGS = -std=c++14 -Wall -W -pedantic -g
DEBUG = -g
SERVEROBJECTS = wrapper.o server.o
CLIENTOBJECTS = wrapper.o client.o
SERVEREXECUTABLE = server
CLIENTEXECUTABLE = client
EXECUTABLES = $(SERVEREXECUTABLE) $(CLIENTEXECUTABLE)
CPP_COMP = $(CPP) $(CPPFLAGS)  -c
CPP_LOAD = $(CPP) $(CPPFLAGS)

##################################################
# Targets
##################################################
all:$(EXECUTABLES)

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
	rm *.o
	rm -rf *.h.gch
	rm $(EXECUTABLES)
cleanobjects:
	rm *.o
