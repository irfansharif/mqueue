#include <mqueue.h>

VALUE mqueue;

void
Init_mqueue() {
  mqueue = rb_define_class("MQueue", rb_cObject);
  rb_define_alloc_func(mqueue, alloc_mqueue);
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
initialize_mqueue(int argc, VALUE* argv, VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_send(VALUE self, value message) {
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
mqueue_attach_notification_hook(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_detach_notification_hook(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}

VALUE 
mqueue_delete(VALUE self) {
  rb_raise(rb_eNotImpError, "Method not implemented");
}
