#ifndef MQUEUE
#define MQUEUE

#include <ruby.h>
#include <fcntl.h>      /* Defines O_* constants */
#include <sys/stat.h>   /* Defines mode constants */
#include <mqueue.h>
#include <unistd.h>
#include <time.h>

extern VALUE mqueue;
typedef struct {
  mqd_t queue_descriptor;
  struct mq_attr attributes;
  char* queue_name;
} mqueue_t;

static void free_mqueue(void*);
size_t size_mqueue(const void*);

static const rb_data_type_t
mqueue_data_type = {
  "mqueue_data_type",
  {
    0,
    free_mqueue,
    size_mqueue
  }
};

void Init_mqueue();
static VALUE alloc_mqueue(VALUE);
VALUE  mqueue_initialize(int, VALUE*, VALUE);

VALUE mqueue_send(VALUE, VALUE);
VALUE mqueue_receive(VALUE);
VALUE mqueue_timedsend(int, VALUE*, VALUE);
VALUE mqueue_timedreceive(int, VALUE*, VALUE);
VALUE mqueue_size(VALUE);
VALUE mqueue_capacity(VALUE);
VALUE mqueue_attach_notification(VALUE);
VALUE mqueue_detach_notification(VALUE);
VALUE mqueue_delete(VALUE);

#endif
