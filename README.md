# TCP-Client-Server
Linux TCP Client Server code


# To build on Linux: build Server and Client
g++ server.cpp -o server

g++ client.cpp -o client

# To build on Windows: build Client 
Open the .sln file using VS 2015 or later and build using IDE

# run server first.
./server 

# get the IP address of the of the machine where server is running (server port is 12000). Use it as input to run the client. e.g:
./client 10.160.8.55 12000


