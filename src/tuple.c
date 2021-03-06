#include "tuple.h"

VALUE IntruderTuple = Qnil;

void Init_intruder_tuple() {
  IntruderTuple = rb_define_class_under(IntruderModule, "Tuple", IntruderTerm);

  rb_include_module(IntruderTuple, rb_mEnumerable);
  rb_define_method(IntruderTuple, "initialize", intruder_tuple_init, 1);
  rb_define_method(IntruderTuple, "each", intruder_tuple_each, 0);
  rb_define_method(IntruderTuple, "[]", intruder_tuple_member_at, 1);
  rb_define_method(IntruderTuple, "length", intruder_tuple_length, 0);
  rb_define_alias(IntruderTuple, "size", "length");
}

VALUE intruder_tuple_each(VALUE self) {
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  int i;
  for (i = 1; i <= erl_size(iterm->eterm); i++) {
    rb_yield(rb_value_from_eterm(erl_element(i, iterm-> eterm)));
  }
  return Qnil;
}

VALUE intruder_tuple_init(VALUE self, VALUE arr) {
  int i, arrsize = RARRAY_LEN(arr);
  INTRUDER_TERM *iterm;
  ETERM **members = (ETERM**)malloc(sizeof(ETERM*) * arrsize);

  for (i = 0; i < arrsize; i++) {
    members[i] = intruder_eterm_from_value(rb_ary_shift(arr));
  }

  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  iterm->eterm = erl_mk_tuple(members, arrsize);
  return self;
}

VALUE intruder_tuple_member_at(VALUE self, VALUE position) {
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);

  ETERM *member;
  int iPosition = NUM2INT(position);
  int tuple_size = erl_size(iterm->eterm);
  if (iPosition < 0 || tuple_size < iPosition+1) {
    return Qnil;
  } else {
    member = erl_element(iPosition+1, iterm->eterm);
    return rb_value_from_eterm(member);
  }
}

VALUE intruder_tuple_length(VALUE self) {
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  int iLength;
  iLength = erl_size(iterm->eterm);
  return INT2NUM(iLength);
}
