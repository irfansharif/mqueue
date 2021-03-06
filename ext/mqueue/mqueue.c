#include "mq.h"

void
Init_mqueue() {
  VALUE mqueue = rb_define_class("MQueue", rb_cObject);
  rb_define_alloc_func(mqueue, alloc_mqueue);

  rb_define_method(mqueue, "initialize", mqueue_initialize, -1);
  rb_define_method(mqueue, "send", mqueue_send, 1);
  rb_define_method(mqueue, "receive", mqueue_receive, 0);
  rb_define_method(mqueue, "timedsend", mqueue_timedsend, -1);
  rb_define_method(mqueue, "timedreceive", mqueue_timedreceive, -1);
  rb_define_method(mqueue, "size", mqueue_size, 0);
  rb_define_method(mqueue, "capacity", mqueue_capacity, 0);
  rb_define_method(mqueue, "max_msgsize", mqueue_max_msgsize, 0);
  /* rb_define_method(mqueue, "on_notification", mqueue_attach_notification, 1); */
  /* rb_define_method(mqueue, "detach_notification", mqueue_detach_notification, 0); */
  rb_define_method(mqueue, "delete", mqueue_delete, 0);
}

static VALUE
alloc_mqueue(VALUE klass) {
  mqueue_t* queue_ptr;
  VALUE object = TypedData_Make_Struct(klass, mqueue_t, &mqueue_data_type, queue_ptr);

  (*queue_ptr).queue_descriptor = -1;
  (*queue_ptr).queue_name = NULL;
  (*queue_ptr).attributes.mq_curmsgs = 0;
  (*queue_ptr).attributes.mq_flags = 0;

  return object;
}

VALUE
mqueue_initialize(int argc, VALUE* argv, VALUE self) {
  VALUE queue_name, options;
  mqueue_t* queue_ptr;

  rb_scan_args(argc, argv, "1:", &queue_name, &options);
  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if ((*queue_ptr).queue_descriptor != -1)
    rb_raise(rb_eRuntimeError, "Illegal initialization");

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
        INT2NUM(4096))
      );
  (*queue_ptr).attributes.mq_flags = generate_flags(options);
  (*queue_ptr).queue_descriptor = mq_open((*queue_ptr).queue_name,
        (*queue_ptr).attributes.mq_flags,
        S_IRUSR | S_IWUSR,
        &(*queue_ptr).attributes
      );

  if ((*queue_ptr).queue_descriptor == (mqd_t)-1)
    rb_raise(rb_eRuntimeError, "Unable to initialize");

  return self;
}

VALUE
mqueue_delete(VALUE self) {
  mqueue_t* queue_ptr;
  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_unlink((*queue_ptr).queue_name) == -1)
    return Qfalse;

  return Qtrue;
}

VALUE
mqueue_send(VALUE self, VALUE message) {
  mqueue_t* queue_ptr;
  char* msg_ptr;
  size_t msg_len;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  msg_ptr = RSTRING_PTR(message);
  msg_len = RSTRING_LEN(message);

  if (mq_send((*queue_ptr).queue_descriptor, msg_ptr, msg_len, 0) == -1)
    return Qfalse;
  return Qtrue;
}

VALUE
mqueue_receive(VALUE self) {
  mqueue_t* queue_ptr;
  int len;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);
  char msg_buffer[(*queue_ptr).attributes.mq_msgsize];

  if ((len = mq_receive((*queue_ptr).queue_descriptor, msg_buffer, (*queue_ptr).attributes.mq_msgsize, 0)) == -1)
    return Qfalse;

  return rb_str_new(msg_buffer, len);
}

VALUE
mqueue_timedsend(int argc, VALUE* argv, VALUE self) {
  VALUE message, timeout;
  mqueue_t* queue_ptr;
  char* msg_ptr;
  size_t msg_len;
  struct timespec abs_timeout;

  rb_scan_args(argc, argv, "11", &message, &timeout);

  if (NIL_P(timeout))
    timeout = INT2NUM(10);

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  msg_ptr = RSTRING_PTR(message);
  msg_len = RSTRING_LEN(message);

  if (clock_gettime(CLOCK_REALTIME, &abs_timeout) == -1)
    return Qfalse;

  abs_timeout.tv_sec += NUM2INT(timeout);

  if (mq_timedsend((*queue_ptr).queue_descriptor, msg_ptr, msg_len, 0, &abs_timeout) == -1)
    return Qfalse;
  return Qtrue;
}

