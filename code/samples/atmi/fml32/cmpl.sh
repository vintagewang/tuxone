export CFLAGS="-m32 -g"
buildclient -o client -f client.c
buildserver -o server -f server.c -s ADD_FRIEND
