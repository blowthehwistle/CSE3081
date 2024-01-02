#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cmath>
#include<stack>

using namespace std;


void insertionsort(int* arr, int low, int size){
    
    for (int i = low+1; i < size; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int median_of_three(int arr[], int low, int high) {
    int mid = (low + high) / 2;
    if (arr[low] > arr[mid]) {
        swap(arr[low], arr[mid]);
    }
    if (arr[mid] > arr[high]) {
        swap(arr[mid], arr[high]);
    }
    if (arr[low] > arr[mid]) {
        swap(arr[low], arr[mid]);
    }
    return mid;
}


// Helper function for partitioning
int partition(int arr[], int low, int high) {
    int pivot_index = high;
    int pivot_value = arr[pivot_index];

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot_value) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(int* arr, int low, int high){
    
        if (low < high) {
            int pivotIndex = partition(arr, low, high);

            quicksort(arr, low, pivotIndex - 1);
            quicksort(arr, pivotIndex + 1, high);
        }   

}

void merge(int arr[], int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int L[n1], R[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[middle + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergesort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergesort(arr, left, middle);
        mergesort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

int improved_partition(int arr[], int low, int high) {
    int pivot_index = median_of_three(arr, low, high);
    int pivot_value = arr[pivot_index];

    swap(arr[pivot_index], arr[high]);
    pivot_index = high;
        
    int i = low;
    int j = low;
    int k = high;

    while (j <= k) {
        if (arr[j] < pivot_value) {
            swap(arr[i], arr[j]);
            i++;
            j++;
        } else if (arr[j] == pivot_value) {
            j++;
        } else {
            swap(arr[j], arr[k]);
            k--;
        }
    }
    return i;
}

void improved_quicksort(int arr[], int low, int high) {
    if (high - low > 25) {
        int pivotIndex = improved_partition(arr, low, high);

        improved_quicksort(arr, low, pivotIndex - 1);
        improved_quicksort(arr, pivotIndex + 1, high);
    } else {
        insertionsort(arr, low, high);
    }
}

void custom_sort(int arr[], int low, int high){
    int count = 0;
    if (high - low > 1000){
        int testsize = (high-low) / 100;
        if(testsize > 1000) 
            testsize = 1000;
        for(int i=0; i<testsize; i++){
            if(arr[low+i] >= arr[high-i]){
                count++;
            }
        }
        if(count > testsize*8/10)
            mergesort(arr, low, high);
        else
            improved_quicksort(arr, low, high);
    }
    else{
        improved_quicksort(arr, low, high);
    }
}

int main(int argc, char* argv[]){
    int size;
    int *arr;
    int algorithm = atoi(argv[2]);

    string ifile = argv[1];
    string ofile = "result_";

    ifstream readFile;
    ofstream writeFile;

    clock_t start, finish;
    double duration;

    
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
    start = clock();
    switch(algorithm){
        case 1:
            insertionsort(arr, 0, size);
            break;
        case 2:
            quicksort(arr, 0, size-1);
            break;
        case 3:
            mergesort(arr, 0, size-1);
            break;
        case 4:
            custom_sort(arr, 0, size-1);
            break;
        default:
            break;
    }
    finish = clock();
    duration = (double)(finish - start)/CLOCKS_PER_SEC;

    ofile.push_back(algorithm+'0');
    ofile.append("_");
    ofile.append(ifile);
  
    writeFile.open(ofile.c_str());
    if(writeFile.is_open()){
        writeFile << ifile << endl;
        writeFile << algorithm << endl;
        writeFile << size << endl;
        writeFile << duration << endl;
        for(int i = 0; i< size; i++){
            writeFile << arr[i] << " ";
        }
        writeFile << endl;
    }    
    writeFile.close();
    delete arr;

    return 0;
}