VALUE
mqueue_timedreceive(int argc, VALUE* argv, VALUE self) {
  VALUE timeout;
  mqueue_t* queue_ptr;
  struct timespec abs_timeout;
  int len;

  rb_scan_args(argc, argv, "01", &timeout);

  if (NIL_P(timeout))
    timeout = INT2NUM(10);

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);
  char msg_buffer[(*queue_ptr).attributes.mq_msgsize];

  if (clock_gettime(CLOCK_REALTIME, &abs_timeout) == -1)
    return Qfalse;

  abs_timeout.tv_sec += NUM2INT(timeout);
  if ((len = mq_timedreceive((*queue_ptr).queue_descriptor, msg_buffer, (*queue_ptr).attributes.mq_msgsize, 0, &abs_timeout)) == -1)
    return Qfalse;

  return rb_str_new(msg_buffer, len);
}

VALUE
mqueue_size(VALUE self) {
  mqueue_t* queue_ptr;
  struct mq_attr attributes;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_getattr((*queue_ptr).queue_descriptor, &attributes) == -1)
    return INT2NUM(-1);

  return INT2NUM(attributes.mq_curmsgs);
}

VALUE
mqueue_capacity(VALUE self) {
  mqueue_t* queue_ptr;
  struct mq_attr attributes;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_getattr((*queue_ptr).queue_descriptor, &attributes) == -1)
    return INT2NUM(-1);

  return INT2NUM(attributes.mq_maxmsg);
}

VALUE
mqueue_max_msgsize(VALUE self) {
  mqueue_t* queue_ptr;
  struct mq_attr attributes;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_getattr((*queue_ptr).queue_descriptor, &attributes) == -1)
    return INT2NUM(-1);

  return INT2NUM(attributes.mq_msgsize);
}

VALUE
mqueue_attach_notification(VALUE self, VALUE callback) {
  if(rb_class_of(callback) != rb_cProc)
    rb_raise(rb_eTypeError, "Expected Proc callback");

  mqueue_t* queue_ptr;
  struct sigevent sev;
  VALUE callback_args = rb_ary_new();

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);
  rb_ary_store(callback_args, 0, callback);

  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = notification_function;
  sev.sigev_notify_attributes = NULL;
  sev.sigev_value.sival_ptr = (void *)callback_args;

  if (mq_notify((*queue_ptr).queue_descriptor, &sev) == -1)
    return Qfalse;

  return Qtrue;
}

VALUE
mqueue_detach_notification(VALUE self) {
  mqueue_t* queue_ptr;

  TypedData_Get_Struct(self, mqueue_t, &mqueue_data_type, queue_ptr);

  if (mq_notify((*queue_ptr).queue_descriptor, NULL) == -1)
    return Qfalse;

  return Qtrue;
}

/******************************************************************************/

static void
notification_function(union sigval sv) {
  VALUE callback, callback_args;

  callback_args = (VALUE)sv.sival_ptr;
  callback = rb_ary_entry(callback_args, 0);

  rb_funcall(callback, rb_intern("call"), 0);
}

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

long
generate_flags(VALUE options) {
  VALUE flags = rb_hash_lookup(options, ID2SYM(rb_intern("flags"))), cur_flag;
  if (flags == Qnil || RARRAY_LEN(flags) == 0)
    return 0;
  int num_flags = RARRAY_LEN(flags), i;
  long mq_flags = 0;

  for(i = 0; i < num_flags; i++) {
    cur_flag = rb_ary_entry(flags, i);
    if (cur_flag == ID2SYM(rb_intern("creat")))
      mq_flags |= O_CREAT;
    else if (cur_flag ==  ID2SYM(rb_intern("excl")))
      mq_flags |= O_EXCL;
    else if (cur_flag == ID2SYM(rb_intern("rdonly")))
      mq_flags |= O_RDONLY;
    else if (cur_flag == ID2SYM(rb_intern("wronly")))
      mq_flags |= O_WRONLY;
    else if (cur_flag == ID2SYM(rb_intern("rdwr")))
      mq_flags |= O_RDWR;
    else if (cur_flag == ID2SYM(rb_intern("nonblock")))
      mq_flags |= O_NONBLOCK;
  }

  return mq_flags;
}
