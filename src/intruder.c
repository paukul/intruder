#include "node.h"

/* common imports */
#include <stdlib.h>
#include <string.h>

VALUE IntruderModule = Qnil;

void Init_intruder(){
  IntruderModule = rb_define_module("Intruder");
  Init_intruder_node();
}
