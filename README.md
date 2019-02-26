# FlightReservation
Flight Reservation System using threads
In this Project I created a reservation system which has threads. Firstly, I planned to create Client thread and then selecting seats here using random function. After selecting seats I create server thread and send my seat request. If this seat is free Server thread assigned it to me in its critical section. 

To run my Project I create a make file. You can run it as follows:
make
./my 60 ( a number between 50 and 100)

Other than make file you can run it like that:
g++ 322.cpp -pthread -o my
./my 60 ( a number between 50 and 100)
Then the results are in output.txt file.
