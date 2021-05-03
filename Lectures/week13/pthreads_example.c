#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_SLOTS 10

static int start, end, num_used;
static int data[NUM_SLOTS];

pthread_mutex_t m;

void *reader(void *arg)
{
  int i;
  int is_empty;

  for (i = 0; i < 1000; i++) {
    do {
      pthread_mutex_lock(&m);
      if (num_used == 0) {
        is_empty = 1;
      } else {
        printf("Read %d from %d\n", data[end % NUM_SLOTS], end % NUM_SLOTS);
        num_used--;
        end++;
        is_empty = 0;
      }
      pthread_mutex_unlock(&m);
    } while (is_empty);
    usleep(rand() % 100000);
  }

  return NULL;
}

void *writer(void *arg)
{
  int i;
  int is_full;

  for (i = 0; i < 1000; i++) {
    do {
      pthread_mutex_lock(&m);
      if (num_used == NUM_SLOTS) {
        is_full = 1;
      } else {
        data[start % NUM_SLOTS] = i;
        printf("Wrote %d to %d\n", data[start % NUM_SLOTS], start % NUM_SLOTS);
        start++;
        is_full = 0;
        num_used++;
      }
      pthread_mutex_unlock(&m);
    } while (is_full);
    usleep(rand() % 100000);
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t reader_id, writer_id;

  srand(time(NULL));

  pthread_mutex_init(&m, NULL);

  pthread_create(&reader_id, NULL, reader, NULL);
  pthread_create(&writer_id, NULL, writer, NULL);

  pthread_join(reader_id, NULL);
  pthread_join(writer_id, NULL);

  pthread_mutex_destroy(&m);

  return 0;
}
