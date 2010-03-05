#ifndef INTRUDER_NODE_H
#define INTRUDER_NODE_H

#include "ruby.h"
#include "ei_connect.h"
#include "ei.h"

#define INTRUDER_DEBUG

#ifdef INTRUDER_DEBUG
#define DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif

#define CLASS_STRUCT struct intruder_node *class_struct; Data_Get_Struct(self, struct intruder_node, class_struct)

struct intruder_node
{
  ei_cnode *cnode;
  int fd;         /* file descriptor for the communication with the epmd */
};

/* prototypes */
void Init_intruder_node();

/* ruby instance methods */
VALUE intruder_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie);
VALUE intruder_node_connect(VALUE self, VALUE remote_node);

/* ruby class methods */
VALUE intruder_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie);
#endif
