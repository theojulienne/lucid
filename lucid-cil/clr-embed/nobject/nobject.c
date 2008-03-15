#include <mono/utils/mono-hash.h>
#include <string.h>

#include "nobject.h"

static MonoGHashTable * _gchandles = NULL;

static guint32 _gchandle_get(MonoObject * mono_obj)
{
    gpointer handle;

    if(_gchandles == NULL)
         _gchandles = mono_g_hash_table_new_type(g_direct_hash,
            g_direct_equal, MONO_HASH_KEY_GC);

    if(! mono_g_hash_table_lookup_extended(_gchandles, mono_obj, NULL, &handle))
    {
        handle = GINT_TO_POINTER(mono_gchandle_new(mono_obj, FALSE));
        mono_g_hash_table_insert(_gchandles, mono_obj, handle);
    }

    return GPOINTER_TO_INT(handle);
}

static void _gchandle_free(guint32 handle)
{
    MonoObject * mono_obj = mono_gchandle_get_target(handle);

    g_return_if_fail(mono_obj != NULL);

    g_assert(mono_g_hash_table_remove(_gchandles, mono_obj));

    mono_gchandle_free(handle);
}

void _gchandle_destroy()
{
    mono_g_hash_table_destroy(_gchandles);
}

#define _PAD(size) ((G_MEM_ALIGN - (size % G_MEM_ALIGN)) % G_MEM_ALIGN)

NObject * n_object_new(MonoObject * mono_obj)
{
    NObject * obj;
    MonoClass * klass;

    g_return_val_if_fail(mono_obj != NULL, NULL);

    klass = mono_object_get_class(mono_obj);

    g_return_val_if_fail(klass != NULL, NULL);
    g_return_val_if_fail(! mono_class_is_valuetype(klass), NULL);

    /*
    gpointer val;
    int size;
    {
        size = mono_class_value_size (klass, NULL);
        obj = g_slice_alloc(sizeof(NObject) + _PAD(size));
        val = mono_object_unbox(mono_obj);
        mono_value_copy((gpointer)&obj->obj, val, klass);
    }
    else
    {
        obj = g_slice_new(NObject);
        obj->obj = _gchandle_get(mono_obj);
    }
    */

    obj = g_slice_new(NObject);

    obj->ref_count = 1;
    obj->klass = klass;
    obj->gc_handle = _gchandle_get(mono_obj);

    return obj;
}

NObject * n_object_ref(Nobject * obj)
{
    return obj;
}

void n_object_unref(NObject * obj)
{
}

void n_val_init(NVal * val, NType type)
{
    g_return_if_fail(val != NULL && ((type > MONO_TYPE_END && type < MONO_TYPE_SZARRAY) || type == MONO_TYPE_ENUM));
    val->type = type;
    memset(val->data, 0, sizeof(val->data));
}

void n_val_set(NVal * val, gpointer value)
{
    g_return_if_fail(val != NULL && value != NULL);
    //switch statement
}

void n_val_unset(NVal * val)
{
    //switch statement
}

/*
NObject * n_object_new_value(MonoClass * klass, gpointer val)
{
    NObject * obj;
    int size;

    g_return_val_if_fail(klass != NULL, NULL);
    g_return_val_if_fail(mono_class_is_valuetype(klass), NULL);

    size = mono_class_value_size (klass, NULL);
    obj = g_slice_alloc(sizeof(NObject) + _PAD(size));
    mono_value_copy((gpointer)&obj->obj, val, klass);
    obj->klass = klass;
    obj->ref_count = 1;

    return obj;
}

void n_object_destroy(NObject * obj)
{
    int size = 0;

    g_return_if_fail(obj != NULL && obj->klass != NULL);

    if(! mono_class_is_valuetype(obj->klass))
        _gchandle_free(obj->obj);
    else
        size = mono_class_value_size(obj->klass, NULL);

    size += sizeof(NObject) + _PAD(size);
    memset(obj, 0, size);
    g_slice_free1(size, obj);
}

*/
