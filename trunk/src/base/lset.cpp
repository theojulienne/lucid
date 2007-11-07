#include <lucid-base.h>

extern "C"
{

LSet<> * lt_set_create(unsigned int (* hash_fn) (void *),
                bool_t (* key_eq_fn) (void *, void *),
                void (* key_free_fn) (void *));
LSet<> * lt_set_str_create(bool_t owns_strings);
void lt_set_add(LSet<> * self, void * element);
bool_t lt_set_remove(LSet<> * self, void * element);
bool_t lt_set_contains(LSet<> * self, void * element);
void lt_set_clear(LSet<> * self);
int lt_set_count(LSet<> * self);
LArray<> * lt_set_get_elements(LSet<> * self);
void lt_set_foreach(LSet<> * self, void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);

}

static void _ghash_foreach_cb(gpointer key, gpointer value, gpointer user_data)
{
	void ** args = (void **)user_data;

	((void ( * )(const void *, void *))args[0])(key, args[1]);   
}

void _lt_set_foreach_impl(LSet<> * self, void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
	void * args[2];
	args[0] = (void *)set_foreach_func;
	args[1] = user_arg;

	g_assert(set_foreach_func != NULL);
	g_hash_table_foreach((GHashTable *)self->GetImpl(), _ghash_foreach_cb, (void *)args);
}

LSet<> * lt_set_create(unsigned int (* hash_fn) (void *),
                bool_t (* key_eq_fn) (void *, void *),
                void (* key_free_fn) (void *))
{
	LT_NEW_CPP(LSet<>, hash_fn, key_eq_fn, key_free_fn);
}

LSet<> * lt_set_str_create(bool_t owns_strings)
{
	LT_NEW_CPP(LSet<>, owns_strings);
}

void lt_set_add(LSet<> * self, void * element)
{
	LT_CALL_CPP(Add, element);
}

bool_t lt_set_remove(LSet<> * self, void * element)
{
	LT_RET_CALL_CPP(Remove, FALSE, element);
}

bool_t lt_set_contains(LSet<> * self, void * element)
{
	LT_RET_CALL_CPP(Contains, FALSE, element);	
}

void lt_set_clear(LSet<> * self)
{
	LT_CALL_CPP(Clear);
}

int lt_set_count(LSet<> * self)
{
	LT_RET_CALL_CPP(Count, -1);	
}

LArray<> * lt_set_get_elements(LSet<> * self)
{
	LT_RET_CALL_CPP(GetElements, NULL);	
}

void lt_set_foreach(LSet<> * self, void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
	LT_CALL_CPP(Foreach, set_foreach_func, user_arg);
}

