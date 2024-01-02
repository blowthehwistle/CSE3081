#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<ctime>
#include<string>

using namespace std;

int n6mss(int m, int n, int** arr);
int n4mss(int m, int n, int** arr);
int n3mss(int m, int n, int** arr);

int main(int argc, char* argv[]){
    int m, n, result;
    int **arr;
    clock_t start, finish;
    double duration;
    ifstream readFile;
    ofstream writeFile;
    
    string ifile = argv[1];
    int idx = atoi(argv[2]);

    readFile.open(ifile.c_str());
    if(readFile.is_open()){
        readFile >> m >> n;
        arr = new int*[m];
        for(int i=0; i<m; i++){
            arr[i] = new int[n];
        }

        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                readFile >> arr[i][j];
            }
        }
    }else cout << "readFile 안열림" << endl;
    readFile.close();
    
    start = clock();
    switch(idx){
        case 1:
            result = n6mss(m, n, arr);
            break;

        case 2:
            result = n4mss(m, n, arr);
            break;

        case 3:
            result = n3mss(m, n, arr);
            break;

        default:
            cout << "index error!" << endl;
            break;
    }
    finish = clock();
    duration = (double)(finish - start);
    cout << result << endl;
    cout << duration << endl;
    for(int i=0; i<m; i++){
        delete[] arr[i];
    }
    delete arr;
    

    string ofile = "result_";
    ofile.append(ifile);
  
    writeFile.open(ofile.c_str());
    if(writeFile.is_open()){
        writeFile << ifile << endl;
        writeFile << argv[2] << endl;
        writeFile << m << endl;
        writeFile << n << endl;
        writeFile << result << endl;
        writeFile << duration << endl;
    }    
    writeFile.close();
    return 0;
}   


int n6mss(int m, int n, int** arr){
    int mss, thissum;
    mss = arr[0][0];
    for(int i1=0; i1<m; i1++){
        for(int j1 = 0; j1<n; j1++){
            for(int i2 = i1; i2 < m; i2++){
                for(int j2 = j1; j2<n; j2++){
                    thissum = 0;                    
                    for(int i = i1; i <= i2; i++){
                        for(int j = j1; j<=j2; j++){
                            thissum += arr[i][j];
                        }
                    }
                    if(thissum > mss) mss = thissum;
                }
            }
        }
    }

    return mss;
}
int n4mss(int m, int n, int** arr){
    int mss, linesum = 0;
    int *ssum = new int[n];
    fill_n(ssum, n, 0);
    mss = arr[0][0];
    for(int i1=0; i1<m; i1++){
        for(int j1 = 0; j1<n; j1++){
            fill_n(ssum, n, 0);
            linesum = 0;
            for(int i2 = i1; i2 < m; i2++){
                linesum = 0;
                for(int j2 = j1; j2<n; j2++){
                    linesum += arr[i2][j2];
                    ssum[j2] += linesum;
                    if(ssum[j2] > mss) mss = ssum[j2];
                }
            }
        }
    }
    delete[] ssum;
    return mss;
}
int n3mss(int m, int n,int** arr){
    int mss;
    int colsum[m], thissum;
    int ssum[m][n];
    mss = arr[0][0];
    for(int c1=0; c1<n; c1++){
        fill_n(colsum, m, 0);
        for(int c2 = c1; c2<n; c2++){
            for(int r = 0; r<m; r++){
                colsum[r] += arr[r][c2];
            }
            thissum = 0;
            for(int i=0; i<m; i++){
                thissum += colsum[i];
                if(thissum > mss) mss = thissum;
                else if(thissum < 0) thissum = 0; 
            }
        }
    }
    return mss;


}