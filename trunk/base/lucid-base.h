#ifndef __LUCID_LUCID_BASE_H__
#define __LUCID_LUCID_BASE_H__

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include <mutex.h>
#include <atomic.h>

#ifndef bool_t
typedef int bool_t;
#endif

#ifdef __cplusplus

#define LT_CALL_SELF_CPP( cpp_func, params... ) \
     G_STMT_START \
     { \
         g_return_if_fail( self != NULL ); \
         self->cpp_func( params ); \
     } \
     G_STMT_END
 
#define LT_RETURN_CALL_SELF_CPP( cpp_func, fail_value, params... ) \
     G_STMT_START \
     { \
         g_return_val_if_fail( self != NULL, fail_value ); \
         return self->cpp_func( params ); \
     } \
     G_STMT_END

#include <ltype.hpp>
#include <larray.hpp>
#include <lbase.hpp>
#include <lhashtable.hpp>
#include <lobject.hpp>

#else

void lt_type_init();

typedef void LType;

void * lt_type_create_instance(LType * type);
LType * lt_type_from_name(const char * klass_name);

typedef void LBase;
#define LT_BASE(obj)    ((LBase *)(obj))

LBase * lt_base_ref(LBase * self);
void lt_base_unref(LBase * self);

typedef void LArray;

typedef int lt_array_compare_func(const void * value1, const void * value2);

LArray * lt_array_create(void (* val_free_fn) (void *));
void lt_array_destroy(LArray * self);
void lt_array_append(LArray * self, void * value);
void lt_array_insert(LArray * self, int index, void * value);
bool_t lt_array_remove(LArray * self, void * value);
bool_t lt_array_remove_index(LArray * self, int index);
void lt_array_reverse(LArray * self);
void lt_array_sort(LArray * self, lt_array_compare_func * compare_func);
void lt_array_foreach(LArray * self, void (* list_foreach_func) (const void * value, 
                                            void * user_arg), 
                                            void * user_arg);
void * lt_array_get_item(LArray * self, int index);
void lt_array_set_item(LArray * self, int index, void * value);
void lt_array_clear(LArray * self);
int lt_array_count(LArray * self);

typedef void LHashtable;

LHashtable * lt_hashtable_create(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));
LHashtable * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *));
void lt_hashtable_insert(LHashtable * self, void * key, void * value);
void * lt_hashtable_lookup(LHashtable * self, void * key);
bool_t lt_hashtable_remove(LHashtable * self, void * key);
void lt_hashtable_clear(LHashtable * self, void * key);
int lt_hashtable_count(LHashtable * self);
LArray * lt_hashtable_get_keys(LHashtable * self);
LArray * lt_hashtable_get_values(LHashtable * self);

typedef void LObject;

LObject * lt_object_create();

#endif

#endif
