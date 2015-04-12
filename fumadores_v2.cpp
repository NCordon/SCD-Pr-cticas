#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
using namespace std;

const int NUM_FUMADORES = 3;
int ofrecido;
string ingredientes[NUM_FUMADORES]={"tabaco","papel","cerillas"};

sem_t sin_papel;
sem_t sin_cerillas;
sem_t sin_tabaco;
sem_t pon_ingrediente;
sem_t mutex;

void fumar(){
    const unsigned miliseg = 100U + (rand() % 1900U);
    usleep (1000U*miliseg);
}

void* fumador(void* x){
    unsigned long necesario = (unsigned long) x;
    
    while(true){
        switch(necesario){
            case 0:
                sem_wait(&sin_tabaco);
                break;
            case 1:
                sem_wait(&sin_papel);
                break;
            case 2:
                sem_wait(&sin_cerillas);
                break;
        }
        sem_wait(&mutex);
        cerr << "Fumador sin " << ingredientes[necesario] << " empieza a fumar";
        cerr << endl;
        sem_post(&mutex);
        fumar();
        sem_wait(&mutex);
        cerr << "Fumador sin " << ingredientes[necesario] << " acaba de fumar";
        cerr << endl;
        sem_post(&mutex);
        sem_post(&pon_ingrediente);
    }    
    return NULL;
}

void* estanquero(void *){
    while (true){
        ofrecido = rand()%3;
        
        switch(ofrecido){
            case 0:
                sem_post(&sin_tabaco);
                break;
            case 1:
                sem_post(&sin_papel);
                break;
            case 2:
                sem_post(&sin_cerillas);
                break;
        }
        sem_wait(&mutex);
        cerr << "Ingrediente ofrecido: " << ingredientes[ofrecido] << endl;
        sem_post(&mutex);
        sem_wait(&pon_ingrediente);
    }
}

int main(){
    srand(time(NULL));
    pthread_t fumadores_t[NUM_FUMADORES];
    pthread_t estanquero_t;
    
    sem_init(&pon_ingrediente, 0, 0);
    sem_init(&sin_tabaco, 0, 0);
    sem_init(&sin_papel, 0, 0);
    sem_init(&sin_cerillas, 0, 0);
    sem_init(&mutex,0,1);
    
    for (unsigned long i=0; i<NUM_FUMADORES; ++i){
        pthread_create(&(fumadores_t[i]), NULL, fumador, (void *) i);
    }
    
    pthread_create(&estanquero_t, NULL, estanquero, NULL);
    
    for (int i=0; i<NUM_FUMADORES; ++i){
        pthread_join(fumadores_t[i], NULL);
    }
    
    pthread_join(estanquero_t, NULL);
}