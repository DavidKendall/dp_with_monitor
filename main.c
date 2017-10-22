/* 
 * main.c
 *
 * Dining philosophers
 *
 */    

#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "dpmonitor.h"


enum {N_ITERATIONS = 20};

static void *philosopher(void *arg); 
static void beginThreads(void);
static void endThreads(void);
static void randomDelay(void);

static pthread_t thread[N_PHIL];

int main(int argc, char *argv[]) {
  dpInit();
  beginThreads();
  endThreads();
  printf("\nFINISHED\n");
  return 0;
}

/*
 * The function implementing the philosopher behaviour.
 * The parameter 'arg' identifies the philosopher: 0, 1, 2, ... 
 */
static void *philosopher(void *arg) {

  long id = (long)(arg);

  for (int n = 0; n < N_ITERATIONS; n+=1) {
    randomDelay();
    dpPickup(id);
    dpPrintstate(id);
    randomDelay();
    dpPutdown(id);
  }
  pthread_exit(NULL);        
}


static void beginThreads(void) {
  int rc;

  for (long i=0; i<N_PHIL; i+=1) {
    rc = pthread_create(&thread[i], NULL,
			philosopher, (void *)i);
    assert(rc == 0);
  }
}


static void endThreads(void) {
  int rc;

  for (int i=0; i<N_PHIL; i+=1) {
    rc = pthread_join(thread[i], NULL);
    assert(rc == 0);
  }
}


static void randomDelay(void) {
  struct timespec delay = {0, (rand() % 49 + 1) * 20000000};  

  nanosleep(&delay, NULL);
}
