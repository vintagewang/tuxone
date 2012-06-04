#
# $Id: mkfldhdr32.mak 127 2011-02-27 15:38:03Z vintage $
#
TOPDIR  = ../..

APPNAME = mkfldhdr32

APPOBJS = \
		mkfldhdr32.o \
		Mkfldhdr32Cmdline.o \

APP_CMPFLGS  = -I$(TOPDIR)/src/libtmb

APP_LDDFLGS  = -ltmb -llwpr

# app build type: bldexe blddll bldslb bldexe_c blddll_c bldslb_c
APP_BUILDTYPE = bldexe
all: $(APP_BUILDTYPE)
clean: cleanup

bldexe blddll bldslb bldexe_c blddll_c bldslb_c cleanup:
	@TOPDIR="$(TOPDIR)"; \
	APPNAME="$(APPNAME)"; \
	APPOBJS="$(APPOBJS)"; \
	APP_CMPFLGS="$(APP_CMPFLGS)"; \
	APP_LDDFLGS="$(APP_LDDFLGS)"; \
	APP_BUILDTYPE="$(APP_BUILDTYPE)"; \
	export TOPDIR APPNAME APPOBJS APP_CMPFLGS APP_LDDFLGS APP_BUILDTYPE; \
	$(MAKE) -f $(TOPDIR)/project/makstand $@
