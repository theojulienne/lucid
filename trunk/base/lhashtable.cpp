#include <lucid-base.h>

static void _lt_hashtable_append_key_cb(const void * key, const void * value, void * user_arg);
static void _lt_hashtable_append_value_cb(const void * key, const void * value, void * user_arg);
static void _lt_hashtable_append_pair_cb(const void * key, const void * value, void * user_arg);

typedef void hashtable_foreach_func(const void * key, const void * value, void * user_arg);

extern "C"
{
LHashtable<> * lt_hashtable_create(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));
LHashtable<> * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *));
void lt_hashtable_insert(LHashtable<> * self, void * key, void * value);
void * lt_hashtable_lookup(LHashtable<> * self, void * key);
bool_t lt_hashtable_remove(LHashtable<> * self, void * key);
void lt_hashtable_clear(LHashtable<> * self, void * key);
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
    LArray<> * pairs = new LArray<>(g_free);
    this->Foreach(_lt_hashtable_append_pair_cb, (void *)pairs);
    return pairs;
}

LHashtable<> * lt_hashtable_create(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *))      
{
    return new LHashtable<>(hash_fn, key_eq_fn, key_free_fn, val_free_fn);    
}

LHashtable<> * lt_hashtable_str_create(bool_t owns_strings, void (* val_free_fn) (void *))
{
    return new LHashtable<>(owns_strings, val_free_fn);
}

void lt_hashtable_insert(LHashtable<> * self, void * key, void * value)
{
    g_return_if_fail(self != NULL);
    self->Insert(key, value);
}

void * lt_hashtable_lookup(LHashtable<> * self, void * key)
{
    g_return_val_if_fail(self != NULL, NULL);
    return self->Lookup(key);
}

bool_t lt_hashtable_remove(LHashtable<> * self, void * key)
{
    g_return_val_if_fail(self != NULL, FALSE);
    return self->Remove(key);
}

void lt_hashtable_clear(LHashtable<> * self, void * key)
{
    g_return_if_fail(self != NULL);
    self->Clear();
}

int lt_hashtable_count(LHashtable<> * self)
{
    g_return_val_if_fail(self != NULL, -1);
    return self->Count();
}

LArray<> * lt_hashtable_get_keys(LHashtable<> * self)
{
    g_return_val_if_fail(self != NULL, NULL); 
    return self->GetKeys();
}

LArray<> * lt_hashtable_get_values(LHashtable<> * self)
{
    g_return_val_if_fail(self != NULL, NULL); 
    return self->GetValues();
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
    void ** args;
    LArray<> * array;
    args = (void **)g_malloc(sizeof(void *) * 2);
    array = static_cast<LArray<> *>(user_arg);
    g_assert(args);
    args[0] = (void *)key;
    args[1] = (void *)value;
    array->Append((void *)args);
}

