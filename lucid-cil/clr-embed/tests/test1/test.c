#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-hash.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    MonoClass * klass;
    gpointer obj;        
} NObject;

#define _PAD(size) ((G_MEM_ALIGN - (size % G_MEM_ALIGN)) % G_MEM_ALIGN)

NObject * n_object_new(MonoObject * mono_obj)
{
    NObject * obj;
    MonoClass * klass;
    gpointer val;
    int size;

    g_return_val_if_fail(mono_obj != NULL, NULL);
    
    klass = mono_object_get_class(mono_obj);
    g_return_val_if_fail(klass != NULL, NULL);

    if(mono_class_is_valuetype(klass))
    {
        size = mono_class_value_size (klass, NULL);
        obj = g_slice_alloc(sizeof(NObject) + _PAD(size));
        val = mono_object_unbox(mono_obj);
        mono_value_copy((gpointer)&obj->obj, val, klass);
    }
    else
    {
        obj = g_slice_new(NObject);
        obj->obj = GINT_TO_POINTER(_gchandle_get(mono_obj));
    }
    obj->klass = klass;
    return obj;
}

// G_MEM_ALIGN
void n_object_free(NObject * obj)
{
    int size = 0;

    g_return_if_fail(obj != NULL && obj->klass != NULL);
    
    if(! mono_class_is_valuetype(obj->klass))
        _gchandle_free(GPOINTER_TO_INT(obj->obj));    
    else
        size = mono_class_value_size(obj->klass, NULL);

    size += sizeof(NObject) + _PAD(size);
    memset(obj, 0, size); 
    g_slice_free1(size, obj); 
}

gboolean n_object_get_int32(NObject * obj, gint32 * val)
{
    g_return_val_if_fail(obj != NULL && obj->klass != NULL, FALSE);
    g_return_val_if_fail(obj->klass == mono_get_int32_class(), FALSE);    

    * val = (* (gint32 *)&obj->obj);

    return TRUE;
}

// Internal API
MonoDelegate * ftnptr_to_delegate (MonoDomain * domain, MonoClass * klass, gpointer ftn)
{
    static MonoMethod * _mono_method = NULL;
    gpointer args[2];
    MonoObject * exc = NULL, * ret, * type;
    
    if(_mono_method == NULL)
    {    
        g_print("%s: %p\n", __FUNCTION__, mono_get_corlib());
        MonoClass * _marshal_klass = mono_class_from_name (mono_get_corlib(), 
            "System.Runtime.InteropServices", "Marshal");
        g_assert(_marshal_klass != NULL);

        MonoMethodDesc * _mdesc = mono_method_desc_new(
            ":GetDelegateForFunctionPointer", FALSE);
        _mono_method = mono_method_desc_search_in_class(_mdesc, _marshal_klass);
        mono_method_desc_free(_mdesc);
        
        g_assert(_mono_method != NULL);
    }
    
    g_assert(klass != NULL && ftn != NULL);

    type = (MonoObject*)mono_type_get_object(domain, mono_class_get_type(klass));
    g_assert(type != NULL);
    
    args[0] = &ftn;
    args[1] = type;
   
    ret = mono_runtime_invoke(_mono_method, NULL, args, &exc);    

    if(exc)
    {
        mono_unhandled_exception(exc);
        return NULL;
    }
    else
        return (MonoDelegate *)ret;
}

//I know this looks dumb..that's because it is.
#define FOO_METH_CTOR 0
#define FOO_METH_CCTOR 1
#define FOO_METH_BAR 2
#define FOO_METH_FINALIZE 3
#define FOO_METH_QUUX 4
#define FOO_METH_LAST 5
static MonoMethod * Embed_Foo_methods[FOO_METH_LAST];

static void cache_methods(MonoClass * klass, MonoMethod ** methods)
{
    int i = 0;
    gpointer iter = NULL;

    g_return_if_fail(klass != NULL);

    while ((methods[i] = mono_class_get_methods (klass, &iter)))
        i++;
}

// Calls Foo.Bar() with a delegate 

void Embed_Foo_Bar(MonoObject * obj, MonoDelegate * del)
{
    gpointer args[1];
    args[0] = del;
    MonoObject * exc = NULL;
    
    g_assert(obj != NULL && del != NULL);

    mono_runtime_invoke(Embed_Foo_methods[FOO_METH_BAR], (void *)obj, args, &exc);    
    
    if(exc)
        mono_unhandled_exception(exc);
}

// Calls Foo.Quux() with a function pointer

void Embed_Foo_Quux(MonoObject * obj, void * ftn_ptr)
{
    gpointer args[1];
    args[0] = &ftn_ptr;
    MonoObject * exc = NULL;
   
    g_assert(obj != NULL && ftn_ptr != 0);

    mono_runtime_invoke(Embed_Foo_methods[FOO_METH_QUUX], (void *)obj, args, &exc);

    if(exc)
        mono_unhandled_exception(exc);
}

static void _bar_cb(guint32 handle)
{
    g_print("_bar_cb(%d)\n", handle);
}

static void do_test(MonoDomain *domain, MonoImage *image)
{
	MonoClass *klass;
	MonoObject *obj;

	klass = mono_class_from_name (image, "Embed", "Foo");
	if (!klass) 
    {
		fprintf (stderr, "Can't find Foo in assembly %s\n", mono_image_get_filename (image));
		exit (1);
	}

    g_print("klass = %p\n", mono_class_get_type(klass));

    // My little routine to make a table of the MonoMethod* objects
    cache_methods(klass, Embed_Foo_methods);

	obj = mono_object_new (domain, klass);
	mono_runtime_object_init (obj);
    g_assert(obj != NULL);

    MonoClass * del_class = mono_class_from_name (image, "Embed", "BarHandler");
    g_assert(del_class != NULL);

    g_print("%s\n", mono_class_get_name(del_class));

    MonoDelegate * del = ftnptr_to_delegate(domain, del_class, (gpointer)_bar_cb);
    g_assert(del != NULL);

    // ** WORKS **
    Embed_Foo_Bar(obj, del);

    // ** FAILS HARD **
    Embed_Foo_Quux(obj, _bar_cb);
}

static void main_function (MonoDomain *domain, const char *file, int argc, char **argv)
{
	MonoAssembly *assembly;

	/* Loading an assembly makes the runtime setup everything
	 * needed to execute it. If we're just interested in the metadata
	 * we'd use mono_image_load (), instead and we'd get a MonoImage*.
	 */
	assembly = mono_domain_assembly_open (domain, file);
	g_assert(assembly != NULL);

    g_print("%s()\n", __FUNCTION__);

    do_test(domain, mono_assembly_get_image (assembly));
}

int main (int argc, char * argv[]) 
{
	MonoDomain *domain;
	const char *file;
	int retval;
	
	if (argc < 2)
    {
		fprintf (stderr, "Please provide an assembly to load\n");
		return 1;
	}

	file = argv [1];

	/*
	 * mono_jit_init() creates a domain: each assembly is
	 * loaded and run in a MonoDomain.
	 */
    //NOTE: Uses the assembly's reference to corlib to determine runtime version. 
    // We only support v2.0, so we use mono_jit_init_version() instead..
	//domain = mono_jit_init(/*file*/"lucid-appdomain");
    
    domain = mono_jit_init_version("clr", "v2.0.50727");
    
    g_print("%s()\n", __FUNCTION__);

	main_function (domain, file, argc - 1, argv + 1);

	retval = mono_environment_exitcode_get ();
	
	mono_jit_cleanup (domain);
	return retval;
}

