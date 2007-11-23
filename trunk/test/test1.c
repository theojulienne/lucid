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
        char * key = * (char **)lt_array_get_item(keys, i);
        printf("%s = %d\n", key, GPOINTER_TO_INT(lt_hashtable_lookup(hash, (void *)key)));
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
/*
    LModule * module = lt_module_load(NULL);
    bool_t (* func) ();

    g_assert(module);

    g_assert(lt_module_get_name(module));

    g_assert(!strcmp("main", lt_module_get_name(module)));

    func = lt_module_get_symbol(module, "call_me_in_module_test");

    g_assert(func != NULL);

    g_assert(func());

    lt_base_unref(LT_BASE(module));
*/
}

static void _test_foreach(const void * value, void * user_arg)
{
    g_print("%s: %f [%p]\n", __FUNCTION__, * (double *)value, user_arg);
}

static void array_test()
{
	int i, len;
   	double x;
    LArray * array = lt_array_create(NULL, sizeof(double));
	LIter it;

    x = 1;    
    lt_array_append(array, &x);
    x = 2;
    lt_array_append(array, &x);
    x = 3;    
    lt_array_append(array, &x);

  //  x = 4;
  //  lt_array_set_item(array, 0, &x);

    len = lt_array_count(array);

   	printf("%d\n", len);

	it = lt_array_get_iter(array);
	do
	{
		double * d = (double *)lt_iter_current(it);
		if(d)		
			printf("%f\n", * d);
	} while(lt_iter_move_next(it));
	lt_iter_destroy(it);

	lt_array_reverse(array);

    for(i = 0; i < len; i++)
    {
	    double * d = (double *)lt_array_get_item(array, i);
		if(d)
			printf("%f\n", * d);
	}
	
    lt_array_foreach(array, _test_foreach, (void *)0xfeedface);
   	lt_array_destroy(array);
}

static void _set_foreach_cb(const void * element, void * user_arg)
{
	printf("%s: %s\n", __FUNCTION__, (char *)element);
	g_assert(user_arg == GINT_TO_POINTER(0xdeadbeef));
}

static void set_test()
{
	LSet * set = lt_set_str_create(FALSE);
	LArray * elements;
	int i;

	lt_set_add(set, "one");
	lt_set_add(set, "two");
	g_assert(lt_set_contains(set, "one"));
       	g_assert(lt_set_count(set) == 2);
	lt_set_add(set, "one");
       	g_assert(lt_set_count(set) == 2);
	g_assert(lt_set_contains(set, "two"));

	lt_set_foreach(set, _set_foreach_cb, GINT_TO_POINTER(0xdeadbeef));

	elements = lt_set_get_elements(set);
	g_assert(lt_array_count(elements) == 2);

	for(i = 0; i < lt_array_count(elements); i++)
		printf("%s\n", (char *) * (void **)lt_array_get_item(elements, i));	
	
	lt_array_destroy(elements);

	lt_set_clear(set);
	g_assert(lt_set_count(set) == 0);
	g_assert(! lt_set_contains(set, "one"));
	g_assert(! lt_set_contains(set, "two"));
	
	lt_base_unref(LT_BASE(set));
}

static void object_test()
{
    LObject * obj;
    LEventID id1, id2;

    //g_mem_set_vtable(glib_mem_profiler_table);
    
    //obj = lt_object_create();
    //lt_type_from_name test
    
    obj = (LObject *)lt_type_create_instance(lt_type_from_name("LObject"));
    
    id1 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xc0ffee, NULL);

    g_assert(id1 > 0);

    id2 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xdeadbeef, NULL);
   
    //  g_assert(id2 > 0);

    //  g_print("%s: id1 = %d, id2 = %d\n", __FUNCTION__, id1, id2);

    g_print("%s: %d\n", __FUNCTION__, (int)lt_object_find_handler(obj, _test_foo, (void *)0xc0ffee));

    g_assert(lt_object_remove_handler(obj, id1));   
  
    g_print("%s: %d\n", __FUNCTION__, (int)lt_object_find_handler(obj, _test_foo, (void *)0xdeadbeef));

    g_assert(lt_object_remove_handler(obj, id2));
    g_assert(lt_object_remove_handler(obj, id2));

    lt_base_unref(LT_BASE(obj));

    //    lt_base_unref(LT_BASE(obj));

    //g_mem_profile ();
}

static void base64_test()
{
    const unsigned char orig[4] = { 0, 1, 2, 3 };
    
    unsigned char * bin;
    char * b64;
    int len, i;

    b64 = lt_base64_encode(orig, 4);
    g_assert(b64 != NULL);    

    bin = lt_base64_decode(b64, &len);
    g_assert(bin != NULL);

    g_assert(sizeof(orig));
    for(i = 0; i < sizeof(orig); i++)
        g_assert(bin[i] == orig[i]);    

    g_free(b64);
    g_free(bin);
}

int main(int argc, char ** argv)
{
    lt_type_init();

    array_test();
    
    hash_test();

    module_test();

    base64_test();

    set_test();

    object_test();

    return 0;
}

