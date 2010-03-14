#include "tuple.h"

VALUE IntruderTuple = Qnil;

void Init_intruder_tuple()
{
  IntruderTuple = rb_define_class_under(IntruderModule, "Tuple", IntruderTerm);

  rb_define_method(IntruderTuple, "[]", intruder_tuple_member_at, 1);
  rb_define_method(IntruderTuple, "length", intruder_tuple_length, 0);
  rb_define_alias(IntruderTuple, "size", "length");
}

VALUE intruder_tuple_member_at(VALUE self, VALUE position)
{
  INTRUDER_TERM *iterm;
  ETERM *member;
  int iPosition = NUM2INT(position);
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  member = erl_element(iPosition+1, iterm->eterm);
  return rb_value_from_eterm(member);
}

VALUE intruder_tuple_length(VALUE self)
{
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  int iLength;
  iLength = erl_size(iterm->eterm);
  return INT2NUM(iLength);
}
