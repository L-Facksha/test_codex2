#include <pthread.h>
#include <stdio.h>

int counter = 0;

struct Person
{
    char name[20];
    int age;
};

pthread_mutex_t mutex;

void *increment(void *arg){
    for (int i = 0; i < 100000; i++){
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    // struct Person *persone = arg;
    // int \*num = arg;
    // printf("%s %d\n", persone->name, persone->age);
    // printf("%d\n", counter);
    // printf("Hello from worcker!\n");
    return NULL;
}

int main(){
    pthread_t t1, t2, t3, t4;
    pthread_attr_t attr;
    
    // pthread_attr_init(&attr);
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    // int x = 6;
    struct Person p = {"anass", 14};
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, increment, &p);
    pthread_create(&t2, NULL, increment, &p);
    pthread_create(&t3, NULL, increment, &p);
    // pthread_create(&t4, NULL, increment, &p);
    
    // pthread_attr_destroy(&attr);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_mutex_destroy(&mutex);
    printf("%d\n", counter);
    // pthread_join(t4, NULL);

    // printf("counter = %d\n", counter);
    printf("Main finished.\n");
    // printf("%d", (char)'A');
    // printf("\n");
    // printf("%c", (char)65);

    return 0;


}