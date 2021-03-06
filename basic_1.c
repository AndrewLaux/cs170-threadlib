#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
struct timeval tv;

#define wait(i) ({tv.tv_sec = i;tv.tv_usec=500000;select(0,NULL,NULL,NULL,&tv);})

#define enjoy_party wait(1)
#define cleanup_party wait(2)


unsigned int thread_1_done = 0;
pthread_t thread_1;

void * bbq_party(void *args) {
	printf("Friend %u came to the party!\n",pthread_self());
	thread_1_done++;
}

int main() {
	
	printf("Inviting friends to the party!\n");

	pthread_create(&thread_1, NULL, bbq_party, NULL);

	while(thread_1_done == 0) {
		enjoy_party;
	}

	printf("Friend %u left the party...\n", thread_1);
	cleanup_party;
	printf("Now we're all alone... :(\n");

	return 0;
}
