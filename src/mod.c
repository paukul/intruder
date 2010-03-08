#include "mod.h"

VALUE IntruderMod = Qnil;
extern VALUE IntruderModule;

static VALUE rb_value_from_eterm(ETERM *eterm);
static VALUE rb_value_from_list(ETERM *list, int is_member);
static VALUE rb_value_from_tuple(ETERM *tuple);
static VALUE rb_value_from_atom(ETERM *atom);

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

  /* name of the module to call is set in a ruby ivar */
  char *mod = RSTRING(rb_iv_get(self, "@modname"))->ptr;
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
  DEBUG("\nrpc call to %s:%s\n", mod, RSTRING(fun)->ptr);
  ret = ei_rpc(in_s->cnode, in_s->fd, mod, RSTRING(fun)->ptr, rpcargs.buff, rpcargs.index, &result);
  if(ret < 0)
    raise_rException_for_erl_errno();

  /* print the result */
  index = 0;
  printf("rpc response:\n");
  ei_print_term(stdout, result.buff, &index);
  fflush(stdout);
  printf("\n");

  printf("Trying to parse some stuff:\n");
  ETERM *tuplep;
  tuplep = erl_decode(result.buff);
  rb_value_from_eterm(tuplep);

  /* free up memory */
  erl_free_compound(tuplep);
  ei_x_free(&rpcargs);
  ei_x_free(&result);
  return Qnil;
}

static VALUE rb_value_from_eterm(ETERM *eterm)
{
  ETERM *member, *tail;
  int size, i = 1;

  /* figure out the type of the eterm (more to come) */
  if (ERL_IS_ATOM(eterm))
    rb_value_from_atom(eterm);
  if (ERL_IS_TUPLE(eterm))
    rb_value_from_tuple(eterm);
  if (ERL_IS_LIST(eterm))
    rb_value_from_list(eterm, 0);

  erl_free_compound(eterm);
  return Qnil;
}

static VALUE rb_value_from_list(ETERM *list, int is_member){
  ETERM *tail;
  int i = 1;
  int size = erl_length(list);

  if (!is_member) printf("[");
  for (i; i <= size; i++) {
    rb_value_from_eterm(erl_hd(list));
    tail = erl_tl(list);
    if(!ERL_IS_EMPTY_LIST(tail)) {
      if (!(i==size)) printf(", ");
      rb_value_from_list(tail, 1);
    }
    erl_free_compound(tail);
  }

  if (!is_member) printf("]");

  erl_free_compound(list);
  return Qnil;
}

static VALUE rb_value_from_tuple(ETERM *tuple){
  int i = 1;
  int size = erl_size(tuple);
  ETERM *member;

  printf("{");
  for (i; i <= size; i++)
    {
      member = erl_element(i, tuple);
      rb_value_from_eterm(member);
      if (!(i==size)) printf(", ");
      erl_free_term(member);
    }

  printf("}");
  erl_free_compound(tuple);
  return Qnil;
}

static VALUE rb_value_from_atom(ETERM *atom){
  printf(ERL_ATOM_PTR(atom));
  erl_free_term(atom);
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
