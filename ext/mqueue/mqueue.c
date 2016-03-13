#include <ruby.h>

void
Init_mqueue() {
  VALUE mqueue;

  mqueue = rb_define_class("MQueue", rb_cObject);
}
