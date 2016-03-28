#ifndef MQUEUE
#define MQUEUE

#include <ruby.h>
#include <fcntl.h>      /* Defines O_* constants */
#include <sys/stat.h>   /* Defines mode constants */
#include <mqueue.h>
#include <unistd.h>

extern VALUE mqueue;
typedef struct {
  mqd_t queue_descriptor;
  struct mq_attr attributes;
  char* queue_name;
  size_t queue_name_len;
} mqueue_t;

static void
free_mqueue (void* ptr) {
  mqueue_t* queue_ptr = ptr;
  if (mq_close((*queue_ptr).queue_descriptor) == -1)
    rb_sys_fail("mq_close failed");

  free((*queue_ptr).queue_name);
  free(ptr);
}

size_t
size_mqueue (const void* ptr) {
  mqueue_t* queue_ptr = queue_ptr;
  return sizeof(mqueue_t) + sizeof(char) * (*queue_ptr).queue_name_len;
}

static const rb_data_type_t
mqueue_data_type = {
  "mqueue_data_type",
  {
    0,
    free_mqueue,
    size_mqueue,
    0
  },
  0, 0, 0
};

void Init_mqueue();
static VALUE alloc_mqueue(VALUE);
VALUE  mqueue_initialize(int argc, VALUE* argv, VALUE self);

VALUE mqueue_send(VALUE self, VALUE message);
VALUE mqueue_receive(VALUE self);
VALUE mqueue_timedsend(VALUE self, VALUE args);
VALUE mqueue_timedreceive(VALUE self, VALUE args);
VALUE mqueue_flush(VALUE self);

VALUE mqueue_size(VALUE self);
VALUE mqueue_capacity(VALUE self);

VALUE mqueue_attach_notification(VALUE self);
VALUE mqueue_detach_notification(VALUE self);

VALUE mqueue_delete(VALUE self);
#endif
