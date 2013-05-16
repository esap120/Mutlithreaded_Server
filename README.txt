
Each client should connect and send some number of lines each of which contains a 
positive integer, terminated by a -1.

The server creates a thread when the client connects and that thread sums up all the 
integers sent by the client and sends the sum to the client.

Server sends each client a second line that reads: "The current Grand Total is XXXXX 
and I have served YYYYY clients so far!", where the number XXXXX is the sum of the 
totals for ALL clients served and YYYYY is the total number of client requests.
