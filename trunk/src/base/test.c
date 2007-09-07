#include <lucid-base.h>

static void _test_foo(LObject * obj, LEvent * args, void * user_data)
{
    printf("%s(%p, %p, %p)\n", __FUNCTION__, obj, args, user_data);
}

static void hash_test()
{
    LHashtable * hash;
    LArray * keys; 
    int i, len;    

    hash = lt_hashtable_str_create(FALSE, NULL);

    lt_hashtable_insert(hash, "a", GINT_TO_POINTER(1));
    lt_hashtable_insert(hash, "b", GINT_TO_POINTER(2));
    lt_hashtable_insert(hash, "c", GINT_TO_POINTER(3));
    lt_hashtable_insert(hash, "d", GINT_TO_POINTER(4));
    lt_hashtable_insert(hash, "e", GINT_TO_POINTER(5));
    lt_hashtable_insert(hash, "f", GINT_TO_POINTER(6));
    lt_hashtable_insert(hash, "g", GINT_TO_POINTER(7));
    lt_hashtable_insert(hash, "h", GINT_TO_POINTER(8));    

    keys = lt_hashtable_get_keys(hash);
    len = lt_array_count(keys);

    lt_array_sort(keys, (lt_array_compare_func *)g_ascii_strcasecmp);
    lt_array_reverse(keys);

    for(i = 0; i < len; i++)
    {
        // THIS LINE WILL COST YOU HOURS. BE CAREFUL.
        char * key = (char *) * (void **)lt_array_get_item(keys, i);
        printf("%s = %d\n", key, lt_hashtable_lookup(hash, (void *)key));
    }
    
    lt_array_destroy(keys);
    lt_base_unref(LT_BASE(hash));
}

bool_t call_me_in_module_test()
{
    return TRUE;
}

static void module_test()
{
    LModule * module = lt_module_load(NULL);
    bool_t (* func) ();

    g_assert(module);

    g_assert(lt_module_get_name(module));

    g_assert(!strcmp("main", lt_module_get_name(module)));

    func = lt_module_get_symbol(module, "call_me_in_module_test");

    g_assert(func != NULL);

    g_assert(func());

    lt_base_unref(LT_BASE(module));
}

static void array_test()
{
    int i, len;
    double x;
    LArray * array = lt_array_create(NULL, sizeof(double));
    x = 1;    
    lt_array_append(array, &x);
    x = 2;
    lt_array_append(array, &x);
    x = 3;    
    lt_array_append(array, &x);

  //  x = 4;
  //  lt_array_set_item(array, 0, &x);

    lt_array_reverse(array);

    len = lt_array_count(array);

    printf("%d\n", len);

    for(i = 0; i < len; i++)
        printf("%f\n", * (double *)lt_array_get_item(array, i));

    lt_array_destroy(array);
}

int main(int argc, char ** argv)
{
    LObject * obj;
    LEventID id1, id2;

    array_test();

    //g_mem_set_vtable(glib_mem_profiler_table);
    
    lt_type_init();

    hash_test();

    module_test();

    obj = lt_object_create();
    
    id1 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xc0ffee, NULL);

    g_assert(id1 > 0);

    id2 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xdeadbeef, NULL);
   
  //  g_assert(id2 > 0);

  //  g_print("%s: id1 = %d, id2 = %d\n", __FUNCTION__, id1, id2);

    g_print("%s: %d\n", __FUNCTION__, lt_object_find_handler(obj, _test_foo, (void *)0xc0ffee));

    g_assert(lt_object_remove_handler(obj, id1));   
  
    g_print("%s: %d\n", __FUNCTION__, lt_object_find_handler(obj, _test_foo, (void *)0xdeadbeef));

    g_assert(lt_object_remove_handler(obj, id2));

    lt_base_unref(LT_BASE(obj));

//    obj = (LObject *)lt_type_create_instance(lt_type_from_name("LObject"));

//    lt_base_unref(LT_BASE(obj));

    //g_mem_profile ();

    return 0;
}

