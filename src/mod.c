#include "mod.h"

VALUE IntruderMod = Qnil;
extern VALUE IntruderModule;
extern VALUE IntruderTerm;

VALUE intruder_mod_init(VALUE self, VALUE modname, VALUE node){
  rb_iv_set(self, "@node", node);
  rb_iv_set(self, "@modname", modname);
  return self;
}

VALUE intruder_mod_alloc(VALUE class){
  INTRUDER_MOD *im = (INTRUDER_MOD*)malloc(sizeof(INTRUDER_MOD));
  VALUE obj;
  obj = Data_Wrap_Struct(class, 0, free, im);
  return obj;
}

VALUE private_intruder_mod_rpc(VALUE self, VALUE args){
  VALUE fun = rb_ary_shift(args);
  VALUE args_str = rb_funcall(args, rb_intern("inspect"), 0);
  VALUE ruby_object;

  /* name of the module to call is set in a ruby ivar */
  char *mod = RSTRING_PTR(rb_iv_get(self, "@modname"));
  int ret;
  /* get the data struct of the Intruder::Node in the Intruder::Mod */
  INTRUDER_NODE *in_s;
  Data_Get_Struct(rb_iv_get(self, "@node"), INTRUDER_NODE, in_s);

  /* prepare the erlang stuff */
  int index = 0;
  ei_x_buff rpcargs, result;
  ei_x_new(&result);
  ei_x_new(&rpcargs);

  /* encoding the params */
  ei_x_format_wo_ver(&rpcargs, "[]");
  printf("sending params: ");
  ei_print_term(stdout, rpcargs.buff, &index);
  fflush(stdout);

  /* RPC call */
  DEBUG("\nrpc call to %s:%s\n", mod, RSTRING_PTR(fun));
  ret = ei_rpc(in_s->cnode, in_s->fd, mod, RSTRING_PTR(fun), rpcargs.buff, rpcargs.index, &result);
  if(ret < 0)
    raise_rException_for_erl_errno();

  ETERM *tuplep;
  tuplep = erl_decode(result.buff);

  ruby_object = rb_value_from_eterm(tuplep);

  /* free up memory */
  ei_x_free(&rpcargs);
  ei_x_free(&result);
  return ruby_object;
}

void Init_intruder_mod(){
  IntruderMod = rb_define_class_under(IntruderModule, "Mod", rb_cObject);

  /* class methods */
  rb_define_alloc_func(IntruderMod, intruder_mod_alloc);

  /* instance methods */
  rb_define_method(IntruderMod, "initialize", intruder_mod_init, 2);
  rb_define_private_method(IntruderMod, "rpc", private_intruder_mod_rpc, -2);
}
