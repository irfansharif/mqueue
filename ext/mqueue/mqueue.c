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
//  (*queue_ptr).attributes = NULL;

  return object;
}


VALUE
mqueue_initialize(int argc, VALUE* argv, VALUE self) {
  VALUE queue_name, queue_capacity, queue_max_msgsize, queue_flag;
  mqueue_t* queue_ptr;

  rb_scan_args(argc, argv, "13", &queue_name, &queue_capacity, &queue_max_msgsize, &queue_flag);
  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (TYPE(queue_name) != T_STRING)
    rb_raise(rb_eTypeError, "Invalid queue name, must be a string");
  else {
    (*queue_ptr).queue_name = ruby_strdup(StringValueCStr(queue_name));
    (*queue_ptr).queue_name_len = RSTRING_LEN(queue_name);

    if (FIXNUM_P(queue_capacity))
      (*queue_ptr).attributes.mq_maxmsg = FIX2INT(queue_capacity);
    if (FIXNUM_P(queue_max_msgsize))
      (*queue_ptr).attributes.mq_msgsize = FIX2INT(queue_max_msgsize);

    (*queue_ptr).queue_descriptor = mq_open((*queue_ptr).queue_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if ((*queue_ptr).queue_descriptor == (mqd_t) -1)
       rb_sys_fail("mq_open failed");
  }
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
