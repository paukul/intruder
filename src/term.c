#include "term.h"

VALUE IntruderTerm = Qnil;

/* internal methods */
void free_intruder_term(void *term) {
  INTRUDER_TERM *iterm = term;
  erl_free_compound(iterm->eterm);
  free(term);
}

INTRUDER_TERM * new_intruder_term() {
  INTRUDER_TERM *iterm = malloc(sizeof(INTRUDER_TERM));
  return iterm;
}

/* instance methods */

VALUE intruder_term_to_s(VALUE self) {
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  ETERM *eterm = iterm->eterm;
  VALUE ret = rb_str_new2("");
  fill_string(&ret, eterm);
  return ret;
}

VALUE rb_value_from_eterm(ETERM *eterm)
{
  VALUE rubyObject;
  INTRUDER_TERM *iterm = new_intruder_term();
  iterm->eterm = eterm;
  if (ERL_IS_LIST(eterm))
    rubyObject = rb_value_from_list(iterm);
  else if (ERL_IS_TUPLE(eterm))
    rubyObject = rb_value_from_tuple(iterm);
  else if (ERL_IS_ATOM(eterm))
    rubyObject = rb_value_from_atom(iterm);
  else
    rubyObject = Data_Wrap_Struct(IntruderTerm, 0, free_intruder_term, iterm);

  return rubyObject;

  /*   figure out the type of the eterm (more to come) */
  /*   if (ERL_IS_BINARY(eterm)) */
  /*     rb_value_from_binary(eterm); */
  /*   erl_free_compound(eterm); */
  /*   return Qnil; */
}

VALUE rb_value_from_list(INTRUDER_TERM *iterm) {
  VALUE rValue;
  iterm->type = INTRUDER_TYPE_LIST;
  rValue = Data_Wrap_Struct(IntruderList, 0, free_intruder_term, iterm);
  return rValue;
}

VALUE rb_value_from_tuple(INTRUDER_TERM *iterm) {
  VALUE rValue;
  iterm->type = INTRUDER_TYPE_TUPLE;
  rValue = Data_Wrap_Struct(IntruderTuple, 0, free_intruder_term, iterm);
  return rValue;
}

VALUE rb_value_from_atom(INTRUDER_TERM *iterm) {
  VALUE rValue;
  iterm->type = INTRUDER_TYPE_ATOM;
  rValue = Data_Wrap_Struct(IntruderAtom, 0, free_intruder_term, iterm);
  return rValue;
}

VALUE rb_value_from_binary(INTRUDER_TERM *iterm) {
  return Qnil;
}

static ETERM *intruder_eterm_from_array(VALUE obj);
VALUE intruder_term_encode(VALUE self, VALUE ruby_object) {
  VALUE ret = Qnil;

  ETERM *eterm;
  switch(TYPE(ruby_object)) {
  case T_SYMBOL :
    eterm = erl_format("~a", rb_id2name(rb_to_id(ruby_object)));
    break;
  case T_ARRAY :
    eterm = intruder_eterm_from_array(ruby_object);
    break;
  default :
    rb_raise(IntruderException, "unable to convert that ruby object to an erlang term");
  }
  ret = rb_value_from_eterm(eterm);
  return ret;
}

static ETERM *intruder_eterm_from_array(VALUE obj) {
  int size = RARRAY_LEN(obj);

  if (size == 0)
    return erl_format("[]");

  ETERM **list = (ETERM **)malloc(sizeof(ETERM*) * size);
  ETERM *element;
  INTRUDER_TERM *iterm;
  VALUE rElement;
  int i;

  for (i = 0; i < size; i++) {
    rElement = rb_ary_shift(obj);
    switch (TYPE(rElement)) {
    case T_SYMBOL :
      element = erl_mk_atom(rb_id2name(SYM2ID(rElement)));
      break;
    default :
      free(list);
      rb_raise(IntruderException, "unable to convert element %d to an erlang term", i);
      return NULL;
    }
    *(list+i) = element;
  }
  return erl_mk_list(list, size);
}

void Init_intruder_term(){
  IntruderTerm = rb_define_class_under(IntruderModule, "Term", rb_cObject);

  /* instance methods */
  rb_define_method(IntruderTerm, "to_s", intruder_term_to_s, 0);
  rb_define_singleton_method(IntruderTerm, "encode", intruder_term_encode, 1);
}
