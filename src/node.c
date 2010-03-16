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
VALUE IntruderNodeException = Qnil;
int node_count = 0;

/* internal methods */
static void declare_attr_accessors();
static void free_class_struct(void *class_struct);

/* implementation */
VALUE intruder_node_alloc(VALUE class){
  INTRUDER_NODE *class_struct = malloc(sizeof(INTRUDER_NODE));
  class_struct->cnode = malloc(sizeof(ei_cnode));
  class_struct->status = INTRUDER_DISCONNECTED;

  /* leak leak leak?? Check back with stkaes */
  VALUE obj = Data_Wrap_Struct(class, 0, free_class_struct, class_struct);
  return obj;
}

static void free_class_struct(void *class_struct)
{
  free(((INTRUDER_NODE *)class_struct)->cnode);
  free(class_struct);
}

VALUE intruder_node_init(VALUE self, VALUE sname, VALUE cookie){
  CLASS_STRUCT;
  rb_iv_set(self, "@sname", sname);
  rb_iv_set(self, "@cookie", cookie);

  /* initialize the node */
  if(ei_connect_init(class_struct->cnode, RSTRING_PTR(sname), RSTRING_PTR(cookie), node_count++) < 0){
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

VALUE intruder_node_connect(VALUE self, VALUE remote_node){
  INTRUDER_NODE *class_struct;
  Data_Get_Struct(self, INTRUDER_NODE, class_struct);
  int fd;

  if((class_struct->fd = ei_connect(class_struct->cnode, RSTRING_PTR(remote_node))) < 0)
    raise_rException_for_erl_errno();

  class_struct->status = INTRUDER_CONNECTED;
  return Qtrue;
}

void Init_intruder_node(){
  IntruderNode = rb_define_class_under(IntruderModule, "Node", rb_cObject);
  declare_attr_accessors();

  /* class methods */
  rb_define_alloc_func(IntruderNode, intruder_node_alloc);

  /* instance methods */
  rb_define_method(IntruderNode, "initialize", intruder_node_init, 2);
  rb_define_method(IntruderNode, "connect", intruder_node_connect, 1);
  rb_define_method(IntruderNode, "pid", intruder_node_pid, 0);
  rb_define_method(IntruderNode, "status", intruder_node_status, 0);

  /* exceptions */
  IntruderNodeException = rb_define_class_under(IntruderModule, "NodeError", IntruderException);
}

static void declare_attr_accessors(){
  ID attr_accessor = rb_intern("attr_reader");

  char  *i_vars[2] = {"cookie", "sname"};
  VALUE params[2];

  int i = 0;
  for(; i <= 1; i++){
    params[i] = STOSYM(i_vars[i]);
  }
  rb_funcall2(IntruderNode, attr_accessor, 2, params);
}
