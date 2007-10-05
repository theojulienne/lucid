#ifndef __LUCID_LHASHTABLE_H__
#define __LUCID_LHASHTABLE_H__

#include <lbase.hpp>

//THIS IS ALL POINTER-BASED..BUT YOU CAN'T RESTRICT A C++ TEMPLTE TO A REFERENCE TYPE

class LHashtableImpl: public LBase
{
public:
    LHashtableImpl(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));       
    LHashtableImpl(bool_t owns_strings, void (* val_free_fn) (void *));
    ~LHashtableImpl();    
    void Foreach(void (* hash_foreach_func) (const void * key, const void * value, void * user_arg), 
                    void * user_arg);
    void Insert(void * key, void * value);
    void * Lookup(const void * key);
    bool_t LookupExtended(const void * key, void ** orig_key, void ** value);
    bool_t Remove(const void * key);    
    void Clear();
    int Count();
    LArray<> * GetKeys();
    LArray<> * GetValues();
    LArray<> * GetPairs();
    void * GetImpl();
private:
    void * impl;    
};

inline LHashtableImpl::LHashtableImpl(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *))
{
    this->impl = (void *)g_hash_table_new_full(
        hash_fn ? (GHashFunc)hash_fn : g_direct_hash, 
        key_eq_fn ? (GEqualFunc)key_eq_fn : g_direct_equal, 
        (GDestroyNotify)key_free_fn, 
        (GDestroyNotify)val_free_fn);
    
    g_assert(this->impl != NULL);        
}

inline LHashtableImpl::LHashtableImpl(bool_t owns_strings, void (* val_free_fn) (void *)) 
{    
    this->impl = (void *)g_hash_table_new_full(g_str_hash, g_str_equal, 
                                owns_strings ? g_free : NULL, val_free_fn);
    g_assert(this->impl != NULL); 
}

inline LHashtableImpl::~LHashtableImpl()
{
    g_hash_table_destroy((GHashTable *)this->impl);
}

inline void LHashtableImpl::Insert(void * key, void * value)
{
    g_hash_table_insert((GHashTable *)this->impl, 
                                (void *)key, 
                                (void *)value);
}

inline void * LHashtableImpl::Lookup(const void * key)
{
    return g_hash_table_lookup((GHashTable *)this->impl, key);
}

inline bool_t LHashtableImpl::LookupExtended(const void * key, void ** orig_key, void ** value)
{
    return (bool_t)g_hash_table_lookup_extended((GHashTable *)this->impl, key, orig_key, value);
}

inline bool_t LHashtableImpl::Remove(const void * key)
{
    return (bool_t)g_hash_table_remove((GHashTable *)this->impl, key);
}

inline void LHashtableImpl::Clear()
{
    g_hash_table_remove_all((GHashTable *)this->impl);
}

inline int LHashtableImpl::Count()
{
    return (int)g_hash_table_size((GHashTable *)this->impl);
}

inline void * LHashtableImpl::GetImpl()
{
    return this->impl;
}

template <class K = void *, class V = void *> class LHashtableEntry
{
public:
    K Key;
    V Value;    
};

template <class K = void *, class V = void *> class LHashtable: public LHashtableImpl
{
public:
    LHashtable(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *));       
    LHashtable(bool_t owns_strings, void (* val_free_fn) (void *)); 
    void Insert(K key, V value);
    V Lookup(const K key);
    bool_t LookupExtended(const K key, K * orig_key, V * value);
    bool_t Remove(const K key);
    LArray<K> * GetKeys();
    LArray<V> * GetValues();    
    LArray< LHashtableEntry<K, V> > * GetPairs();
};

template <class K, class V>
inline LHashtable<K, V>::LHashtable(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *), 
                void (* val_free_fn) (void *)): LHashtableImpl(hash_fn, key_eq_fn, 
                                                    key_free_fn, val_free_fn)
{
}

template <class K, class V>
inline LHashtable<K, V>::LHashtable(bool_t owns_strings, void (* val_free_fn) (void *)):
    LHashtableImpl(owns_strings, val_free_fn) 
{
}


template <class K, class V> 
inline void LHashtable<K, V>::Insert(K key, V value)
{
    LHashtableImpl::Insert((void *)key, (void *)value);
}

template <class K, class V> 
inline V LHashtable<K, V>::Lookup(const K key)
{
    return (V)LHashtableImpl::Lookup(key);
}

template <class K, class V> 
inline bool_t LHashtable<K, V>::LookupExtended(const K key, K * orig_key, V * value)
{
    return LHashtableImpl::LookupExtended(key, (void **)orig_key, (void **)value);
}

template <class K, class V> 
inline bool_t LHashtable<K, V>::Remove(const K key)
{
    return LHashtableImpl::Remove(key);
}

template <class K, class V>
inline LArray<K> * LHashtable<K, V>::GetKeys()
{   
    return reinterpret_cast<LArray<K> *>(LHashtableImpl::GetKeys());    
}

template <class K, class V>
inline LArray<V> * LHashtable<K, V>::GetValues()
{   
    return reinterpret_cast<LArray<V> *>(LHashtableImpl::GetValues());    
}

template <class K, class V>
inline LArray< LHashtableEntry<K, V> > * LHashtable<K, V>::GetPairs()
{
    return reinterpret_cast<LArray<LHashtableEntry<K, V> > *>(LHashtableImpl::GetPairs());    
}

#endif

