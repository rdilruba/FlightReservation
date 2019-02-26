#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
int number; // seat number given by user
int *seats; // seat array shows which client takes which seat
int *sarray; // this array keeps track of used seats (0-1)
ofstream outf("output.txt"); 

/* As a design choice in Server I only checked if the seat is reserved or not and 
 not assigned it in critical section.
 Other jobs like random seat selecting is done in Client */
void *Server(void *arr) {
    int *array = (int *)arr;
    int seatno;
    seatno = array[0];
    int id;
    id = array[1];
    
    if(seats[seatno]==0) // if seat is not selected yet.
        {
            pthread_mutex_lock(&lock);
            seats[seatno] = id; // assign it in critical section
            sarray[id] =1;
            pthread_mutex_unlock(&lock);
            outf <<"Client"<<id<< " reserves Seat"<< seatno << endl; // write it to file instantly
            pthread_exit(NULL);
        
            
        }
    pthread_exit(NULL);
}

// Client thread. I did seat selection here because in real life client chooses the seat.
void *Client(void *threadid) {
    
    int sleeptime = ((random()%151) + 50) / 1000;
    unsigned sleep(sleeptime);
    /* some necessary initializations for server thread creation */
    long tid;
    tid = (long)threadid;
    pthread_mutex_lock(&lock);
    int seatnumber = (random() % (number)) + 1; // randomly selecting seat number
    pthread_mutex_unlock(&lock);
    int* arr; // array to send server, clent id & its selected seat number
    arr = (int *)(malloc (sizeof(int)*3));
    arr[0] = seatnumber;
    arr[1] = tid;
    pthread_t server;
    pthread_attr_t attr;
    pthread_attr_init(&attr); // setting default attributes
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int success = 0; // success is 0 if seat is already reserved, 1 otherwise
    while(!success){ // tries to find a free seat
    pthread_create(&server,&attr,Server,arr); // creating server thread
    pthread_join(server,NULL);
    success = sarray[(int)tid];
    int seatnumber = (random() % (number)) + 1;
    arr[0] = seatnumber; 
    }
    free(arr); // releasing memory
    pthread_exit(NULL);
}

int main (int argc,char * argv[]) {
    srand((unsigned) time(NULL));
    number = atoi(argv[1]); // taking number of seats from user
    // creating arrays
    sarray = (int *)malloc(sizeof(int)*(number+1)); 
    seats = (int *)malloc(sizeof(int)*(number+1));
    pthread_t threads[number+1]; // thread array for clients
    int rc;
    long i;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // making threads joinable
    outf << "Number of total seats: "<< number <<endl;
    // creating of Client threads
    for( i = 1; i <= number; i++ ) {
        rc = pthread_create(&threads[i],&attr,Client,(void *)i); 

        if (rc) {
            outf << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    
    int j;
    // waiting for all client threads to finish
    for(j=1; j<= number; j++)
    {
        pthread_join(threads[j],NULL);
    }
    outf << "All seats are reserved." ;
    outf.close();
    free(sarray); // releasing memory
    free(seats); // releasing memory
    return 0;
}
