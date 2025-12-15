# Channels

A collection of high-performance, low-level channels implemented in C for inter-thread communication. 
This project aims to provide lock-free, minimal-overhead communication primitives with predictable memory behavior.

## Available Channels

 - **Lock-free SPSC channel** for communication between a single producer and a single consumer thread


### SPSC Channel 

#### Features

- Lock-free SPSC channel for communication between a single producer and a single consumer thread.
- Dynamically allocated buffer with wrap-around (ring buffer) using modular arithmetic.
- try_send ensures safe sending without overwriting unread data.
- Supports arbitrary element types via element size (elem_size).
- Simple and minimal API with predictable memory behavior.

#### API

```c
typedef enum ChanState {
    OPEN = 0,
    CLOSED = 1
} ChanState;

typedef struct ChannelSpsc_t ChannelSpsc;
typedef struct SenderSpsc_t SenderSpsc;
typedef struct ReceiverSpsc_t ReceiverSpsc;

ChannelSpsc *channel_create_spsc(size_t capacity, size_t elem_size);
void spsc_close(ChannelSpsc *chan);
ChanState spsc_is_closed(ChannelSpsc *chan);
void spsc_destroy(ChannelSpsc *chan);

SenderSpsc *spsc_get_sender(ChannelSpsc *chan);
ReceiverSpsc *spsc_get_receiver(ChannelSpsc *chan);

int spsc_try_send(SenderSpsc *sender, const void *element);
int spsc_recv(ReceiverSpsc *receiver, void* out);
```

#### Usage Example

```c
#include "spsc.h"
#include <stdio.h>
#include <stdint.h>

#define CHANNEL_CAPACITY 100

int main() {
    // Create a channel for 100 long integers
    ChannelSpsc *chan = channel_create_spsc(CHANNEL_CAPACITY, sizeof(long));
    if (!chan) {
        fprintf(stderr, "Failed to create channel\n");
        return 1;
    }

    // Get sender and receiver handles
    SenderSpsc *sender = spsc_get_sender(chan);
    ReceiverSpsc *receiver = spsc_get_receiver(chan);

    printf("Channel example started\n");

    // Sample values to send
    long val0 = 10;
    long val1 = 20;
    long val2 = 230344398;

    printf("Sending: %ld, %ld, %ld\n", val0, val1, val2);

    // Send values
    if (spsc_try_send(sender, &val0) != SPSC_OK) {
        fprintf(stderr, "Failed to send val0\n");
    }
    if (spsc_try_send(sender, &val1) != SPSC_OK) {
        fprintf(stderr, "Failed to send val1\n");
    }
    if (spsc_try_send(sender, &val2) != SPSC_OK) {
        fprintf(stderr, "Failed to send val2\n");
    }

    // Receive values
    long recv0, recv1, recv2;

    if (spsc_recv(receiver, &recv0) == SPSC_OK) {
        printf("Received: %ld\n", recv0);
    }
    if (spsc_recv(receiver, &recv1) == SPSC_OK) {
        printf("Received: %ld\n", recv1);
    }
    if (spsc_recv(receiver, &recv2) == SPSC_OK) {
        printf("Received: %ld\n", recv2);
    }

    // Clean up
    spsc_destroy(chan);

    return 0;
}
```

#### Notes

- Thread-safety: Only safe for a single producer thread and single consumer thread.
- Wrap-around: Ring buffer behavior is handled with modulo arithmetic (index = head % capacity).
- Element size: The channel supports arbitrary data types using elem_size. Users must provide correct element size.
- Error handling:
    - `SPSC_ERR_FULL`: Send failed, buffer is full.
    - `SPSC_ERR_EMPTY`: Receive failed, buffer is empty.
    - `SPSC_ERR_NULL`: Null pointer provided.
    - Memory: `spsc_try_send` will never overwrite unread elements.
