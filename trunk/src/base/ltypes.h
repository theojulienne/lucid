#ifndef __LUCID_LTYPES_H__
#define __LUCID_LTYPES_H__

#include <stdio.h>
#include <stdint.h>

G_BEGIN_DECLS

// VARIOUS TYPE DEFINITIONS

typedef int bool_t;

typedef uint64_t LEventID;

typedef struct 
{
    void * key;
    void * value;
} lt_hashtable_pair;

G_END_DECLS

#endif
