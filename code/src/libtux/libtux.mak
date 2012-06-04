#
# $Id: libtux.mak 158 2011-03-13 10:45:04Z vintage $
#
TOPDIR  = ../..

APPNAME = tux

APPOBJS = \
		AppConfig.o \
		AppContext.o \
		AppNSHandlerImpl.o \
		ATMIAPI.o \
		ATMIBufferManager.o \
		AutoConfig.o \
		GateCheckThread.o \
		ServiceRepository.o \
		LocalServiceStub.o \
		TLSLibtux.o \
		ServerProxy.o \
		View32Manager.o \
		FML32API.o \

APP_CMPFLGS = -I$(TOPDIR)/src/libtmb

APP_LDDFLGS = -ltmb -llwpr

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
