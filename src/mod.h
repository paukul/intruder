#ifndef INTRUDER_MOD_H_
#define INTRUDER_MOD_H_

#include "ruby.h"
#include "ei_connect.h"
#include "ei.h"

struct intruder_mod
{
  ei_cnode *node;
};

void Init_intruder_mod();

VALUE intruder_mod_init(VALUE self, VALUE modname, VALUE node);
VALUE intruder_mod_alloc(VALUE self);

#endif
