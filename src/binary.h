#ifndef INTRUDER_BINARY_H
#define INTRUDER_BINARY_H

#include "ruby.h"
#include "term.h"

extern VALUE IntruderModule;
extern VALUE IntruderTerm;

void Init_intruder_binary();
VALUE intruder_binary_init(VALUE self, VALUE str);

#endif /* INTRUDER_BINARY_H */
