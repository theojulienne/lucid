#ifndef __N_OBJECT_H__
#define __N_OBJECT_H__

#include <mono/metadata/blob.h>
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>

typedef MonoTypeEnum NType;

#define     N_TYPE_BYTE     MONO_TYPE_I1
#define     N_TYPE_SBYTE    MONO_TYPE_U1
#define     N_TYPE_INT16    MONO_TYPE_I2
#define     N_TYPE_UINT16   MONO_TYPE_U2
#define     N_TYPE_INT32    MONO_TYPE_I4
#define     N_TYPE_UINT32   MONO_TYPE_U4
#define     N_TYPE_INT64    MONO_TYPE_I8
#define     N_TYPE_UINT64   MONO_TYPE_U8
#define     N_TYPE_POINTER  MONO_TYPE_I
#define     N_TYPE_SINGLE   MONO_TYPE_R4
#define     N_TYPE_DOUBLE   MONO_TYPE_R8
#define     N_TYPE_CHAR     MONO_TYPE_CHAR
#define     N_TYPE_BOOLEAN  MONO_TYPE_BOOLEAN
#define     N_TYPE_REF      MONO_TYPE_OBJECT
#define     N_TYPE_STRUCT   MONO_TYPE_VALUETYPE

typedef struct
{
    volatile guint32 ref_count;
    MonoClass * klass;
    guint32 gc_handle;
} NObject;

typedef struct
{
    NType type;
    union
    {
        gint32	v_int32;
        guint32	v_uint32;
        gint64  v_int64;
        guint64 v_uint64;
        gfloat	v_single;
        gdouble	v_double;
        gpointer v_pointer;
        NObject * v_ref;
    } data;
} NVal;

#define     N_VAL_TYPE(val)     ((val)->type)
#define     N_VAL_HOLDS(val, type)  (N_VAL_TYPE(val) == (type))

void n_val_init(NVal * val, NType type);
void n_val_set(NVal * val, gpointer value);
void n_val_unset(NVal * val);

NObject * n_object_new(MonoObject * mono_obj);
NObject * n_object_ref(NObject * obj);
void n_object_unref(NObject * obj);

static inline gboolean n_val_get_byte(NVal * val, guint8 * p)
{
    g_return_val_if_fail(N_VAL_HOLDS(val, N_TYPE_BYTE), FALSE);
    g_return_val_if_fail(p != NULL, FALSE);
    *p = val->data.v_int32;
    return TRUE;
}
/*
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

*/

#endif

