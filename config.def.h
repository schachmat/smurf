#ifndef _SMURF_CONFIG_H
#define _SMURF_CONFIG_H

#include "util.h"

#define MODKEY GDK_CONTROL_MASK

static Key keys[] = {
    /* modifier              keyval      function    arg */
    { MODKEY|GDK_SHIFT_MASK, GDK_o,      NULL,  { 0 } },
};

#endif
