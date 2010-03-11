#include "term.h"

VALUE IntruderTerm = Qnil;

/* internal methods */
void free_intruder_term(void *term){
  INTRUDER_TERM *iterm = term;
  erl_free_compound(iterm->eterm);
  free(term);
}

INTRUDER_TERM * new_intruder_term(){
  INTRUDER_TERM *iterm = malloc(sizeof(INTRUDER_TERM));
  return iterm;
}

/* instance methods */

VALUE intruder_term_to_s(VALUE self){
  INTRUDER_TERM *iterm;
  Data_Get_Struct(self, INTRUDER_TERM, iterm);
  ETERM *eterm = iterm->eterm;
  VALUE ret = rb_str_new2("");
  fill_string(&ret, eterm);
  return ret;
}

void Init_intruder_term(){
  IntruderTerm = rb_define_class_under(IntruderModule, "Term", rb_cObject);

  /* instance methods */
  rb_define_method(IntruderTerm, "to_s", intruder_term_to_s, 0);
}
