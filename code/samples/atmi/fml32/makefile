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

all: server client

clean:
	rm -f *.o
	rm -f server client
	rm -f friend.h

friend.h: friend.fml
	mkfldhdr32 $<

server.c client.c: friend.h

server: server.c
	buildserver -o $@ -f $< -s ADD_FRIEND

client: client.c
	buildclient -o $@ -f $<
