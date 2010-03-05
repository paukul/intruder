#ifndef INVADER_NODE_H
#define INVADER_NODE_H

#include "ruby.h"
#include "ei_connect.h"
#include "ei.h"

#define INVADER_DEBUG

#ifdef INVADER_DEBUG
#define DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif

#define CLASS_STRUCT struct invader_node *class_struct; Data_Get_Struct(self, struct invader_node, class_struct)

struct invader_node
{
  ei_cnode *cnode;
  int fd;         /* file descriptor for the communication with the epmd */
};

/* prototypes */
void Init_invader_node();

/* ruby instance methods */
VALUE invader_node_init(VALUE self, VALUE host, VALUE sname, VALUE cookie);
VALUE invader_node_connect(VALUE self, VALUE remote_node);

/* ruby class methods */
VALUE invader_node_new(VALUE class, VALUE host, VALUE sname, VALUE cookie);
#endif
