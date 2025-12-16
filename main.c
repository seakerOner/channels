//
// #include "mpsc.h"
// #include "spsc.h"
// #include <stdio.h>
//
// int main() {
//   ChannelSpsc *chan = channel_create_spsc(100, sizeof(long));
//   if (!chan) {
//     return 420;
//   }
//   ReceiverSpsc *receiver = spsc_get_receiver(chan);
//   SenderSpsc *sender = spsc_get_sender(chan);
//   printf("Hello, World \n");
//
//   long val = 10;
//   long val1 = 20;
//   long val2 = 230344398;
//
//   printf("sending : %lo \n", val);
//   printf("sending : %lo \n", val1);
//   printf("sending : %lo \n", val2);
//
//   int res = spsc_try_send(sender, &val);
//   int res1 = spsc_try_send(sender, &val1);
//   int res2 = spsc_try_send(sender, &val2);
//   if (res == -1 || res == -2) {
//     return res;
//   }
//   long value;
//   long value1;
//   long value2;
//
//   int v = spsc_recv(receiver, &value);
//   int v1 = spsc_recv(receiver, &value1);
//   int v2 = spsc_recv(receiver, &value2);
//
//   printf("Received: %lo \n", value);
//   printf("Received: %lo \n", value1);
//   printf("Received: %lo \n", value2);
//
//   spsc_destroy(chan);
//   return 0;
// }
#include "mpsc.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define CHANNEL_CAPACITY 100
#define NUM_PRODUCERS 4

ChannelMpsc *chan;

void *producer(void *arg) {
  int id = *(int *)arg;
  SenderMpsc *sender = mpsc_get_sender(chan);

  for (int i = 0; i < 10; i++) {
    int value = id * 100 + i;
    mpsc_send(sender, &value);
    printf("Producer %d sent %d\n", id, value);
  }

  mpsc_close_sender(sender);
  return NULL;
}

void *consumer(void *arg) {
  ReceiverMpsc *receiver = mpsc_get_receiver(chan);
  int value;
  int received_count = 0;

  while (1) {
    if (mpsc_recv(receiver, &value) == CHANNEL_OK) {
      printf("Consumer received %d\n", value);
      received_count++;
    } else if (mpsc_is_closed(chan) && received_count >= NUM_PRODUCERS * 10) {
      break;
    }
  }

  return NULL;
}

int main() {
  chan = channel_create_mpsc(CHANNEL_CAPACITY, sizeof(int));

  pthread_t producers[NUM_PRODUCERS];
  int ids[NUM_PRODUCERS];

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    ids[i] = i;
    pthread_create(&producers[i], NULL, producer, &ids[i]);
  }

  pthread_t cons;
  pthread_create(&cons, NULL, consumer, NULL);

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(producers[i], NULL);
  }
  mpsc_close(chan);
  pthread_join(cons, NULL);

  mpsc_destroy(chan);
  return 0;
}
