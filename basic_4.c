#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/select.h>
struct timeval tv1,tv2;

#define wait(i,j) ({if(j == 0) {tv1.tv_sec = i;tv1.tv_usec=500000;select(0,NULL,NULL,NULL,&tv1);} else {tv2.tv_sec = i;tv2.tv_usec=500000;select(0,NULL,NULL,NULL,&tv2);}})

#define enjoy_party wait(1,0)
#define cleanup_party wait(2,0)
#define pass_time wait(1,1)

unsigned int thread_1_done = 0;
unsigned int make_mess = 0;
unsigned int is_welcome = 1;
pthread_t thread_1;

void * bbq_party(void *args) {
	printf("Friend %u came to the party!\n",pthread_self());
	while(is_welcome > 0) {
		make_mess++;
		pass_time;
	}
	thread_1_done++;
}

int main() {
	
	printf("Inviting friends to the party!\n");

	pthread_create(&thread_1, NULL, bbq_party, NULL);

	while(thread_1_done == 0) {
		if(is_welcome > 0 && make_mess >= 2) {
			printf("%u made a mess! Not a good friend.\n",thread_1);
			is_welcome = 0;
		} else {
			enjoy_party;
		}
	}

	printf("Not-Friend %u finally left the party.\n", thread_1);
	cleanup_party;
	printf("I should find better friends...\n");

	return 0;
}