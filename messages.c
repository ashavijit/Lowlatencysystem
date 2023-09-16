#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_MESSAGES 1000000

int messageQueue[NUM_MESSAGES];
int front = 0;
int rear = -1;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void enqueue(int msg){
	pthread_mutex_lock(&mutex);
	if(count < NUM_MESSAGES){
		rear = (rear+1) % NUM_MESSAGES;
		messageQueue[rear] = msg;
		count++;
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
}

int dequeue(){
	pthread_mutex_lock(&mutex);
	while(count <= 0){
		pthread_cond_wait(&cond,&mutex);
	}
	int curr_msg = messageQueue[front];
	front = (front+1) % NUM_MESSAGES;
	count--;
	pthread_mutex_unlock(&mutex);
	return curr_msg;
}


void *msgProcessor(void *arg){
	int thread_id = *((int *)arg);
	for(int i=0;i<NUM_MESSAGES/NUM_THREADS;i++){
		int message = dequeue();
		printf("Thread %d processed message: %d\n", thread_id, message);
		usleep(10);
	}
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[NUM_MESSAGES];
	int thread_ids[NUM_THREADS];
	
	// how to create a worker thread ???? lets see 

	for(int i=0;i<NUM_THREADS;i++){
		thread_ids[i] = i;
		pthread_create(&threads[i],NULL,msgProcessor,&thread_ids[i]);
	}

	for(int i=0;i<= NUM_MESSAGES;i++){
		enqueue(i);
	}
	for(int i=0;i< NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
	}
	return 0;
}
/*NOTES



*/