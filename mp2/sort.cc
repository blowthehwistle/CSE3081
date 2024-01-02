#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cmath>
#include<stack>

using namespace std;





// Helper function for partitioning
int partition(int arr[], int low, int high) {
    int pivot_index = high;
    int pivot_value = arr[pivot_index];

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] >= pivot_value) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(int* arr, int low, int high){
    
        if (low < high) {
            // Partition the array into two halves and get the pivot index
            int pivotIndex = partition(arr, low, high);

            // Recursively sort the subarrays
            quicksort(arr, low, pivotIndex - 1);
            quicksort(arr, pivotIndex + 1, high);
        }   

}


int main(int argc, char* argv[]){
    int size;
    int *arr;

    string ifile = argv[1];
    string ofile = "input_reverse_";
    ofile.append(ifile);
    
    ifstream readFile;
    ofstream writeFile;

    
    //read array from the input file
    readFile.open(ifile.c_str());
    if(readFile.is_open()){
        readFile >> size;
        arr = new int[size];
        for(int i=0; i<size; i++){
            readFile >> arr[i];
        }
    }

    //sorting  
 
    quicksort(arr, 0, size-1);
 
      
    writeFile.open(ofile.c_str());
    if(writeFile.is_open()){
        writeFile << size << " ";
        for(int i = 0; i< size; i++){
            writeFile << arr[i] << " ";
        }
        writeFile << endl;
    }    
    writeFile.close();
    
    return 0;
}



