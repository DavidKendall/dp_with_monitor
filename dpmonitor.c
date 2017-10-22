/* 
 * dpmonitor.c    
 *
 * Dining philosophers monitor
 */

#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dpmonitor.h"

/************ Type declarations **********************/

typedef enum {THINKING, HUNGRY, EATING} state_t;


/************ Local function prototypes **************/

static void eatIfOk(int i);
static int leftNghbr(int i);
static int rightNghbr(int i);

/*********** Monitor variables **********************/

static pthread_mutex_t dpMutex;

static pthread_cond_t okToEat[N_PHIL];

static state_t state[N_PHIL];


/*********** Monitor function definitions ***********/

void dpPickup(int i) {
  pthread_mutex_lock(&dpMutex);
  state[i] = HUNGRY;
  eatIfOk(i);
  while (state[i] != EATING) {
    pthread_cond_wait(&okToEat[i], &dpMutex);
  }
  pthread_mutex_unlock(&dpMutex);
}


void dpPutdown(int i) {
  pthread_mutex_lock(&dpMutex);
  state[i] = THINKING;
  eatIfOk(rightNghbr(i));
  eatIfOk(leftNghbr(i));
  pthread_mutex_unlock(&dpMutex);
}


/* Prints out state of philosophers as, say, TEHHE, meaning 
 * that philosopher 0 is THINKING, philosophers 1 and 4 are 
 * EATING, and philosophers 2 and 3 are HUNGRY.
 */
void dpPrintstate(int i){
  static char stat[] = "THE";

  pthread_mutex_lock(&dpMutex);
  printf("%d:", i);
  for (int i=0; i<N_PHIL; i++) {
    printf("%c", stat[(int)(state[i])]);
  }
  printf("\n");
  pthread_mutex_unlock(&dpMutex);
}


/*
 * Initialise the monitor mutex, state and
 * condition variables
 *
 * N.B. No mutex to lock and unlock in this function
 */
void dpInit(void) {
  int rc;

  // Initialise the monitor mutex  
  rc = pthread_mutex_init(&dpMutex, NULL);
  assert(rc == 0);

  // Initialise the state and the condition variables
  for (int i=0; i<N_PHIL; i+=1) {
    state[i] = THINKING;
    rc = pthread_cond_init(&okToEat[i], NULL);
    assert(rc == 0);
  }
}


/***************** Local function definitions *********************/

static void eatIfOk (int i) {
  if ((state[i] == HUNGRY) &&
      (state[rightNghbr(i)] != EATING) &&
      (state[leftNghbr(i)] != EATING)) {
    state[i] = EATING;
    pthread_cond_signal(&okToEat[i]);
  }
}


static int leftNghbr(int i) {
  return ((i+(N_PHIL-1)) % N_PHIL);
}


static int rightNghbr(int i) {
  return ((i+1) % N_PHIL);
}
