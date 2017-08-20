#Makefile for Static library targets

#Build configuration. Either Release or Debug. This will default to release if not specified
ifndef (CFG)
	CFG=Release
endif

#The library name without the lib prefix or the .a suffix
LIBNAME = pxgf

CPPFLAGS += -DNO_LOG4CXX

#A space separated list of sub directories of source files to build into this library
SUBDIRS = . handlers handlers/packings

# Or alternatively you can specify the name of individual source files to build into this library
#LIB_SRCS_CPP =

include $(MAKEINCLUDE)/StaticLib.mk
