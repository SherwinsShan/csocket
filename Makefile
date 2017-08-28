TOPPATH=$(shell pwd)
include config.mk

.PHONY :lib example pureclean libclean libpureclean exampleclean

all:example

lib:
	@cd src && make

example:lib
	@cd example && make

clean: libclean exampleclean

pureclean:libpureclean exampleclean 


libclean:
	@cd src && make clean

libpureclean: 
	@cd src && make pureclean 


exampleclean:
	@cd example && make clean


