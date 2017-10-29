#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <fstream>


using namespace std;


//MACROS
const int ARRAY_SIZE = 20;
const char *INFILE = "p2data.txt";


//Global Error variables
int file_err=0;


//initialize the sorting array
int *input_array = new int[ARRAY_SIZE];
int *sorted_array = new int[ARRAY_SIZE];

//Create struct for thread args
typedef struct {
  int left;
  int right;
} sort_struct;


void print_array(const int array[ARRAY_SIZE]){
  //print results to the screen
  for (int i=0; i < ARRAY_SIZE; i++){
    //if (array[i] != 0)
      cout << array[i] << endl;
  }
}


void get_input_from_file(string infile){
  ifstream input_stream;
  input_stream.open(INFILE);

  int temp=0, j=0;
  //Store input in sort_array
  while (input_stream >> temp){
    //if input > 20, return error and wait for new file name
    if (j < ARRAY_SIZE){
      input_array[j] = temp;
      j++;
    }
    else{
      file_err = 1;

      //close file stream
      input_stream.close();

      return;
    }
  }

  //initialize remainder of array
  while (j<ARRAY_SIZE){
    input_array[j] = 0;
    j++;
  }
}


void *merge(void * params){
  //Testing code
  cout << "MERGE thread: " << pthread_self() << endl;
  //dereference pointer and get struct
  sort_struct *data = (sort_struct*)params;

  //get elements
  int mid = ((data->left + data->right) / 2);
  int left_count = data->left;
  int right_count = mid+1;


  //create and set looping variable to 0
  int merge_count = data->left;

  //loop until one sub_array is empty
  while (left_count <= mid && right_count <= data->right){
    //add new elements to the sorted array
    if (input_array[left_count] < input_array[right_count]){
      sorted_array[merge_count] = input_array[left_count];
      merge_count++;
      left_count++;
    }

    else if(input_array[left_count] >= input_array[right_count]){
      sorted_array[merge_count] = input_array[right_count];
      merge_count++;
      right_count++;
    }
  }

  //add the rest of the elements to the sorted array
  while (right_count <= data->right){
    sorted_array[merge_count] = input_array[right_count];
    merge_count++;
    right_count++;
  }

  while (left_count <= mid){
    sorted_array[merge_count] = input_array[left_count];
    merge_count++;
    left_count++;
  }

  //copy sorted list back to input_array
  for (int i=data->left; i<merge_count;i++){
    input_array[i] = sorted_array[i];
  }

  //return
  return 0;
}


void *mergesort(void * params){
  //testing code
  cout << "Mergesort thread: " << pthread_self() << endl;
  //dereference pointer and get struct
  sort_struct *data = (sort_struct*)params;

  //create middle of array
  int mid = ((data->left + data->right) / 2);

  //create structures to pass to threads
  sort_struct ldata, *ldata_ptr, rdata, *rdata_ptr;

  //initialize left data structures and pointer
  ldata = (sort_struct){data->left, mid};
  ldata_ptr = &ldata;

  //initialize right data structures and pointer
  rdata = (sort_struct){mid+1, data->right};
  rdata_ptr = &rdata;

  if (data->left < data->right){
    //create threads and return values
    pthread_t l_thread, r_thread, merge_thread;
    int lt_ret_val, rt_ret_val, merge_t_ret_val;

    //call thread 1
    lt_ret_val = pthread_create(&l_thread, NULL, mergesort, ldata_ptr);

    if (lt_ret_val != 0)
      cout << "Thread (left) failed to start" << endl;
	  //testing code
	  //mergesort(ldata_ptr);

    //call thread 2
    rt_ret_val = pthread_create(&r_thread, NULL, mergesort, rdata_ptr);
    if (rt_ret_val !=0)
      cout << "Thread (right) failed to start";

    //testing code
    //mergesort(rdata_ptr);

    //wait for both to return
    pthread_join(l_thread, NULL);
    pthread_join(r_thread, NULL);

    //call thread 3 to merge the two sorted arrays
    //merge_t_ret_val = pthread_create(&merge_thread, NULL, merge, data);
	merge(data);
  }

  return 0;
}

int main(int argc, char const *argv[]) {
  //initialize array from file
  get_input_from_file(INFILE);

  //check if the file had an error
  if (file_err != 0){
    cout << "ERROR: Input list larger than " << ARRAY_SIZE << endl;
    return 0;
  }

  //print_array(input_array);

  //initialize params
  sort_struct params, *param_ptr;
  params = (sort_struct){0,ARRAY_SIZE-1};
  param_ptr = &params;

  //call mergesort
  mergesort(param_ptr);

  print_array(sorted_array);

  return 0;
}

