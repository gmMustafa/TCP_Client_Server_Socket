# TCP_Client_Server_Socket



### Server:
Server calls the socket call and then bind the server to the specific port and IP Address.
After binding server goes to listing state from the call of listen. When client connect to the
server, server accept call works and join the bridge by connecting the sockets on the both sides.
A new server process is created in result of fork() call which works as child process.
The parent socket descriptor in then closed in child process and the child socket descriptor is
closed in parent due to non-usage. And the Parent process goes back to listen state for further
client request acceptance. On the other hand, data transfer is conducted from the child process.
When data transfer completes the process finishes.

### Client:
Client call the socket call and then calls the connect call to form a connection after the
acceptance from server. Client request for reading and writing of data. After successful transfer
of data. The socket is closed. 

<img align="center" src="https://www.cs.dartmouth.edu/~campbell/cs60/TCPsockets.jpg" data-canonical-src="https://www.cs.dartmouth.edu/~campbell/cs60/TCPsockets.jpg" width="400" height="500" />
