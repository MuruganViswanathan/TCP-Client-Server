# TCP-Client-Server
Linux TCP Client Server code


# To build on Linux: build Server and Client
g++ server.cpp -o server

g++ client.cpp -o client

# To build using toolchain:
For example I have used the ARM toolchain to cross-compile for my ARM based Target system:
The example files shown below (streamserver.cpp and streamclient.cpp) are located under ARM folder. 
After building for the ARM Target copy whichever you want to the Target. For e.g. I copied streamserverto target to run Target as a Server. And used client program from Host.

<your toolchain path>/arm-package-linux-gnueabi-gcc  streamserver.cpp -o streamserver

<your toolchain path>/arm-package-linux-gnueabi-gcc  streamclient.cpp -o streamclient


# To build on Windows: build Client 
Open the .sln file using VS 2015 or later and build using IDE

# How to Test?:
Run server first.
./server 

Get the IP address of the of the machine where server is running (server port is 12000). Use it as input to run the client.

e.g:
./client 10.160.8.55 12000

