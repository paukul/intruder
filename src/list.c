#include "list.h"

VALUE IntruderList = Qnil;

void Init_intruder_list()
{
  IntruderList = rb_define_class_under(IntruderModule, "List", IntruderTerm);

  rb_define_method(IntruderList, "each", intruder_list_each, 0);
}

/* Enumerable */
VALUE intruder_list_each(VALUE self)
{
  INTRUDER_TERM *term;
  ETERM *member, *list, *old_list;
  VALUE rMember;
  int i;

  Data_Get_Struct(self, INTRUDER_TERM, term);
  list = erl_copy_term(term->eterm);
  int length = erl_length(list);

  for(i = 1; i <= length; i++)
    {
      member = erl_hd(list);
      old_list = list;
      list = erl_tl(list);
      rMember = rb_value_from_eterm(member);
      rb_yield(rMember);
    }
}
