This library was written to debug calls to libopenal by preloading it.

Usage (example):

LD_PRELOAD='src/.libs/libopenal_debug.so' ./src/hello

Before starting the application, run: nc -4 -d -I1024 -k -l 10000 -n
because it connects to localhost port 10000 to write debug output.

