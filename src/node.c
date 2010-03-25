/*
 * Helpfull stuff:
 * Erl_interface doku: http://erlang.mirror.su.se/doc/man/ei_connect.html
 * http://erlang.org/pipermail/erlang-questions/2003-August/009536.html
 * Erlix discussion: http://www.mentby.com/Group/ruby-talk/ruby-interface-of-erlang.html
 * Extending ruby (german): http://home.vrweb.de/~juergen.katins/ruby/buch/ext_ruby.html
 * http://www.eqqon.com/index.php/Ruby_C_Extension#debug
 */
#include "node.h"

#define STOSYM(string) ID2SYM(rb_intern(string))

VALUE IntruderNode = Qnil;
VALUE IntruderNodeException = Qnil;
int node_count = 0;
pthread_t alive_thread;
INTRUDER_NODE **connectlist;
fd_set socks;
int highsock;
int readsocks;
int connectlist_inited = 0;

/* internal methods */
static void declare_attr_accessors();
static void free_class_struct(void *class_struct);

void lock_node(INTRUDER_NODE *node);
void unlock_node(INTRUDER_NODE *node);
void read_socks();
void *aliveloop();
void build_select_list();

/* implementation */
VALUE intruder_node_alloc(VALUE class){
  INTRUDER_NODE *class_struct = malloc(sizeof(INTRUDER_NODE));
  class_struct->cnode = malloc(sizeof(ei_cnode));
  class_struct->status = INTRUDER_DISCONNECTED;
  class_struct->mutex = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(class_struct->mutex, NULL);

  VALUE obj = Data_Wrap_Struct(class, 0, free_class_struct, class_struct);
  return obj;
}

static void free_class_struct(void *class_struct)
{
  pthread_mutex_destroy(((INTRUDER_NODE *)class_struct)->mutex);
  free(((INTRUDER_NODE *)class_struct)->cnode);
  free(class_struct);
}

VALUE intruder_node_init(VALUE self, VALUE sname, VALUE cookie){
  CLASS_STRUCT;
  rb_iv_set(self, "@sname", sname);
  rb_iv_set(self, "@cookie", cookie);

  if (!connectlist_inited) {
    connectlist = (INTRUDER_NODE **)malloc(sizeof(INTRUDER_NODE*) * CONBUFFSIZE);
    printf("initializing connectlist\n");
    connectlist_inited = 1;
  }

  /* initialize the node */
  if(ei_connect_init(class_struct->cnode, RSTRING_PTR(sname), RSTRING_PTR(cookie), node_count) < 0){
    free(class_struct->cnode);
    rb_raise(rb_eRuntimeError, "Error initializing the node");
  }

  return self;
}

VALUE intruder_node_status(VALUE self){
  CLASS_STRUCT;
  if(class_struct->status == INTRUDER_CONNECTED)
    return STOSYM("connected");
  else
    return STOSYM("disconnected");
}

VALUE intruder_node_pid(VALUE self){
  CLASS_STRUCT;
  erlang_pid *pid = ei_self(class_struct->cnode);
  return INT2FIX(pid->num);
}

VALUE intruder_node_connect(VALUE self, VALUE remote_node){
  INTRUDER_NODE *class_struct;
  Data_Get_Struct(self, INTRUDER_NODE, class_struct);

  if((class_struct->fd = ei_connect(class_struct->cnode, RSTRING_PTR(remote_node))) < 0)
    raise_rException_for_erl_errno();

  class_struct->status = INTRUDER_CONNECTED;

/*   printf("setting node %d on the connectlist\n", node_count); */
  connectlist[node_count] = class_struct;
  node_count++;

  if (alive_thread == NULL) {
    pthread_create(&alive_thread, NULL, aliveloop, NULL);
  }
  return Qtrue;
}

void Init_intruder_node(){
  IntruderNode = rb_define_class_under(IntruderModule, "Node", rb_cObject);
  declare_attr_accessors();

  /* class methods */
  rb_define_alloc_func(IntruderNode, intruder_node_alloc);

  /* instance methods */
  rb_define_method(IntruderNode, "initialize", intruder_node_init, 2);
  rb_define_method(IntruderNode, "connect", intruder_node_connect, 1);
  rb_define_method(IntruderNode, "pid", intruder_node_pid, 0);
  rb_define_method(IntruderNode, "status", intruder_node_status, 0);

  /* exceptions */
  IntruderNodeException = rb_define_class_under(IntruderModule, "NodeError", IntruderException);
}

static void declare_attr_accessors(){
  ID attr_accessor = rb_intern("attr_reader");

  char  *i_vars[2] = {"cookie", "sname"};
  VALUE params[2];

  int i = 0;
  for(; i <= 1; i++){
    params[i] = STOSYM(i_vars[i]);
  }
  rb_funcall2(IntruderNode, attr_accessor, 2, params);
}

void build_select_list() {
  int listnum;
  FD_ZERO(&socks);
/*   printf("building select list\n"); */
  for (listnum = 0; listnum < 5; listnum++) {
    if (connectlist[listnum] != 0) {
/*       printf("adding fd %d (%d) to connectlist\n", connectlist[listnum], listnum); */
      FD_SET(connectlist[listnum]->fd, &socks);
      if (connectlist[listnum]->fd > highsock)
        highsock = connectlist[listnum]->fd;
    }
  }
}

void read_socks() {
  int listnum;
  int got;
  char buf[400];

/*   printf("reading open sockets\n"); */
  printf("Nodecount = %d\n", node_count);
  for (listnum = 0; listnum < node_count; listnum++) {
    printf("checking socket %d (%d)\n", connectlist[listnum]->fd, listnum);
    if (connectlist[listnum] != 0 && FD_ISSET(connectlist[listnum]->fd, &socks)) {
      printf("erl_receive\n");
      got = erl_receive(connectlist[listnum]->fd, buf, 400);
      if (got == ERL_TICK) {
        printf("keeping alive!!\n");
        continue;
      } else {
        rb_raise(rb_eRuntimeError, "Keep Alive thread cought a message other than ERL_TICK");
      }
    }
  }
}

void *aliveloop() {
  struct timeval timeout;

  while(1) {
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    build_select_list();
/*     printf("waiting for sockets\n"); */
    readsocks = select(highsock+1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);

    if (readsocks < 0) {
      perror("select");
      exit(EXIT_FAILURE);
    }
    if (readsocks == 0) {
      /* Nothing ready to read, just show that
         we're alive */
/*       printf("."); */
/*       fflush(stdout); */
    } else
      read_socks();
  }
}

void lock_node(INTRUDER_NODE *node) {
  pthread_mutex_lock(node->mutex);
}

void unlock_node(INTRUDER_NODE *node) {
  pthread_mutex_unlock(node->mutex);
}
