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

SHELL    = /bin/sh

TOPDIR  := $(shell pwd)/..

TARGETS_DIR  = \
		$(TOPDIR)/src/libtmb \
		$(TOPDIR)/src/txgate \
		$(TOPDIR)/src/libtux \
		$(TOPDIR)/src/tmadmin \
		$(TOPDIR)/src/tmshutdown \
		$(TOPDIR)/src/buildserver \
		$(TOPDIR)/src/buildclient \
		$(TOPDIR)/src/viewc32 \
		$(TOPDIR)/src/viewdis32 \
		$(TOPDIR)/src/mkfldhdr32 \

all::
	@cd $(TOPDIR)/ext/lwpr/project; $(MAKE)
	@-mkdir $(TOPDIR)/include/lwpr
	@-cp $(TOPDIR)/ext/lwpr/src/*.h $(TOPDIR)/include/lwpr/
	@-mv $(TOPDIR)/include/lwpr/lwpr.h $(TOPDIR)/include/
	@-cp $(TOPDIR)/ext/lwpr/lib/* $(TOPDIR)/lib/
	@-cp $(TOPDIR)/ext/lwpr/project/makstand $(TOPDIR)/project/
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "making $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) ) || exit 1; \
	done

clean::
	@cd $(TOPDIR)/ext/lwpr/project; $(MAKE) clean
	@-cp $(TOPDIR)/ext/lwpr/project/makstand $(TOPDIR)/project/
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "clean $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) clean ) || exit 1; \
	done
