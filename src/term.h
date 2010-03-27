#ifndef INTRUDER_TERM_H
#define INTRUDER_TERM_H

#include "ruby.h"
#include "ei.h"
#include "erl_interface.h"

#include "intruder_ext.h"
#include "list.h"
#include "erlix_helper.h"

#define INTRUDER_TYPE_ATOM       0
#define INTRUDER_TYPE_LIST       1
#define INTRUDER_TYPE_TUPLE      2
#define INTRUDER_TYPE_INT        3
#define INTRUDER_TYPE_UINT       4
#define INTRUDER_TYPE_FLOAT      5
#define INTRUDER_TYPE_PID        6
#define INTRUDER_TYPE_PORT       7
#define INTRUDER_TYPE_REF        8
#define INTRUDER_TYPE_BIN        9
#define INTRUDER_TYPE_CON       10
#define INTRUDER_TYPE_STRING    11
#define INTRUDER_TYPE_BINARY    12

extern VALUE IntruderModule;
extern VALUE IntruderList;
extern VALUE IntruderTuple;
extern VALUE IntruderAtom;
extern VALUE IntruderException;
extern VALUE IntruderBinary;

typedef struct intruder_term
{
  ETERM *eterm;
  int type;
} INTRUDER_TERM;

void Init_intruder_term();

/* instance methods */
VALUE intruder_term_to_s(VALUE self);
VALUE intruder_term_encode(VALUE self, VALUE ruby_object);

  /* allocation */
VALUE intruder_term_alloc(VALUE class);
void free_intruder_term(void *term);
INTRUDER_TERM *new_intruder_term();

/* C Methods */
VALUE rb_value_from_eterm(ETERM *eterm);
VALUE rb_value_from_list(INTRUDER_TERM *iterm);
VALUE rb_value_from_tuple(INTRUDER_TERM *iterm);
VALUE rb_value_from_atom(INTRUDER_TERM *iterm);
VALUE rb_value_from_binary(INTRUDER_TERM *iterm);
ETERM *intruder_eterm_from_value(VALUE obj);

#endif /* ifndef INTRUDER_TERM_H */
