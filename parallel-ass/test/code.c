#include <stdio.h>
#include <pthread.h>

void* hello(void*tid){
	printf("Hello from thread %d \n",(int) tid);
}
int main(){
	pthread_t tid[10];
	int i;
	for (i = 0; i<10;i++){
		pthread_create(&tid[i],NULL,hello,(void*) i);
		sleep(1);
	}
	pthread_exit(NULL);
}
