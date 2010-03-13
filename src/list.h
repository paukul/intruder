#ifndef INTRUDER_LIST_H
#define INTRUDER_LIST_H

#include "ruby.h"

#include "term.h"

extern VALUE IntruderModule;
extern VALUE IntruderTerm;

void Init_intruder_list();

/* Enumerable */
VALUE intruder_list_each(VALUE self);

#endif /* ifndef INTRUDER_LIST_H */
