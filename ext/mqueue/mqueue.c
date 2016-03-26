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
  rb_define_method(mqueue, "delete", mqueue_delete, -1);

}

static VALUE 
alloc_mqueue(VALUE klass) {
  mqueue_t* queue_ptr;
  VALUE object = TypedData_Make_Struct(klass, mqueue_t, &mqueue_data_type, queue_ptr);

  (*queue_ptr).queue_descriptor = -1;
  (*queue_ptr).queue_name = NULL;

  return object;
}


VALUE 
mqueue_initialize(int argc, VALUE* argv, VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_send(VALUE self, VALUE message) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_receive(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_timedsend(VALUE self, VALUE args) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_timedreceive(VALUE self, VALUE args) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_flush(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_size(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_capacity(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_attach_notification(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_detach_notification(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_delete(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}
