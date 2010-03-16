#include "atom.h"

VALUE IntruderAtom = Qnil;

void Init_intruder_atom(){
  IntruderAtom = rb_define_class_under(IntruderModule, "Atom", IntruderTerm);
}
