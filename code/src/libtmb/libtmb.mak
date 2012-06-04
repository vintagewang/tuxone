#
# $Id: libtmb.mak 135 2011-03-05 10:21:58Z vintage $
#
TOPDIR  = ../..

APPNAME = tmb

APPOBJS = \
		BulletinBoard.o \
		Message.o \
		TuxNSHandlerImpl.o \
		View32Util.o \
		Fml32Util.o \

APP_CMPFLGS =

APP_LDDFLGS = -llwpr

# app build type: bldexe blddll bldslb bldexe_c blddll_c bldslb_c
APP_BUILDTYPE = blddll
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
