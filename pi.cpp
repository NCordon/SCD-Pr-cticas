// Calculates PI
#include <iostream>
#include <pthread.h>
using namespace std;

// Number of processors
const unsigned int n=4;
// Number of threads. Must be a multiple of n
const unsigned int m=9000;
double sums[m];

// Function f(x) = 4/(1+x^2)
void* f(void* x){
    unsigned long j = (unsigned long) x;
    double arg;
    
    for(unsigned int i=0; i<m/n; ++i){
        arg = (i + j*m/n);
        arg = arg/m;
        sums[j] += 4.0/(1+arg*arg);
    }
    
    return NULL;
}


int main(){
    pthread_t hebras[n];
    double sum = 0;
    void * arg;
    unsigned long x;

    for(unsigned long i=0; i<n; ++i){
        arg = (void*) i;
        pthread_create (&(hebras[i]), NULL, f, arg);
    }

    for(unsigned int j=0; j<n; ++j){
	pthread_join(hebras[j], NULL);
    }

    for(unsigned int j=0; j<m; ++j){
	sum += sums[j];
    }
    
    cout << "Pi vale: " << sum/m << endl;
}
