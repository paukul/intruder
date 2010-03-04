#include "ruby.h"
#include "erl_interface.h"

VALUE ErlNode = Qnil;
static int node_count = 0;

/* prototypes */
void Init_erl_node();

/* ruby methods */
static VALUE erl_node_init(VALUE self, VALUE url, VALUE cookie);
static VALUE method_test(VALUE self);

/* internal methods */
static void declare_attr_accessors();

/* implementation */
static VALUE erl_node_init(VALUE self, VALUE url, VALUE cookie){
  node_count++;
  rb_iv_set(self, "@url", url);
  rb_iv_set(self, "@cookie", cookie);
  rb_iv_set(self, "@nodename", rb_str_new2("c" + node_count));
}

static VALUE method_test(VALUE self){
  return rb_str_new2("foobar");
}

void Init_erl_node(){
  ErlNode = rb_define_class("ErlNode", rb_cObject);
  declare_attr_accessors();

  /* instance methods */
  rb_define_method(ErlNode, "test", method_test, 0);
  rb_define_method(ErlNode, "initialize", erl_node_init, 2);
}

static void declare_attr_accessors(){
  ID attr_accessor = rb_intern("attr_accessor");

  char  *i_vars[3] = {"url", "cookie", "nodename"};
  VALUE params[3];

  int i = 0;
  for(; i <= 2; i++){
    params[i] = ID2SYM(rb_intern(i_vars[i]));
  }
  rb_funcall2(ErlNode, attr_accessor, 3, params);
}
