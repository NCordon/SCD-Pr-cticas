// Calculates PI
#include <iostream>
#include <pthread.h>
using namespace std;

// Number of processors
const unsigned int n=4;
// Number of threads. Must be a multiple of n
const unsigned int m=2500;
double sums[m];

// Function f(x) = 4/(1+x^2)
void* f(void* x){
    double arg = 1.0 * (unsigned long) x/ m;
    sums[(unsigned long)x] = 4.0/(1+arg*arg);
}


int main(){
    pthread_t hebras[n];
    double sum = 0;
    void * arg;
    unsigned long x;

    for(unsigned int i=0; i<m/n; ++i){
	for(unsigned int j=0; j<n; ++j){
	    x = (i*n + j);
	    arg = (void*) x;
	    pthread_create (&(hebras[j]), NULL, f, arg);
	}
    }

    for(unsigned int j=0; j<n; ++j){
	pthread_join(hebras[j], NULL);
    }

    for(unsigned int j=0; j<m; ++j){
	sum += sums[j];
    }
    
    cout << "Pi vale: " << sum/m << endl;
}
