/*
 * Helpfull stuff:
 * Erl_interface doku: http://erlang.mirror.su.se/doc/man/ei_connect.html
 * http://erlang.org/pipermail/erlang-questions/2003-August/009536.html
 * Erlix discussion: http://www.mentby.com/Group/ruby-talk/ruby-interface-of-erlang.html
 * Extending ruby (german): http://home.vrweb.de/~juergen.katins/ruby/buch/ext_ruby.html
 * http://www.eqqon.com/index.php/Ruby_C_Extension#debug
 */
#include "node.h"
#define STOSYM(string) ID2SYM(rb_intern(string))

VALUE IntruderNode = Qnil;
VALUE IntruderException = Qnil;
int node_count = 0;

/* internal methods */
static void declare_attr_accessors();

/* implementation */
VALUE intruder_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie){
  CLASS_STRUCT;
  rb_iv_set(self, "@host", host);
  rb_iv_set(self, "@sname", sname);
  rb_iv_set(self, "@cookie", cookie);

  /* initialize the node */
  if(ei_connect_init(class_struct->cnode, RSTRING(sname)->ptr, RSTRING(cookie)->ptr, node_count++) < 0){
    free(class_struct->cnode);
    rb_raise(rb_eRuntimeError, "Error initializing the node");
  }

  return self;
}

VALUE intruder_node_status(VALUE self){
  CLASS_STRUCT;
  if(class_struct->status == INTRUDER_CONNECTED)
    return STOSYM("connected");
  else
    return STOSYM("disconnected");
}

VALUE intruder_node_pid(VALUE self){
  CLASS_STRUCT;
  erlang_pid *pid = ei_self(class_struct->cnode);
  return INT2FIX(pid->num);
}

VALUE intruder_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie){
  VALUE argv[3];
  argv[0] = host;
  argv[1] = sname;
  argv[2] = cookie;

  struct intruder_node *class_struct = malloc(sizeof(struct intruder_node));
  class_struct->cnode = malloc(sizeof(ei_cnode));
  class_struct->status = INTRUDER_DISCONNECTED;

  /* leak leak leak?? */
  VALUE class_instance = Data_Wrap_Struct(class, 0, free, class_struct);
  rb_obj_call_init(class_instance, 3, argv);
  return class_instance;
}

VALUE intruder_node_connect(VALUE self, VALUE remote_node){
  struct intruder_node *class_struct;
  Data_Get_Struct(self, struct intruder_node, class_struct);

  if(ei_connect(class_struct->cnode, RSTRING(remote_node)->ptr) < 0){

    DEBUG("Result: %d\n", erl_errno);
    DEBUG("ERHOSTUNREACH: %d\n", EHOSTUNREACH);
    DEBUG("ENOMEM: %d\n", ENOMEM);
    DEBUG("EIO: %d\n", EIO);

    switch( erl_errno )
      {
      case EHOSTUNREACH :
        rb_raise(IntruderException, "Host unreachable");
        break;
      case ENOMEM :
        rb_raise(IntruderException, "Memory Error");
        break;
      case EIO :
        rb_raise(IntruderException, "IO error");
        break;
      }
  }
  class_struct->status = INTRUDER_CONNECTED;
  return Qtrue;
}

void Init_intruder_node(){
  IntruderNode = rb_define_class("IntruderNode", rb_cObject);
  declare_attr_accessors();

  /* class methods */
  rb_define_singleton_method(IntruderNode, "new", intruder_node_new, 3);

  /* instance methods */
  rb_define_method(IntruderNode, "initialize", intruder_node_init, 3);
  rb_define_method(IntruderNode, "connect", intruder_node_connect, 1);
  rb_define_method(IntruderNode, "pid", intruder_node_pid, 0);
  rb_define_method(IntruderNode, "status", intruder_node_status, 0);

  /* exceptions */
  IntruderException = rb_define_class("IntruderNodeException", rb_eRuntimeError);
}

static void declare_attr_accessors(){
  ID attr_accessor = rb_intern("attr_reader");

  char  *i_vars[3] = {"host", "cookie", "sname"};
  VALUE params[3];

  int i = 0;
  for(; i <= 2; i++){
    params[i] = STOSYM(i_vars[i]);
  }
  rb_funcall2(IntruderNode, attr_accessor, 3, params);
}
