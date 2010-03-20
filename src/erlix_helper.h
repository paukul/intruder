#ifndef ERLIX_HELPER_H
#define ERLIX_HELPER_H

#include "ruby.h"
#include "erl_interface.h"
#include "ei.h"

#include <ctype.h>
#include <string.h>

void fill_string(VALUE *str, ETERM *ep);

#endif
