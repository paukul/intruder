#ifndef INTRUDER_EXT_H_
#define INTRUDER_EXT_H_

#include "ruby.h"
#include "node.h"
#include "mod.h"
#include "term.h"
#include "list.h"
#include "atom.h"
#include "binary.h"

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#define RSTRING_LEN(s) (RSTRING(s)->len)
#define RARRAY_LEN(a) (RARRAY(a)->len)
#endif

extern void raise_rException_for_erl_errno();
#endif /* #ifndef INTRUDER_EXT_H */

/*
 * TODO: Add Check_Type checks for every method that needs a specific argument type 
 * TODO: macros for retrival of the class structs like:
 *       #define GetDBM(obj, dbmp) {\
 *          Data_Get_Struct(obj, struct dbmdata, dbmp);\
 *          if (dbmp->di_dbm == 0) closed_dbm();\
 *       }
*/
