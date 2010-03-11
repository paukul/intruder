/*
 * taken from erlix erlix
 * http://github.com/KDr2/erlix
 */

#include "erlix_helper.h"

static int  is_printable_list(const ETERM* term);
static void fill_printable_list(VALUE *str, const ETERM* ep);

static int is_printable_list(const ETERM* term)
{
  while (ERL_TYPE(term) == ERL_LIST) {
    ETERM* head = (ETERM*)(ERL_CONS_HEAD(term));
    if (ERL_INT_VALUE(head) < 0 || ERL_INT_VALUE(head) > 255) {
      return 0;
    }
    if (ERL_INT_VALUE(head) < ' ') {
      switch (ERL_INT_VALUE(head)) {
      case '\n':
      case '\r':
      case '\t':
      case '\v':
      case '\b':
      case '\f':
        break;
      default:
        return 0;
      }
    }
    term = (ETERM*)(ERL_CONS_TAIL(term));
  }
  return ERL_IS_EMPTY_LIST(term);
}

static void fill_printable_list(VALUE *str, const ETERM* ep){
  ID concat=rb_intern("concat");
  rb_funcall(*str,concat,1,rb_str_new2("\""));

  while (ERL_IS_CONS(ep)) {
    int c = ERL_INT_VALUE((ETERM*)ERL_CONS_HEAD(ep));
    if (c >= ' ') {
      char tmp_buf[2];
      tmp_buf[0]=c;
      tmp_buf[1]=0;
      rb_funcall(*str,concat,1,rb_str_new2(tmp_buf));
    }
    else {
      if(c=='\n'){
        rb_funcall(*str,concat,1,rb_str_new2("\\n"));
      }else if(c=='\r'){
        rb_funcall(*str,concat,1,rb_str_new2("\\r"));
      }else if(c=='\t'){
        rb_funcall(*str,concat,1,rb_str_new2("\\t"));
      }else if(c=='\v'){
        rb_funcall(*str,concat,1,rb_str_new2("\\v"));
      }else if(c=='\b'){
        rb_funcall(*str,concat,1,rb_str_new2("\\b"));
      }else if(c=='\f'){
        rb_funcall(*str,concat,1,rb_str_new2("\\f"));
      }else{
        char tmp_buf[8];
        memset(tmp_buf,0,8);
        sprintf(tmp_buf,"\\%o",c);
        rb_funcall(*str,concat,1,rb_str_new2(tmp_buf));
      }
    }
    ep = (ETERM*)ERL_CONS_TAIL(ep);
  }
  rb_funcall(*str,concat,1,rb_str_new2("\""));
}

void fill_string(VALUE *str,ETERM *ep){
  ID concat=rb_intern("concat");

  int j,i,doquote;
  if (!ep) return;

  j = i = doquote = 0;

  if(ERL_IS_ATOM(ep)){
    /* FIXME: what if some weird locale is in use? */
    if (!islower((int)ERL_ATOM_PTR(ep)[0]))
      doquote = 1;
    for (i = 0; !doquote && i < ERL_ATOM_SIZE(ep); i++){
      doquote = !(isalnum((int)ERL_ATOM_PTR(ep)[i])
                  || (ERL_ATOM_PTR(ep)[i] == '_'));
    }
    if (doquote) {
      rb_funcall(*str,concat,1,rb_str_new2("\'"));
    }
    rb_funcall(*str,concat,1,rb_str_new2(ERL_ATOM_PTR(ep)));
    if(doquote){
      rb_funcall(*str,concat,1,rb_str_new2("\'"));
    }
  }else if(ERL_IS_PID(ep)){
    char tmp_buf_pid[24];
    memset(tmp_buf_pid,0,24);
    sprintf(tmp_buf_pid,"<%s.%d.%d>",ERL_PID_NODE(ep),ERL_PID_NUMBER(ep), ERL_PID_SERIAL(ep));
    rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_pid));
  }else if(ERL_IS_PORT(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("#Port"));
  }else if(ERL_IS_REF(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("#Ref"));
  }else if(ERL_IS_EMPTY_LIST(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("[]"));
  }else if(ERL_IS_LIST(ep)){
    if (is_printable_list(ep)) {
      fill_printable_list(str,ep);
    } else {
      rb_funcall(*str,concat,1,rb_str_new2("["));
      ETERM *tl=ep,*hd,*tmp;
      while (ERL_IS_CONS(tl)){
        hd=erl_hd(tl);
        fill_string(str,hd);
        //erl_free_term(hd);
        tmp = erl_tl(tl);
        //if(tl!=ep)erl_free_term(tl);
        tl=tmp;
        if (ERL_IS_CONS(tl)) {
          rb_funcall(*str,concat,1,rb_str_new2(", "));
        }
      }
      if (!ERL_IS_EMPTY_LIST(tl)) {
        rb_funcall(*str,concat,1,rb_str_new2("|"));
        fill_string(str, tl);
      }else{
        //erl_free_term(tl);
      }
      rb_funcall(*str,concat,1,rb_str_new2("]"));
    }
  }else if(ERL_IS_TUPLE(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("{"));
    for (i=0; i < ERL_TUPLE_SIZE(ep); i++) {
      ETERM *e=erl_element(i+1,ep);
      fill_string(str,e);
      //erl_free_term(e);
      if (i != ERL_TUPLE_SIZE(ep)-1) {
        rb_funcall(*str,concat,1,rb_str_new2(", "));
      }
    }
    rb_funcall(*str,concat,1,rb_str_new2("}"));
  }else if(ERL_IS_BINARY(ep)){
      rb_funcall(*str,concat,1,rb_str_new2("#Bin"));
  }else if(ERL_IS_INTEGER(ep)){
      char tmp_buf_num[24];
      memset(tmp_buf_num,0,24);
      sprintf(tmp_buf_num,"%d", ERL_INT_VALUE(ep));
      rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_num));
  }else if(ERL_IS_UNSIGNED_INTEGER(ep)){
    char tmp_buf_unum[24];
    memset(tmp_buf_unum, 0, 24);
    sprintf(tmp_buf_unum, "%u", ERL_INT_UVALUE(ep));
    rb_funcall(*str,concat, 1, rb_str_new2(tmp_buf_unum));
  }else if(ERL_IS_FLOAT(ep)){
    char tmp_buf_float[24];
    memset(tmp_buf_float, 0, 24);
    sprintf(tmp_buf_float, "%f", ERL_FLOAT_VALUE(ep));
    rb_funcall(*str,concat, 1, rb_str_new2(tmp_buf_float));
  }else{
    rb_funcall(*str,concat, 1, rb_str_new2("*Unknow*"));
    //rb_raise(rb_eException,"ErlixTerm.to_s: Bad type of term !");
  }
}
