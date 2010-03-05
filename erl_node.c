#include "ruby.h"
#include "ei_connect.h"
#include "ei.h"

/* common imports */
#include <stdlib.h>
#include <string.h>

#define ERL_NODE_DEBUG

#ifdef ERL_NODE_DEBUG
#define DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif

/*
 * Helpfull stuff:
 * Erl_interface doku: http://erlang.mirror.su.se/doc/man/ei_connect.html
 * http://erlang.org/pipermail/erlang-questions/2003-August/009536.html
 * Erlix discussion: http://www.mentby.com/Group/ruby-talk/ruby-interface-of-erlang.html
 */
VALUE ErlNode = Qnil;
VALUE ErlException = Qnil;
int node_count = 0;

/* prototypes */
void Init_erl_node();

/* ruby instance methods */
static VALUE erl_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie);
static VALUE erl_connect(VALUE self, VALUE remote_node);

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
  ei_cnode *node = malloc(sizeof(ei_cnode));

  if(ei_connect_init(node, "ruby_node", RSTRING(cookie)->ptr, node_count++) < 0){
    free(node);
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

static VALUE erl_connect(VALUE self, VALUE remote_node){
  ei_cnode *node;
  Data_Get_Struct(self, ei_cnode, node);

  if(ei_connect(node, RSTRING(remote_node)->ptr) < 0){
    DEBUG("Result: %d\n", erl_errno);
    DEBUG("ERHOSTUNREACH: %d\n", EHOSTUNREACH);
    DEBUG("ENOMEM: %d\n", ENOMEM);
    DEBUG("EIO: %d\n", EIO);
    switch( erl_errno )
      {
      case EHOSTUNREACH :
        rb_raise(ErlException, "Host unreachable");
        break;
      case ENOMEM :
        rb_raise(ErlException, "Memory Error");
        break;
      case EIO :
        rb_raise(ErlException, "IO error");
        break;
      }
  }

  return Qnil;
}

void Init_erl_node(){
  ErlNode = rb_define_class("ErlNode", rb_cObject);
  declare_attr_accessors();

  /* class methods */
  rb_define_singleton_method(ErlNode, "new", erl_node_new, 3);

  /* instance methods */
  rb_define_method(ErlNode, "initialize", erl_node_init, 3);
  rb_define_method(ErlNode, "connect", erl_connect, 1);

  /* exceptions */
  ErlException = rb_define_class("ErlNodeException", rb_eRuntimeError);
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
