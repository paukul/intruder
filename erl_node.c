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
 * Extending ruby (german): http://home.vrweb.de/~juergen.katins/ruby/buch/ext_ruby.html
 */
VALUE ErlNode = Qnil;
VALUE ErlException = Qnil;
int node_count = 0;

struct ruby_node
{
  ei_cnode *cnode;
  int fd;         /* file descriptor for the communication with the epmd */
};

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

  struct ruby_node *class_struct;
  Data_Get_Struct(self, struct ruby_node, class_struct);

  /* initialize the node */
  if(ei_connect_init(class_struct->cnode, RSTRING(sname)->ptr, RSTRING(cookie)->ptr, node_count++) < 0){
    free(class_struct->cnode);
    rb_raise(rb_eRuntimeError, "Error initializing the node");
  }

  return self;
}

static VALUE erl_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie){
  VALUE argv[3];
  argv[0] = host;
  argv[1] = sname;
  argv[2] = cookie;

  struct ruby_node *class_struct = malloc(sizeof(struct ruby_node));
  class_struct->cnode = malloc(sizeof(ei_cnode));

  /* leak leak leak?? */
  VALUE class_instance = Data_Wrap_Struct(class, 0, free, class_struct);
  rb_obj_call_init(class_instance, 3, argv);
  return class_instance;
}

static VALUE erl_connect(VALUE self, VALUE remote_node){
  struct ruby_node *class_struct;
  Data_Get_Struct(self, struct ruby_node, class_struct);

  if(ei_connect(class_struct->cnode, RSTRING(remote_node)->ptr) < 0){

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

  return Qtrue;
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
