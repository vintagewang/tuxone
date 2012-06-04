export CFLAGS=-m32
buildclient -o client -f client.c
buildserver -o server -f server.c -s ECHO
