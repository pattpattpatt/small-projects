//Created by Alex Patton
//Nov. 15 2017
//Licensed under MIT license

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <random>
#include <semaphore.h>

using namespace std;

//constants
const int BUFFER_SIZE = 10;

//counter
int buff_count = 0;

//global buffer
int buffer[BUFFER_SIZE];

//semaphores
sem_t full_sem;
sem_t empty_sem;
pthread_mutex_t buff_lock;

//global thread arg structure
struct thread_info {
    int thread_id;
    pthread_t thread;
};

void init_sems(){
    sem_init(&full_sem, 0, 0);
    sem_init(&empty_sem, 0, 10);
    pthread_mutex_init(&buff_lock, nullptr);
}

void print_buffer (){
    for (int i=0; i < BUFFER_SIZE; i++){
        cout << buffer[i] << ' ';
    }
    cout << endl;
}

bool insert_item(int item){

    //if buffer is not full, acquire lock and insert
    if (buff_count < BUFFER_SIZE){
        buffer[buff_count] = item;

        //print success
        cout << "Producer [" << pthread_self() << "] produced item " << item << " in buffer " << buff_count << endl;

        buff_count++;
        return true;
    }

    //else return false
    else{
        return false;
    }
}

bool remove_item(int *item){

    //if buffer ! empty, extract and return true
    if (buff_count > 0){
        *item = buffer[buff_count-1];
        buffer[buff_count-1] = -1;

        //print true
        cout << "Consumer [" << pthread_self() << "] consumed item " << *item << " from buffer " << buff_count-1 << endl;

        //decrement counter
        buff_count--;
        return true;
    }

    //else return false
    else
        return false;
}

void * producer(void *param){
    //cout << "***Producer Thread [" << param << "]***" << endl;

    int item;

    while (1){
        //produce random wait time
        int wait_time = rand()%4;

        sleep(wait_time);

        //generate random item
        item = rand()%21;

        //wait if all buffers are full
        sem_wait(&empty_sem);

        //lock access to buffer with lock
        pthread_mutex_lock(&buff_lock);

        //testing
        //cout << "Producer Enter: " << pthread_self() << endl;

        if (insert_item(item));

        else
            cout << "BUFFER FULL!" << endl;

        //testing
        //print_buffer();
        //cout << "Producer Exit: " << pthread_self() << endl;

        //unlock buffer
        pthread_mutex_unlock(&buff_lock);

        //increment full semaphore
        sem_post(&full_sem);
    }
}

void * consumer(void *param){
    //cout << "***Consumer Thread [" << param << "]***" << endl;

    int item;

    while(1){
        //random wait time
        int wait_time = rand()%4;

        //reset item value for testing
        item = -10;

        sleep(wait_time);

        //wait if buffer empty
        sem_wait(&full_sem);

        //acquire lock
        pthread_mutex_lock(&buff_lock);

        //testing
        //cout << "Consumer Enter: " << pthread_self() << endl;

        //get item from buffer
        if (remove_item(&item));

        else
            cout << "BUFFER EMPTY!" << endl;

        //testing
        //print_buffer();
        //cout << "Consumer Exit: " << pthread_self() << endl;

        //release lock
        pthread_mutex_unlock(&buff_lock);

        //increment empty semaphore
        sem_post(&empty_sem);
    }
}

int main() {
    //local data
    int wait_seconds = 5; //default to 5 for debugging
    int prod_thread_count = 0;
    int cons_thread_count = 0;

    //initialize buffer pool to all -1
    for (int i = 0; i <  BUFFER_SIZE;i++){
        buffer[i] = -1;
    }

    //get user input
    cout << "Enter number of seconds for main to sleep: ";
    cin >> wait_seconds;

    cout << endl << "Enter number of producer threads: ";
    cin >> prod_thread_count;

    cout << endl << "Enter number of consumer threads: ";
    cin >> cons_thread_count;
    cout << endl;

    //initialize semaphores
    init_sems();

    //initialize thread arrays
    pthread_t prod_threads[prod_thread_count];
    pthread_t cons_threads[cons_thread_count];

    // initialize thread args
    vector<int> prod_id;
    vector<int> cons_id;

    //create producer threads
    for (int thread_cnt = 0; thread_cnt < prod_thread_count; thread_cnt++){
        //creat args
        prod_id.push_back(thread_cnt+1);
        void * arg = &prod_id[thread_cnt];

        //create thread
        int er = pthread_create(&prod_threads[thread_cnt], NULL, producer, arg);

        if (er != 0){
            cout << "Producer Thread [" <<  prod_id[thread_cnt] << "] id: [" << prod_threads[thread_cnt] << "] failed to initialize" << endl;
        }
        else;
            //cout << "Producer Thread [" << prod_id[thread_cnt] << "] id: [" << prod_threads[thread_cnt] << "] initialized successfully!" << endl;
    }

    //create consumer threads
    for (int thread_cnt = 0; thread_cnt < cons_thread_count; thread_cnt++){
        //creat args
        cons_id.push_back(thread_cnt+1);
        void * arg = &cons_id[thread_cnt];

        //create thread
        int er = pthread_create(&cons_threads[thread_cnt], NULL, consumer, arg);

        if (er != 0){
            cout << "Consumer Thread [" << cons_id[thread_cnt] << "] id: [" << cons_threads[thread_cnt] << "] failed to initialize" << endl;
        }
        else;
            //cout << "Consumer Thread [" << cons_id[thread_cnt]  << "] id: [" << cons_threads[thread_cnt] << "]initialized successfully!" << endl;
    }

    //sleep
    sleep(wait_seconds);

    //exit the program
    printf("Exiting main\n");
    return 0;
}
