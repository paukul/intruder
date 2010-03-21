#include "list.h"

VALUE IntruderList = Qnil;

void Init_intruder_list()
{
  IntruderList = rb_define_class_under(IntruderModule, "List", IntruderTerm);

  rb_include_module(IntruderList, rb_mEnumerable);
  rb_define_method(IntruderList, "each", intruder_list_each, 0);
  rb_define_method(IntruderList, "[]", intruder_list_member_at, 1);
  rb_define_method(IntruderList, "length", intruder_list_length, 0);
  rb_define_alias(IntruderList, "size", "length");
}

/* lenght and size */
VALUE intruder_list_length(VALUE self){
  INTRUDER_TERM *term;
  Data_Get_Struct(self, INTRUDER_TERM, term);

  return INT2FIX(erl_length(term->eterm));
}

/* [] */
VALUE intruder_list_member_at(VALUE self, VALUE pos)
{
  int position = NUM2INT(pos) + 1;
  if (position <= 0 || NUM2INT(intruder_list_length(self)) < position)
    return Qnil;

  INTRUDER_TERM *term;
  Data_Get_Struct(self, INTRUDER_TERM, term);

  int i;
  ETERM *member, *list;
  list = erl_copy_term(term->eterm);

  for(i = 1; i <= position; i++)
    {
      member = erl_hd(list);
      list = erl_tl(list);
    }

  erl_free_compound(list);
  return rb_value_from_eterm(member);
}

/* Enumerable */
VALUE intruder_list_each(VALUE self)
{
  INTRUDER_TERM *term;
  ETERM *member, *list;
  VALUE rMember;
  int i;

  Data_Get_Struct(self, INTRUDER_TERM, term);
  list = erl_copy_term(term->eterm);
  int length = erl_length(list);

  for(i = 1; i <= length; i++)
    {
      member = erl_hd(list);
      list = erl_tl(list);
      rMember = rb_value_from_eterm(member);
      rb_yield(rMember);
    }

  erl_free_compound(list);
  return Qnil;
}
