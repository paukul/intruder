#include "intruder_ext.h"
#include "node.h"
#include "mod.h"

/* common imports */
#include <stdlib.h>
#include <string.h>

/* TODO in den header*/
VALUE IntruderModule = Qnil;
extern VALUE IntruderException;

void Init_intruder_ext(){
  erl_init(NULL, 0);
  IntruderModule = rb_define_module("Intruder");
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
        rb_raise(IntruderException, "Host unreachable");
        break;
      case ENOMEM :
        rb_raise(IntruderException, "Memory Error");
        break;
      case EIO :
        rb_raise(IntruderException, "IO error");
        break; 
      }
}
