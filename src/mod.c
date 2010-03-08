#include "mod.h"

VALUE IntruderMod = Qnil;
extern VALUE IntruderModule;

static VALUE rb_value_from_eterm(char *eterm, int *index);

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

  /* prepare the erlang stuff (wtf o_O) */
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
  printf("rpc response: ");
  ei_print_term(stdout, result.buff, &index);
  fflush(stdout);
  printf("\n");
  index = 0;
  rb_value_from_eterm(result.buff, &index);

  /* free up memory */
  ei_x_free(&rpcargs);
  ei_x_free(&result);
  return Qnil;
}

static VALUE rb_value_from_eterm(char *eterm, int *index)
{
  printf("index %d \n", *index);
  int type, size, arity, i;

  if (ei_get_type(eterm, index, &type, &size) < 0)
    DEBUG("ERROR determining type");
  
  DEBUG("type %c\n", type);
  switch (type)
    {
    case ERL_SMALL_TUPLE_EXT :
      DEBUG("small tuple\n");
      int header_index = 0;
      ei_decode_tuple_header(eterm, &header_index, &arity);

      DEBUG("decoding %d tuple elements \n", arity);
      DEBUG("{ \n");
      for (i = 1; i <= arity; i++)
        {
          ++*index;
          rb_value_from_eterm(eterm, index);
        }
      DEBUG(" }\n");
      break;
    case ERL_LARGE_TUPLE_EXT :
      DEBUG("large tuple\n");
      break;
    case ERL_ATOM_EXT :
      DEBUG("atom ");
      char *buff;
      ei_decode_atom(eterm, index, buff);
      DEBUG("%s", buff);
      break;
    default :
        DEBUG("undef\n");
    }
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
