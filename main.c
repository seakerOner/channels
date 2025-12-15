#include "spsc.h"
#include <stdio.h>

int main() {
  ChannelSpsc *chan = channel_create_spsc(100, sizeof(long));
  if (!chan) {
    return 420;
  }
  ReceiverSpsc *receiver = spsc_get_receiver(chan);
  SenderSpsc *sender = spsc_get_sender(chan);
  printf("Hello, World \n");

  long val = 10;
  long val1 = 20;
  long val2 = 230344398;

  printf("sending : %lo \n", val);
  printf("sending : %lo \n", val1);
  printf("sending : %lo \n", val2);

  int res = spsc_try_send(sender, &val);
  int res1 = spsc_try_send(sender, &val1);
  int res2 = spsc_try_send(sender, &val2);
  if (res == -1 || res == -2) {
    return res;
  }
  long value;
  long value1;
  long value2;

  int v = spsc_recv(receiver, &value);
  int v1 = spsc_recv(receiver, &value1);
  int v2 = spsc_recv(receiver, &value2);


  printf("Received: %lo \n", value);
  printf("Received: %lo \n", value1);
  printf("Received: %lo \n", value2);

  spsc_destroy(chan);
  return 0;
}
