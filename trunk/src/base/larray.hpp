#ifndef __LUCID_LARRAY_H__
#define __LUCID_LARRAY_H__

#include <glib.h>
#include <string.h>

#define LT_ARRAY_INDEX(i) (void *)(((this->m_impl)->data) + this->m_elem_size * (i))

class LArrayImpl
{
public:
    LArrayImpl(void (* val_free_fn) (void *), int elem_size);
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
    GArray * m_impl;
    int m_elem_size;
    void (* m_val_free_fn) (void *);
};


inline void LArrayImpl::Append(void * value)
{
    g_array_append_vals(this->m_impl, value, 1);
}

//FIXME
inline bool_t LArrayImpl::Remove(void * value)
{
    return TRUE;//g_ptr_array_remove((GPtrArray *)this->m_impl, value);
}

inline bool_t LArrayImpl::RemoveIndex(int i)
{
    g_array_remove_index(this->m_impl, i);
    return TRUE;
}

inline void LArrayImpl::Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
    g_array_sort(this->m_impl, (GCompareFunc)list_compare_func);    
}

inline void * LArrayImpl::GetItem(int i)
{
    return LT_ARRAY_INDEX(i);
}
    
inline void LArrayImpl::SetItem(int i, void * value)
{
    memmove(LT_ARRAY_INDEX(i), value, this->m_elem_size);
}

inline void LArrayImpl::Clear()
{
    g_array_remove_range(this->m_impl, 0, this->m_impl->len);
}

inline int LArrayImpl::Count()
{
    return this->m_impl->len;
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
inline LArray<V>::LArray(void (* val_free_fn) (void *)): LArrayImpl(val_free_fn, sizeof(V))
{
}

template <class V>
inline void LArray<V>::Append(V value)
{
    LArrayImpl::Append((void *)&value);
}

template <class V>
inline void LArray<V>::Insert(int i, V value)
{
    LArrayImpl::Insert(i, (void *)&value);
}

template <class V>
inline bool_t LArray<V>::Remove(V value)
{
    return LArrayImpl::Remove((void *)&value);
}

template <class V>
inline V LArray<V>::GetItem(int i)
{
    return (V) * (V *)LArrayImpl::GetItem(i);
}

template <class V>
inline void LArray<V>::SetItem(int i, V value)
{
    LArrayImpl::SetItem(i, (void *)&value);
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
