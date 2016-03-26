#ifndef MQUEUE
#define MQUEUE

#include <ruby.h>
#include <fcntl.h>      /* Defines O_* constants */
#include <sys/stat.h>   /* Defines mode constants */
#include <mqueue.h>

// message queue descriptor type

// Creates a new message queue or opens existing one
/* mq_open(const char *name, int oflag, ..., mode_t mode, struct mq_attr *attr); */

/*
 * O_CREAT - Create queue if doesn't already exist
 * O_EXCL  - With O_CREAT, create queue exclusively
 * O_RDONLY - Read only
 * O_WRONLY - Write only
 * O_RDWR - Reading/Writing
 * O_NONBLOCK - Non-blocking mode, fails all blocking commands
 */

// Returns 0 on success, or -1 on error, notification registration removed
/* mq_close(mqd_t queue_descriptor); */

// Destroy queue
/* mq_unlink(const char *name) */

/* struct mq_attr { */
/*   long mq_flags; */
/*   long mq_maxmsg; */
/*   long mq_msgsize; */
/*   long mq_curmsgs; */
/* } */

extern VALUE mqueue;
typedef struct {
  mqd_t queue_descriptor;
  struct mq_attr attributes;
  char* queue_name;
} mqueue_t;

static void
free_mqueue (void* ptr) {

}

size_t
size_mqueue (const void* ptr) {

}

static const rb_data_type_t
mqueue_data_type = {
  "mqueue_data_type",
  {
    0,
    free_mqueue,
    size_mqueue
  },
  0,
  0,
  RUBY_TYPED_FREE_IMMEDIATELY
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
