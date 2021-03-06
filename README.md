# TCP_Client_Server_Socket

TCP Concurrent Sever to handle multiple clients.

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

<p align="center">
  <img width="400" height="500" src="https://www.cs.dartmouth.edu/~campbell/cs60/TCPsockets.jpg">
</p>


## Authenticated File Transfer Application
Notes
* File to be transferred should be .txt files.
* Files should not contain any empty lines.
* Account details are in database.txt file.
* Run server terminal in server folder at path
* To Run on Your machine, Change the Path Accordingly.
* The database format is (Username,Password).
* The name representation of file generated in client is file_RandomNumber_Username.txt.

## Run
###Open Terminal in current working directory.

* Build output file for server.c using 
```
gcc server.c -o server.out
```

* Run Server using 
```
./server.out
```

You can built multiple client using multiple terminals

* Build output file for client.c using 
```
gcc client.c -o client.out
```

* Run Server using 
```
./client.out
```
