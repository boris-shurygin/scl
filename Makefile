#
# @file Makefile
# Makefile for ShowGraph project
# @author Boris Shurygin
#
# Main targets are:
#   all - build SCL in debug and release modes
#   debug (not yet implemented) - build debug vesion of SCL
#   release - build release version of SCL
#   doc - run doxygen to generate documentation from source code
#
#   targets are buit in two steps:
#     1. Generate additional .cpp files by QT tools ( MOC and RCC)
#     2. Compile all .cpp to .o and link them
#


#Makefile that implements main building functionality
MAKEFILE_IMPL = Makefile-impl


# Tools
export CC = gcc
export CXX = g++
exportPERL = perl
export MKDIR = mkdir
export RM = rm
export GREP = grep
export TOUCH = touch
export DOXYGEN = doxygen

#Directories
export BIN_DIR := bin
export OBJECT_DIR := objects
export SOURCES := sources
export DEBUG_OBJECTS_DIR := $(OBJECT_DIR)/debug
export RELEASE_OBJECTS_DIR := $(OBJECT_DIR)/release
	
HEADERS:= $(wildcard $(SOURCES)/*/*.h $(SOURCES)/Core/*/*.h)

#
# All targets
#
all: release debug

#	
#Debug targets
#
debug: utestd

utestd: gen utestd_impl

utestd_impl:
	$(MAKE) -f $(MAKEFILE_IMPL) $@

#
# Additional generation target (if ever needed)
#
gen:

#
# Release targets
#
release: utest

utest: gen utest_impl

utest_impl:
	$(MAKE) -f $(MAKEFILE_IMPL) $@

#
# Documentation
#
doc: 
	@echo [doxygen]
	@cd autodoc;$(DOXYGEN) Doxyfile
#
# Cleanup routines
#
.PHONY: clean

clean:
	$(eval EXISTING_MOCS = $(wildcard $(SOURCES)/*/*_moc.cpp))
	$(eval EXISTING_RESOURCES = $(RESOURCES:.qrc=.cpp))
	-$(RM) -f $(EXISTING_RESOURCES)
	-$(RM) -f $(EXISTING_MOCS)
	-$(RM) -rf $(OBJECT_DIR)
	-$(RM) -rf $(BIN_DIR)

	



 
