#include "binary.h"

VALUE IntruderBinary = Qnil;

void Init_intruder_binary() {
  IntruderBinary = rb_define_class_under(IntruderModule, "Binary", IntruderTerm);

  /* class methods */
  rb_define_method(IntruderBinary, "initialize", intruder_binary_init, 1);

  /* allocation */
  rb_define_alloc_func(IntruderBinary, intruder_binary_alloc);
}

VALUE intruder_binary_alloc(VALUE class) {
  INTRUDER_TERM *iterm = new_intruder_term();
  VALUE obj = Data_Wrap_Struct(class, 0, free_intruder_term, iterm);
  return obj;
}

VALUE intruder_binary_init(VALUE self, VALUE str) {
  ETERM *eterm;
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);

  eterm = erl_mk_binary(RSTRING_PTR(str), RSTRING_LEN(str));
  iterm->eterm = eterm;
  return self;
}
