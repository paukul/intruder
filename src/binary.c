#include "binary.h"

VALUE IntruderBinary = Qnil;

void Init_intruder_binary() {
  IntruderBinary = rb_define_class_under(IntruderModule, "Binary", IntruderTerm);

  /* class methods */
  rb_define_method(IntruderBinary, "initialize", intruder_binary_init, 1);
}

VALUE intruder_binary_init(VALUE self, VALUE str) {
  ETERM *eterm;
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);

  eterm = erl_mk_binary(RSTRING_PTR(str), RSTRING_LEN(str) + 1);
  iterm->eterm = eterm;
  return self;
}
