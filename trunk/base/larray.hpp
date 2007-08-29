#ifndef __LUCID_LARRAY_H__
#define __LUCID_LARRAY_H__

#include <glib.h>

class LArrayImpl
{
public:
    LArrayImpl(void (* val_free_fn) (void *));
    ~LArrayImpl();
    void Append(void * value);
    void Insert(int i, void * value);
    bool_t Remove(void * value);
    bool_t RemoveIndex(int i);
    void Reverse();
    void Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2));
    void Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);
    void * GetItem(int i);
    void SetItem(int i, void * value);      
    void Clear();
    int Count();    
private:
    void * impl;
    void (* val_free_fn) (void *);
};


inline void LArrayImpl::Append(void * value)
{
    g_ptr_array_add((GPtrArray *)this->impl, value);
}

inline bool_t LArrayImpl::Remove(void * value)
{
    return (bool_t)g_ptr_array_remove((GPtrArray *)this->impl, value);
}

inline bool_t LArrayImpl::RemoveIndex(int i)
{
    return (bool_t)g_ptr_array_remove_index((GPtrArray *)this->impl, i); 
}

inline void LArrayImpl::Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
    g_ptr_array_sort((GPtrArray *)this->impl, (GCompareFunc)list_compare_func);    
}

inline void LArrayImpl::Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
    g_ptr_array_foreach((GPtrArray *)this->impl, (GFunc)list_foreach_func, user_arg);
}
    
inline void * LArrayImpl::GetItem(int i)
{
    return g_ptr_array_index((GPtrArray *)this->impl, i);
}

inline void LArrayImpl::SetItem(int i, void * value)
{
    g_ptr_array_index((GPtrArray *)this->impl, i) = value;
}

inline void LArrayImpl::Clear()
{
    g_ptr_array_remove_range((GPtrArray *)this->impl, 0, ((GPtrArray *)this->impl)->len);
}

inline int LArrayImpl::Count()
{
    return ((GPtrArray *)this->impl)->len;
}


template <class V = void *> class LArray: public LArrayImpl
{
public:
    LArray(void (* val_free_fn) (void *));
    void Append(V value);
    void Insert(int i, V value);
    bool_t Remove(V value);
    V GetItem(int i);
    void SetItem(int i, V value);
//    V & operator[](int i);
};


template <class V> 
inline LArray<V>::LArray(void (* val_free_fn) (void *)): LArrayImpl(val_free_fn)
{
}

template <class V>
inline void LArray<V>::Append(V value)
{
    LArrayImpl::Append((void *)value);
}

template <class V>
inline void LArray<V>::Insert(int i, V value)
{
    LArrayImpl::Insert(i, (void *)value);
}

template <class V>
inline bool_t LArray<V>::Remove(V value)
{
    return LArrayImpl::Remove((void *)value);
}

template <class V>
inline V LArray<V>::GetItem(int i)
{
    return (V)LArrayImpl::GetItem(i);
}

template <class V>
inline void LArray<V>::SetItem(int i, V value)
{
    LArrayImpl::SetItem(i, (void *)value);
}

/*
// how the hell do you do this properly?
template <class V>
inline V & LArray<V>::operator[](int i)
{
    return this->GetItem(i);
}
*/

#endif
