#ifndef _SMURF_CONFIG_H
#define _SMURF_CONFIG_H

#define MODKEY GDK_CONTROL_MASK

static Key keys[] = {
    /* modifier              keyval      function    arg */
    { MODKEY|GDK_SHIFT_MASK, GDK_o,      inspector,  { 0 } },
};

#endif
