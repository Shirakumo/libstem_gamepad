CD ?= cd

ifeq ($(OS),Windows_NT)
    MAKEDIR ?= mkdir
    REMOVEDIR ?= rmdir /s
    CMAKE ?= cmake -G "NMake makefiles"
    MAKE ?= nmake
else
    MAKEDIR ?= mkdir -p
    REMOVEDIR ?= rm -r
    CMAKE ?= cmake
    MAKE ?= make
endif

all : library

library : clean 
	$(MAKEDIR) build
	$(CD) build && $(CMAKE) .. && $(MAKE)

clean :
	$(REMOVEDIR) build
