BIN:= timer 
INSTDIR := 
INSTDIR1 := 
INSTDIR2 := 
INSTDIR3 := 

ROOT_PATH=./
PHISHING_ROOT=$(ROOT_PATH)
#MALWARE_ROOT=$(PHISHING_ROOT)/models/malware_enginee/
#BAYS_ROOT=$(PHISHING_ROOT)/models/bayes_classifier/

INCS := ./ \
	./libevent

LIBPATHS := -lpthread \
	-L./libevent/libs -levent

USER_MARCOS := _POSIX_THREADS 
#CFLAGS = -static -Wall -g -s
CFLAGS = -g -D_DEBUG_INTERNAL
CC = g++

#SOURCE := $(wildcard *.cpp *.cc models/*.cpp)
SOURCE := $(wildcard *.cpp *.cc ../../../proto/*.cc)

OBJS := $(patsubst %.cpp,%.o,$(SOURCE)) 

%.o:%.cpp
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@

%.o:%.cc
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@

$(BIN): $(OBJS)	
	$(CC) $(CFLAGS) $(addprefix -I,$(INCS)) -o $(BIN) $(OBJS) $(LIBPATHS) 

clean :  
	rm -rf *.d *.o *.lo $(BIN)
 
realclean: clean  
	rm -rf *.o *.d *.lo  $(EXECUTABLE)  

install: $(EXECUTABLE)
	@if [ -d $(INSTDIR) ]; then \
	  cp $(EXECUTABLE) $(INSTDIR); \
	  chmod a+x $(INSTDIR)/$(EXECUTABLE); \
	  chmod og-w $(INSTDIR)/$(EXECUTABLE); \
	  echo "Installed in $(INSTDIR)"; \
	else \
	  echo "$(INSTDIR) doesn't exist"; \
	fi
	
	@if [ -d $(INSTDIR1) ]; then \
	  cp $(EXECUTABLE) $(INSTDIR1); \
	  chmod a+x $(INSTDIR1)/$(EXECUTABLE); \
	  chmod og-w $(INSTDIR1)/$(EXECUTABLE); \
	  echo "Installed in $(INSTDIR1)"; \
	else \
	  echo "$(INSTDIR) doesn't exist"; \
	fi
	
	@if [ -d $(INSTDIR2) ]; then \
	  cp $(EXECUTABLE) $(INSTDIR2); \
	  chmod a+x $(INSTDIR2)/$(EXECUTABLE); \
	  chmod og-w $(INSTDIR2)/$(EXECUTABLE); \
	  echo "Installed in $(INSTDIR2)"; \
	else \
	  echo "$(INSTDIR) doesn't exist"; \
	fi
	
	@if [ -d $(INSTDIR3) ]; then \
	  cp $(EXECUTABLE) $(INSTDIR3); \
	  chmod a+x $(INSTDIR3)/$(EXECUTABLE); \
	  chmod og-w $(INSTDIR3)/$(EXECUTABLE); \
	  echo "Installed in $(INSTDIR3)"; \
	else \
	  echo "$(INSTDIR) doesn't exist"; \
	fi
	
DEPEND= makedepend $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS))
depend:$(SOURCE)
	$(DEPEND) $(SOURCE)


	
# DO NOT DELETE
