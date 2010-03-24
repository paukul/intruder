/* FIXME node.h =schlechte idee, umbenennen */
#ifndef INTRUDER_NODE_H
#define INTRUDER_NODE_H

#include "ruby.h"
#include "erl_interface.h"
#include "ei.h"

#include "intruder_ext.h"

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define INTRUDER_DEBUG

#ifdef INTRUDER_DEBUG
#define DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif

#define INTRUDER_CONNECTED 0
#define INTRUDER_DISCONNECTED 1

#define CLASS_STRUCT struct intruder_node *class_struct; Data_Get_Struct(self, struct intruder_node, class_struct)

typedef struct intruder_node
{
  ei_cnode *cnode;
  int fd;         /* file descriptor for the communication with the epmd */
  int status;
} INTRUDER_NODE;

extern VALUE IntruderModule;
extern VALUE IntruderException;

/* prototypes */
void Init_intruder_node();

/* ruby instance methods */
VALUE intruder_node_init(VALUE self, VALUE sname, VALUE cookie);
VALUE intruder_node_connect(VALUE self, VALUE remote_node);
/* VALUE intruder_node_mod(VALUE self, VALUE mod_name); */

/* ruby class methods */
VALUE intruder_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie);
#endif
