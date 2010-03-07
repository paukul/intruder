#ifndef INTRUDER_MOD_H_
#define INTRUDER_MOD_H_

#include "ruby.h"
#include "erl_interface.h"
#include "ei.h"

#include "intruder_ext.h"
#include "node.h"


typedef struct intruder_mod
{
  ei_cnode *node;
} INTRUDER_MOD;

void Init_intruder_mod();

VALUE intruder_mod_init(VALUE self, VALUE modname, VALUE node);
VALUE intruder_mod_alloc(VALUE self);
VALUE private_intruder_mod_rpc(VALUE self, VALUE args);

#endif
