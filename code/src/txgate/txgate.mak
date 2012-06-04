#
# $Id: txgate.mak 108 2011-02-24 13:33:22Z vintage $
#
TOPDIR  = ../..

APPNAME = txgate

APPOBJS = \
		AppDepot.o \
		AutoConfig.o \
		GateConfig.o \
		GateNSHandlerImpl.o \
		HousekeepingThread.o \
		ShareCookie.o \
		txgate.o \
		txwsh.o \
		WaitNotifyThread.o \
		GateProxy.o \
		GateManager.o \
		txsync.o \

APP_CMPFLGS = -I$(TOPDIR)/src/libtmb

APP_LDDFLGS = -ltmb -llwpr

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
