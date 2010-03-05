#include "ruby.h"
#include "ei_connect.h"
#include "ei.h"
#include <stdlib.h>
#include <string.h>

VALUE ErlNode = Qnil;
int node_count = 0;

/* prototypes */
void Init_erl_node();

/* ruby instance methods */
static VALUE erl_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie);

/* ruby class methods */
static VALUE erl_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie);

/* internal methods */
static void declare_attr_accessors();

/* implementation */
static VALUE erl_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie){
  rb_iv_set(self, "@host", host);
  rb_iv_set(self, "@sname", sname);
  rb_iv_set(self, "@cookie", cookie);
  return self;
}

static VALUE erl_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie){
  /* initialize the node */
  ei_cnode *node;
  const char *this_node_name = "ruby_node";

  if(ei_connect_init(node, this_node_name, RSTRING(cookie)->ptr, node_count++) < 0){
    rb_raise(rb_eRuntimeError, "Error initializing the node");
  }

  /* wrap the node and call initialize */
  VALUE argv[3];
  argv[0] = host;
  argv[1] = sname;
  argv[2] = cookie;

  VALUE ruby_node = Data_Make_Struct(class, ei_cnode, 0, free, node);
  rb_obj_call_init(ruby_node, 3, argv);
  return ruby_node;
}

void Init_erl_node(){
  ErlNode = rb_define_class("ErlNode", rb_cObject);
  declare_attr_accessors();

  /* class methods */
  rb_define_singleton_method(ErlNode, "new", erl_node_new, 3);

  /* instance methods */
  rb_define_method(ErlNode, "initialize", erl_node_init, 3);
}

static void declare_attr_accessors(){
  ID attr_accessor = rb_intern("attr_accessor");

  char  *i_vars[3] = {"host", "cookie", "sname"};
  VALUE params[3];

  int i = 0;
  for(; i <= 2; i++){
    params[i] = ID2SYM(rb_intern(i_vars[i]));
  }
  rb_funcall2(ErlNode, attr_accessor, 3, params);
}
