#include "mod.h"

VALUE IntruderMod = Qnil;
extern VALUE IntruderModule;

VALUE intruder_mod_init(VALUE self, VALUE modname, VALUE node){
  rb_iv_set(self, "@node", node);
  rb_iv_set(self, "@modname", modname);
  return self;
}

VALUE intruder_mod_alloc(VALUE class){
  struct intruder_mod *im = (struct intruder_mod*)malloc(sizeof(struct intruder_mod));
  VALUE obj;
  obj = Data_Wrap_Struct(class, 0, free, im);
  return obj;
}

VALUE private_intruder_mod_rpc(VALUE self, VALUE args){
  VALUE fun = rb_ary_shift(args);
  VALUE args_str = rb_funcall(args, rb_intern("inspect"), 0);
  DEBUG("%s\n", RSTRING(args_str)->ptr);
  DEBUG("%s\n", RSTRING(fun)->ptr);

  /* name of the module to call is set in a ruby ivar */
  char *mod = RSTRING(rb_iv_get(self, "@modname"))->ptr;
  int ret;
  /* get the data struct of the Intruder::Node in the Intruder::Mod */
  struct intruder_node *in_s;
  Data_Get_Struct(rb_iv_get(self, "@node"), struct intruder_node, in_s);

  /* prepare the erlang stuff (wtf o_O) */
  int index = 0, is_alive;
  ei_x_buff rpcargs, result;
  ei_x_new(&result);
  ei_x_new(&rpcargs);
  ei_x_encode_list_header(&rpcargs, 0);
  ei_x_encode_empty_list(&rpcargs);

  DEBUG("rpc call to %s:%s\n", mod, RSTRING(fun)->ptr);
  ret = ei_rpc(in_s->cnode, in_s->fd, mod, RSTRING(fun)->ptr, rpcargs.buff, rpcargs.index, &result);
  if(ret < 0)
    raise_rException_for_erl_errno();

  return Qnil;
}

void Init_intruder_mod(){
  IntruderMod = rb_define_class_under(IntruderModule, "Mod", rb_cObject);

  /* class methods */
  rb_define_alloc_func(IntruderMod, intruder_mod_alloc);

  /* instance methods */
  rb_define_method(IntruderMod, "initialize", intruder_mod_init, 2);
  rb_define_private_method(IntruderMod, "rpc", private_intruder_mod_rpc, -2);
}
