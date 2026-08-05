#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


pthread_mutex_t dongle1;
pthread_mutex_t dongle2;

void *coderA (void *arg){
    pthread_mutex_lock(&dongle1);
    printf("Coder A took Dangle 1\n");
    
    sleep(1);
    
    printf("waiting for dongle 2...\n");
    pthread_mutex_lock(&dongle2);
    // printf("Coder A took Dangle 2\n");
    printf("Coder A Compile...\n");
    
    pthread_mutex_unlock(&dongle1);
    pthread_mutex_unlock(&dongle2);
}

void *coderB (void *arg){
    pthread_mutex_lock(&dongle2);
    printf("Coder B took Dangle 2\n");
    
    sleep(1);
    
    printf("waiting for dongle 1...\n");
    pthread_mutex_lock(&dongle1);
    // printf("Coder B took Dangle 1\n");
    printf("Coder B Compile...\n");
    
    pthread_mutex_unlock(&dongle2);
    pthread_mutex_unlock(&dongle1);
}

int main (){

    pthread_t t1, t2;

    pthread_mutex_init(&dongle1, NULL);
    pthread_mutex_init(&dongle2, NULL);

    pthread_create(&t1, NULL, &coderA, NULL);
    pthread_create(&t2, NULL, &coderB, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&dongle1);
    pthread_mutex_destroy(&dongle2);

    return 0;
}




























// #include <pthread.h>
// #include <stdio.h>
// #include <unistd.h>

// pthread_mutex_t dongle1;
// pthread_mutex_t dongle2;

// void *coderA(void *arg)
// {
//     pthread_mutex_lock(&dongle1);
//     printf("A took dongle 1\n");

//     sleep(1);

//     printf("A waiting for dongle 2...\n");
//     pthread_mutex_lock(&dongle2);

//     printf("A coding\n");

//     pthread_mutex_unlock(&dongle2);
//     pthread_mutex_unlock(&dongle1);

//     return NULL;
// }

// void *coderB(void *arg)
// {
//     pthread_mutex_lock(&dongle2);
//     printf("B took dongle 2\n");

//     sleep(1);

//     printf("B waiting for dongle 1...\n");
//     pthread_mutex_lock(&dongle1);

//     printf("B coding\n");

//     pthread_mutex_unlock(&dongle1);
//     pthread_mutex_unlock(&dongle2);

//     return NULL;
// }

// int main(void)
// {
//     pthread_t t1, t2;

//     int x = pthread_mutex_init(&dongle1, NULL);
//     printf("%d\n", x);

//     pthread_mutex_init(&dongle2, NULL);

//     pthread_create(&t1, NULL, coderA, NULL);
//     pthread_create(&t2, NULL, coderB, NULL);

//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);

//     pthread_mutex_destroy(&dongle1);
//     pthread_mutex_destroy(&dongle2);

//     return 0;
// }