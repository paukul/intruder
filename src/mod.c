#include "mod.h"

VALUE IntruderMod = Qnil;
extern VALUE IntruderModule;

VALUE intruder_mod_init(VALUE self, VALUE modname, VALUE node){
  struct intruder_mod *im;
  ei_cnode *cn;
  Data_Get_Struct(self, struct intruder_mod, im);
  Data_Get_Struct(node, ei_cnode, cn);
  im->node = cn;
  return self;
}

VALUE intruder_mod_alloc(VALUE class){
  struct intruder_mod *im = (struct intruder_mod*)malloc(sizeof(struct intruder_mod));
  VALUE obj;
  obj = Data_Wrap_Struct(class, 0, free, im);
  return obj;
}

void Init_intruder_mod(){
  IntruderMod = rb_define_class_under(IntruderModule, "Mod", rb_cObject);

  /* class methods */
  rb_define_alloc_func(IntruderMod, intruder_mod_alloc);

  /* instance methods */
  rb_define_method(IntruderMod, "initialize", intruder_mod_init, 2);
}
