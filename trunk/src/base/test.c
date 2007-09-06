#include <lucid-base.h>

static void _test_foo(LObject * obj, LEvent * args, void * user_data)
{
    printf("%s(%p, %p, %p)\n", __FUNCTION__, obj, args, user_data);
}

int main(int argc, char ** argv)
{
    LObject * obj;
    LHashtable * hash;
    LArray * keys; 
    int i, len;    
    LEventID id1, id2;

    //g_mem_set_vtable(glib_mem_profiler_table);
    
    lt_type_init();

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
        void * key = lt_array_get_item(keys, i);
        printf("%s = %d\n", key, lt_hashtable_lookup(hash, key));
    }
    
    lt_array_destroy(keys);
    lt_base_unref(LT_BASE(hash));

    obj = lt_object_create();
    
    id1 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xc0ffee, NULL);

    g_assert(id1 > 0);

 //   id2 = lt_object_add_handler(obj, "foo", _test_foo, (void *)0xc0ffee, NULL);
   
  //  g_assert(id2 > 0);

  //  g_print("%s: id1 = %d, id2 = %d\n", __FUNCTION__, id1, id2);

      g_print("%s: %d\n", __FUNCTION__, lt_object_find_handler(obj, _test_foo, (void *)0xc0ffee));

      g_assert(lt_object_remove_handler(obj, id1));   
  
      g_print("%s: %d\n", __FUNCTION__, lt_object_find_handler(obj, _test_foo, (void *)0xc0ffee));

 //   g_assert(lt_object_remove_handler(obj, id2));

    lt_base_unref(LT_BASE(obj));

/*    obj = (LObject *)lt_type_create_instance(lt_type_from_name("LObject"));

    lt_base_unref(LT_BASE(obj));
*/
    //g_mem_profile ();

    return 0;
}

