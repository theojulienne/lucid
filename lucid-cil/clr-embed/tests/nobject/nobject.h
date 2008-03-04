#ifndef __N_OBJECT_H__
#define __N_OBJECT_H__

#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>

typedef struct
{
    guint32 ref_count;
    MonoClass * klass;
    gpointer obj;        
} NObject;

NObject * n_object_new_ref(MonoObject * mono_obj);
NObject * n_object_new_value(MonoClass * klass, gpointer value);
void n_object_destroy(NObject * obj);

static inline NObject * n_object_ref(NObject * obj)
{
    g_return_val_if_fail(obj->ref_count > 0, NULL);

    g_atomic_int_inc(&obj->ref_count);
    return obj;
}

static inline void n_object_unref(NObject * obj)
{
    g_return_if_fail(obj->ref_count > 0);

    if(! g_atomic_int_dec_and_test(&obj->ref_count))
        n_object_destroy(obj);
}

static inline gboolean n_object_get_byte(NObject * obj, guint8 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_byte_class(), FALSE);
    * val = (* (guint8 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_sbyte(NObject * obj, gint8 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_sbyte_class(), FALSE);
    * val = (* (gint8 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_int16(NObject * obj, gint16 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_int16_class(), FALSE);
    * val = (* (gint16 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_uint16(NObject * obj, guint16 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_uint16_class(), FALSE);
    * val = (* (guint16 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_int32(NObject * obj, gint32 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_int32_class(), FALSE);
    * val = (* (gint32 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_uint32(NObject * obj, guint32 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_uint32_class(), FALSE);
    * val = (* (guint32 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_int64(NObject * obj, gint64 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_int64_class(), FALSE);
    * val = (* (gint64 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_uint64(NObject * obj, guint64 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_uint64_class(), FALSE);
    * val = (* (guint64 *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_intptr(NObject * obj, gpointer * val)
{
    g_return_val_if_fail(obj->klass == mono_get_intptr_class(), FALSE);
    * val = (* (gpointer *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_uintptr(NObject * obj, gpointer * val)
{
    g_return_val_if_fail(obj->klass == mono_get_uintptr_class(), FALSE);
    * val = (* (gpointer *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_single(NObject * obj, float * val)
{
    g_return_val_if_fail(obj->klass == mono_get_single_class(), FALSE);
    * val = (* (float *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_double(NObject * obj, double * val)
{
    g_return_val_if_fail(obj->klass == mono_get_double_class(), FALSE);
    * val = (* (double *)&obj->obj); 
    return TRUE;   
}

static inline gboolean n_object_get_boolean(NObject * obj, gboolean * val)
{
    g_return_val_if_fail(obj->klass == mono_get_boolean_class(), FALSE);
    * val = (* (MonoBoolean *)&obj->obj) ? TRUE : FALSE; 
    return TRUE;   
}

static inline gboolean n_object_get_char(NObject * obj, gunichar2 * val)
{
    g_return_val_if_fail(obj->klass == mono_get_char_class(), FALSE);
    * val = (* (gunichar2 *)&obj->obj); 
    return TRUE;   
}

#endif

