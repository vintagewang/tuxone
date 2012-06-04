#
# $Id: viewc32.mak 121 2011-02-27 08:27:36Z vintage $
#
TOPDIR  = ../..

APPNAME = viewc32

APPOBJS = \
		viewc32.o \
		$(TOPDIR)/src/viewc32/Viewc32Cmdline.o \

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
