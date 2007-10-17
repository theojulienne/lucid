#ifndef __LUCID_LARRAY_H__
#define __LUCID_LARRAY_H__

#include <string.h>
#include <glib.h>

#include <liter.h>

#define LT_ARRAY_INDEX(i) (void *)(((this->m_impl)->data) + this->m_elem_size * (i))

void	_lt_array_iter_destroy(void * self);

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
	void * Pop(int index);
    	void Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2));
    	void Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);
    	void * GetItem(int i);
    	void SetItem(int i, void * value);      
    	void Clear();
    	int Count(); 
    	char * GetData();
	LT_DEFINE_ITER_FULL(LArrayImpl, void *, _lt_array_iter_destroy, GetArrayIter());    	   
private:
	void * GetArrayIter();	
    	GArray * m_impl;
    	int m_elem_size;
    	void (* m_val_free_fn) (void *);
};


inline void LArrayImpl::Append(void * value)
{
	g_array_append_vals(this->m_impl, value, 1);
}

//FIXME- THIS SERIOUSLY NEEDS TO BE IMPLEMENTED!
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

inline char * LArrayImpl::GetData()
{
	return this->m_impl->data;
}

template <class V = void *> class LArray: public LArrayImpl
{
public:
	LArray(void (* val_free_fn) (void *));
    	void Append(V value);
    	void Insert(int index, V value);
    	bool_t Remove(V value);
    	V GetItem(int index);
	V Pop(int index);
    	void SetItem(int index, V value);
//    	V & operator[](int i); 
    	V * GetData();
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
inline void LArray<V>::Insert(int index, V value)
{
    	LArrayImpl::Insert(index, (void *)&value);
}

template <class V>
inline bool_t LArray<V>::Remove(V value)
{
    	return LArrayImpl::Remove((void *)&value);
}

template <class V>
inline V LArray<V>::GetItem(int index)
{
    	return (V) * (V *)LArrayImpl::GetItem(index);
}

template <class V>
inline V LArray<V>::Pop(int index)
{
    	return (V) * (V *)LArrayImpl::Pop(index);
}

template <class V>
inline void LArray<V>::SetItem(int index, V value)
{
    	LArrayImpl::SetItem(index, (void *)&value);
}

template <class V>
inline V * LArray<V>::GetData()
{
    	return (V *)LArrayImpl::GetData();
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
