#ifndef __LUCID_LUCID_BASE_H__
#define __LUCID_LUCID_BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <glib.h>

#include <mutex.h>
#include <atomic.h>

#ifndef bool_t
typedef int bool_t;
#endif

#ifndef LEventID
typedef uint64_t LEventID;
#endif

#ifndef lt_hashtable_pair
typedef struct 
{
    void * key;
    void * value;
} lt_hashtable_pair;
#endif

// FUNCTIONS

G_BEGIN_DECLS

void lt_type_init();

char * lt_base64_encode(const unsigned char * in, int len);
unsigned char * lt_base64_decode(const char * in, int * len);

G_END_DECLS

#ifdef __cplusplus

static inline char * sstrdup(char * s)
{
    g_assert(s != NULL);
    return g_strdup(s);
}

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

#ifndef LT_NO_DEBUG
#define LT_DEBUG(fmt, ...)  g_print(fmt, __VA_ARGS__)
#else
#define LT_DEBUG(fmt, ...)
#endif

#include <ltype.hpp>
#include <larray.hpp>
#include <lbase.hpp>
#include <lmodule.hpp>
#include <lhashtable.hpp>
#include <lobject.hpp>
#include <lxml.hpp>
#include <lset.hpp>

#else

//FIXME- implement this
typedef void LError;

typedef void LType;

void * lt_type_create_instance(LType * type);
LType * lt_type_from_name(const char * klass_name);

typedef void LBase;
#define LT_BASE(obj)    ((LBase *)(obj))

LBase * lt_base_ref(LBase * self);
void lt_base_unref(LBase * self);

typedef void LArray;

typedef int lt_array_compare_func(const void * value1, const void * value2);

LArray * lt_array_create(void (* val_free_fn) (void *), int elem_size);
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
char * lt_array_get_data(LArray * self);

typedef void LHashtable;

LHashtable * lt_hashtable_create(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));
LHashtable * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *));
void lt_hashtable_insert(LHashtable * self, void * key, void * value);
void * lt_hashtable_lookup(LHashtable * self, void * key);
bool_t lt_hashtable_remove(LHashtable * self, void * key);
void lt_hashtable_clear(LHashtable * self);
int lt_hashtable_count(LHashtable * self);
LArray * lt_hashtable_get_keys(LHashtable * self);
LArray * lt_hashtable_get_values(LHashtable * self);

typedef void LModule;

LModule * lt_module_load(const char * file_name);
const char * lt_module_get_name(LModule * self);
void * lt_module_get_symbol(LModule * self, const char * symbol_name);

typedef void LEvent;

typedef void LObject;

typedef void lt_object_event_func(LObject * sender, LHashtable * args, void * user_data);

LObject * lt_object_create();
LEventID lt_object_add_handler(LObject * self, char * event_name, 
    lt_object_event_func * event_func, void * user_data, void (* val_free_fn) (void *));
bool_t lt_object_remove_handler(LObject * self, LEventID event_id);
LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data); 

typedef void LXml;

typedef void lt_xml_event_func(const char * element, const char ** attr_names,
	const char ** attr_values, void * user_data);

LXml * lt_xml_create(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *));
bool_t lt_xml_parse_buffer(LXml * self, const char * data, int len, bool_t is_final, LError ** err);
bool_t lt_xml_parse_file(LXml * self, const char * file_name, LError ** err);

typedef void LSet;

LSet * lt_set_create(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *));
LSet * lt_set_str_create(bool_t owns_strings);
void lt_set_add(LSet * self, void * element);
bool_t lt_set_remove(LSet * self, void * element);
bool_t lt_set_contains(LSet * self, void * element);
void lt_set_clear(LSet * self);
int lt_set_count(LSet * self);
LArray * lt_set_get_elements(LSet * self);
void lt_set_foreach(LSet * self, void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);

#endif

#endif
