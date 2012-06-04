#
# $Id: makefile 128 2011-03-17 14:49:36Z vintage $
#

SHELL    = /bin/sh

TOPDIR  := $(shell pwd)/..

TARGETS_DIR  = \
		$(TOPDIR)/src \
		$(TOPDIR)/tools/genconfig \
		$(TOPDIR)/tools/pmon \

all::
	@-mkdir $(TOPDIR)/include
	@-mkdir $(TOPDIR)/bin
	@-mkdir $(TOPDIR)/lib
	@-mkdir $(TOPDIR)/include/lwpr
	@-cp $(TOPDIR)/src/*.h $(TOPDIR)/include/lwpr/
	@-cp $(TOPDIR)/src/lwpr.h $(TOPDIR)/include/
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "making $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) ) || exit 1; \
	done

clean::
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "clean $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) clean ) || exit 1; \
	done
