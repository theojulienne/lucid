#include <lucid-base.h>

static void _lt_hashtable_append_key_cb(const void * key, const void * value, void * user_arg);
static void _lt_hashtable_append_value_cb(const void * key, const void * value, void * user_arg);
static void _lt_hashtable_append_pair_cb(const void * key, const void * value, void * user_arg);

typedef void hashtable_foreach_func(const void * key, const void * value, void * user_arg);

extern "C"
{

LHashtable<> * lt_hashtable_create(unsigned int (* hash_fn) (void *),
                bool_t (* key_eq_fn) (void *, void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));
LHashtable<> * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *));
void lt_hashtable_insert(LHashtable<> * self, void * key, void * value);
void * lt_hashtable_lookup(LHashtable<> * self, void * key);
bool_t lt_hashtable_remove(LHashtable<> * self, void * key);
void lt_hashtable_clear(LHashtable<> * self);
int lt_hashtable_count(LHashtable<> * self);
LArray<> * lt_hashtable_get_keys(LHashtable<> * self);
LArray<> * lt_hashtable_get_values(LHashtable<> * self);

}

static void _ghash_foreach_cb(gpointer key, gpointer value, gpointer user_data)
{
	void ** args = (void **)user_data;
    	hashtable_foreach_func * foreach_func = (hashtable_foreach_func *)args[0];

    	foreach_func(key, value, args[1]);   
}

void LHashtableImpl::Foreach(void (* foreach_func) (const void * key, const void * value, void * user_arg), void * user_arg)
{
    	void * args[2];
    	args[0] = (void *)foreach_func;
    	args[1] = user_arg;

    	g_assert(foreach_func != NULL);
    	g_hash_table_foreach((GHashTable *)this->impl, _ghash_foreach_cb, (void *)args);
}

LArray<> * LHashtableImpl::GetKeys()
{
    	LArray<> * keys = new LArray<>(NULL);
    	this->Foreach(_lt_hashtable_append_key_cb, (void *)keys);
    	return keys;   
}
 
LArray<> * LHashtableImpl::GetValues()
{
    	LArray<> * values = new LArray<>(NULL);
    	this->Foreach(_lt_hashtable_append_value_cb, (void *)values);
    	return values;  
}

LArray<> * LHashtableImpl::GetPairs()
{
    	LArray<lt_hashtable_pair> * pairs = new LArray<lt_hashtable_pair>(NULL);
    	this->Foreach(_lt_hashtable_append_pair_cb, (void *)pairs);
    	return reinterpret_cast<LArray<> *>(pairs);
}

LHashtable<> * lt_hashtable_create(unsigned int (* hash_fn) (void *),
                bool_t (* key_eq_fn) (void *, void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *))      
{
    	return new LHashtable<>(hash_fn, key_eq_fn, key_free_fn, val_free_fn);    
}

LHashtable<> * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *))
{
    	LT_NEW_CPP(LHashtable<>, owns_strings, val_free_fn);
}

void lt_hashtable_insert(LHashtable<> * self, void * key, void * value)
{
    	LT_CALL_CPP(Insert, key, value);
}

void * lt_hashtable_lookup(LHashtable<> * self, void * key)
{
    	LT_RET_CALL_CPP(Lookup, NULL, key);
}

bool_t lt_hashtable_remove(LHashtable<> * self, void * key)
{
    	LT_RET_CALL_CPP(Remove, FALSE, key);
}

void lt_hashtable_clear(LHashtable<> * self)
{
    	LT_CALL_CPP(Clear);
}

int lt_hashtable_count(LHashtable<> * self)
{
    	LT_RET_CALL_CPP(Count, -1);
}

LArray<> * lt_hashtable_get_keys(LHashtable<> * self)
{
    	LT_RET_CALL_CPP(GetKeys, NULL);
}

LArray<> * lt_hashtable_get_values(LHashtable<> * self)
{
    	LT_RET_CALL_CPP(GetValues, NULL);
}

static void _lt_hashtable_append_key_cb(const void * key, const void * value, void * user_arg)
{
    	LArray<> * array = static_cast<LArray<> *>(user_arg);
    	array->Append((void *)key);
}

static void _lt_hashtable_append_value_cb(const void * key, const void * value, void * user_arg)
{
    	LArray<> * array = static_cast<LArray<> *>(user_arg);
    	array->Append((void *)value);
}

static void _lt_hashtable_append_pair_cb(const void * key, const void * value, void * user_arg)
{
    	LArray<lt_hashtable_pair> * array = static_cast<LArray<lt_hashtable_pair> *>(user_arg);
    	lt_hashtable_pair args;
   	args.key = (void *)key;
    	args.value = (void *)value;
    	array->Append(args);
}

