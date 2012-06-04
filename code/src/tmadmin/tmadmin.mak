#
# Copyright 2012 Wangxr, vintage.wang@gmail.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
TOPDIR  = ../..

APPNAME = tmadmin

APPOBJS = \
		tmadmin.o \

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
