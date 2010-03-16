#include "intruder_ext.h"

/* common imports */
#include <stdlib.h>
#include <string.h>

/* TODO in den header*/
VALUE IntruderModule = Qnil;
VALUE IntruderException = Qnil;
extern VALUE IntruderNodeException;

void Init_intruder_ext(){
  erl_init(NULL, 0);
  IntruderModule = rb_define_module("Intruder");
  IntruderException = rb_define_class_under(IntruderModule, "Error", rb_eRuntimeError);

  Init_intruder_term();
  Init_intruder_list();
  Init_intruder_tuple();
  Init_intruder_atom();
  Init_intruder_node();
  Init_intruder_mod();
}

void raise_rException_for_erl_errno(){
    DEBUG("Result: %d\n", erl_errno);
    DEBUG("ERHOSTUNREACH: %d\n", EHOSTUNREACH);
    DEBUG("ENOMEM: %d\n", ENOMEM);
    DEBUG("EIO: %d\n", EIO);

    switch( erl_errno )
      {
      case EHOSTUNREACH :
        rb_raise(IntruderNodeException, "Host unreachable");
        break;
      case ENOMEM :
        rb_raise(IntruderNodeException, "Memory Error");
        break;
      case EIO :
        rb_raise(IntruderNodeException, "IO error");
        break;
      }
}
