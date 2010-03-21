#ifndef INTRUDER_TUPLE_H
#define INTRUDER_TUPLE_H

#include "ruby.h"
#include "term.h"

extern VALUE IntruderModule;
extern VALUE IntruderTerm;

void Init_intruder_tuple();

VALUE intruder_tuple_each(VALUE self);
VALUE intruder_tuple_init(VALUE self, VALUE arr);
VALUE intruder_tuple_member_at(VALUE self, VALUE position);
VALUE intruder_tuple_length(VALUE self);

#endif /* ifndef INTRUDER_TUPLE_H */
