#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
pthread_barrier_t barrier; // optional: to hopefully make deadlock more consistent

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];
//pthread_mutex_t clear_lock;
void clear(){
	for (int i = 0; i<2; i++){
		 printf("\033[A\033[K");
	}

}

int randomval(int lower, int upper){
	long randomnum= random();
	int ranval = (randomnum% (upper-lower+1)) + lower;
	return ranval;

}

void *eat(void *arg) {
    int n = (int) (long)arg;

    // optional: sync up threads to make deadlock hopefully happen more consistently
    pthread_barrier_wait(&barrier);

    // take two chopsticks
    pthread_mutex_lock(&chopstick[n]);
    printf("Philosopher %d got chopstick %d\n", n, n);

	//https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_lock.html for trylock and ebusy
	int while_count = 0;
    while (pthread_mutex_trylock(&chopstick[(n+1)%5]) == EBUSY){

	   	printf("Philosopher %d set down chopstick %d\n", n, n);
		pthread_mutex_unlock(&chopstick[n]);
		usleep(randomval(50*(1<<while_count), 100*(1<<while_count)));	
    	pthread_mutex_lock(&chopstick[n]);
    	printf("Philosopher %d got chopstick %d\n", n, n);

		while_count++;
	};
    printf("Philosopher %d got chopstick %d\n", n, (n+1)%5);
    
    printf ("Philosopher %d is eating\n",n);
    sleep(1);
    
    // set them back down
    printf("Philosopher %d set down chopstick %d\n", n, (n+1)%5);
    pthread_mutex_unlock(&chopstick[(n+1)%5]);
    printf("Philosopher %d set down chopstick %d\n", n, n);
    pthread_mutex_unlock(&chopstick[n]);
    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);
//	pthread_mutex_init(&clear_lock, NULL);
    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}
