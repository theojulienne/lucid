#ifndef __LUCID_ATOMIC_H__
#define __LUCID_ATOMIC_H__

#ifdef REAL_GLIB

#define lt_atomic_get_int(ptr)					g_atomic_int_get(ptr)
#define lt_atomic_set_int(ptr, new_val)				g_atomic_int_set(ptr, new_val)
#define lt_atomic_add_int(ptr, val)				g_atomic_int_add(ptr, val)
#define lt_atomic_add_xfer_int(ptr, val)			g_atomic_int_exchange_and_add(ptr, val)
#define lt_atomic_compare_xfer_int(ptr, old_val, new_val)	g_atomic_int_compare_and_exchange(ptr, old_val, new_val)    

#else

G_BEGIN_DECLS

int lt_atomic_get_int(volatile int * ptr);

void lt_atomic_set_int(volatile int * ptr, int new_val);

void lt_atomic_add_int(volatile int * ptr, int val);

int lt_atomic_add_xfer_int(volatile int * ptr, int val);

int lt_atomic_compare_xfer_int(volatile int * ptr, int old_val, int new_val);

G_END_DECLS

#endif

#endif

