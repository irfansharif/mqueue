#include "mq.h"

VALUE mqueue;

void
Init_mqueue() {
  mqueue = rb_define_class("MQueue", rb_cObject);
  rb_define_alloc_func(mqueue, alloc_mqueue);

  rb_define_method(mqueue, "initialize", mqueue_initialize, -1);
  rb_define_method(mqueue, "send", mqueue_send, -1);
  rb_define_method(mqueue, "receive", mqueue_receive, -1);
  rb_define_method(mqueue, "timedsend", mqueue_timedsend, -1);
  rb_define_method(mqueue, "timedreceive", mqueue_timedreceive, -1);
  rb_define_method(mqueue, "flush", mqueue_flush, -1);
  rb_define_method(mqueue, "size", mqueue_size, -1);
  rb_define_method(mqueue, "capacity", mqueue_capacity, -1);
  rb_define_method(mqueue, "on_notification", mqueue_attach_notification, -1);
  rb_define_method(mqueue, "detach_notification", mqueue_detach_notification, -1);
  rb_define_method(mqueue, "delete", mqueue_delete, 0);
}

static VALUE
alloc_mqueue(VALUE klass) {
  mqueue_t* queue_ptr;
  VALUE object = TypedData_Make_Struct(klass, mqueue_t, &mqueue_data_type, queue_ptr);

  (*queue_ptr).queue_descriptor = -1;
  (*queue_ptr).queue_name = NULL;
  (*queue_ptr).attributes.mq_curmsgs = 0;

  return object;
}


VALUE
mqueue_initialize(int argc, VALUE* argv, VALUE self) {
  VALUE queue_name, options;
  mqueue_t* queue_ptr;

  rb_scan_args(argc, argv, "1:", &queue_name, &options);
  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (TYPE(queue_name) != T_STRING)
    rb_raise(rb_eTypeError, "Invalid queue name, must be a string");
  if (TYPE(options) != T_HASH)
    options = rb_hash_new();

  (*queue_ptr).queue_name = ruby_strdup(StringValueCStr(queue_name));
  (*queue_ptr).attributes.mq_maxmsg = FIX2INT(rb_hash_lookup2(options,
        ID2SYM(rb_intern("capacity")),
        INT2NUM(10))
      );
  (*queue_ptr).attributes.mq_msgsize = FIX2INT(rb_hash_lookup2(options,
        ID2SYM(rb_intern("max_msgsize")),
        INT2NUM(8192))
      );
  (*queue_ptr).queue_descriptor = mq_open((*queue_ptr).queue_name,
        O_CREAT | O_RDWR,
        S_IRUSR | S_IWUSR,
        &(*queue_ptr).attributes
      );

  if ((*queue_ptr).queue_descriptor == (mqd_t) -1)
    rb_sys_fail("Failed to create queue");

  return self;
}

VALUE
mqueue_delete(VALUE self) {
  mqueue_t* queue_ptr;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_unlink((*queue_ptr).queue_name) == -1)
    rb_sys_fail("Failed to unlink message queue");

  return Qtrue;
}

VALUE
mqueue_send(VALUE self, VALUE message) {
  rb_raise(rb_eNotImpError, "send method not implemented");
}

VALUE
mqueue_receive(VALUE self) {
  rb_raise(rb_eNotImpError, "receive method not implemented");
}

VALUE
mqueue_timedsend(VALUE self, VALUE args) {
  rb_raise(rb_eNotImpError, "timedsend method not implemented");
}

VALUE
mqueue_timedreceive(VALUE self, VALUE args) {
  rb_raise(rb_eNotImpError, "timedreceive method not implemented");
}

VALUE
mqueue_flush(VALUE self) {
  rb_raise(rb_eNotImpError, "flush method not implemented");
}

VALUE
mqueue_size(VALUE self) {
  rb_raise(rb_eNotImpError, "size method not implemented");
}

VALUE
mqueue_capacity(VALUE self) {
  rb_raise(rb_eNotImpError, "capacity method not implemented");
}

VALUE
mqueue_attach_notification(VALUE self) {
  rb_raise(rb_eNotImpError, "attach_notification method not implemented");
}

VALUE
mqueue_detach_notification(VALUE self) {
  rb_raise(rb_eNotImpError, "detach_notification method not implemented");
}

/******************************************************************************/

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
  return sizeof(mqueue_t) + sizeof(char) * strlen((*queue_ptr).queue_name);
}
