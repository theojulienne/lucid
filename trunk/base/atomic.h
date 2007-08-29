#ifndef __LUCID_ATOMIC_H__
#define __LUCID_ATOMIC_H__

#ifdef REAL_GLIB

#define lt_atomic_get_int    g_atomic_int_get
#define lt_atomic_set_int    g_atomic_int_set
#define lt_atomic_add_int    g_atomic_int_add
#define lt_atomic_add_xfer_int   g_atomic_int_exchange_and_add
#define lt_atomic_compare_xfer_int   g_atomic_int_compare_and_exchange    

#else

G_BEGIN_DECLS

int lt_atomic_get_int(volatile int * ptr);

void lt_atomic_set_int(volatile int * ptr, int new_val);

void lt_atomic_add_int(volatile int * ptr, int val);

int lt_atomic_add_xfer_int(volatile int * ptr, int val);

int lt_atomic_compare_xfer_int(volatile int	* ptr, int old_val, int new_val);

G_END_DECLS

#endif

G_BEGIN_DECLS

void _lt_atomic_init();

G_END_DECLS

#endif

