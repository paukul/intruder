#include "node.h"
#include "mod.h"

/* common imports */
#include <stdlib.h>
#include <string.h>

VALUE IntruderModule = Qnil;

void Init_intruder_ext(){
  IntruderModule = rb_define_module("Intruder");
  Init_intruder_node();
  Init_intruder_mod();
}
