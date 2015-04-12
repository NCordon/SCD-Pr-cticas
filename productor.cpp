#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

const int num_items=100;
const int tam_vec = 15;
int buffer[tam_vec];
int primera_libre;

sem_t escribe;
sem_t lee;

int producir_dato(){
    static int contador = 1 ;
    return contador ++ ;
}

void consumir_dato(int dato){
    cout << "dato recibido: " << dato << endl ;
}

void* productor(void *){
    for(unsigned i=0; i<num_items; i++){
        int dato = producir_dato();
        
        sem_wait(&escribe);
        buffer[primera_libre]=dato;
        primera_libre++;
        sem_post(&lee);      
        
    }
    return NULL;
}

void* consumidor(void *){
    for(unsigned i=0; i<num_items; i++){
        int dato;
        
        sem_wait(&lee);
        dato = buffer[primera_libre--];
        consumir_dato(dato);
        sem_post(&escribe);
    }
}

int main(){
    pthread_t hebra1, hebra2;
    sem_init(&escribe, 0, tam_vec);
    sem_init(&lee, 0, 0);
    primera_libre = 0;
    
    pthread_create(&hebra1, NULL, productor, NULL);
    pthread_create(&hebra2, NULL, consumidor, NULL);
    
    pthread_join(hebra1, NULL);
    pthread_join(hebra2, NULL);
    cout << endl << "fin" << endl;
}