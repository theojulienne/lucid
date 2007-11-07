#ifndef __LUCID_LARRAY_H__
#define __LUCID_LARRAY_H__

#include <string.h>
#include <glib.h>

#include <liter.h>

void	_lt_array_iter_destroy(void * self);

class LArrayImpl
{
public:
	LArrayImpl(void (* val_free_fn) (void *), int elem_size);
	virtual ~LArrayImpl() {}
	virtual void Append(void * value) = 0;
    	virtual void Insert(int i, void * value) = 0;
    	bool_t Remove(void * value);
    	virtual bool_t RemoveIndex(int i) = 0;
	virtual bool_t RemoveRange(int i, int len) = 0;
    	void Reverse();
	void * Pop(int index);
    	virtual void Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2)) = 0;
    	void Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);
    	virtual void * GetItem(int i) = 0;
    	virtual void SetItem(int i, void * value) = 0;      
    	void Clear();
    	virtual int Count() = 0; 
    	virtual char * GetData() = 0;
	LT_DEFINE_ITER_FULL(LArrayImpl, void *, _lt_array_iter_destroy, GetArrayIter());    
	static LArrayImpl * CreateDefault(
		void (* val_free_fn) (void *), int elem_size);	   
protected:
	void * GetArrayIter();	
    	int m_elem_size;
    	void (* m_val_free_fn) (void *);
};

template <class V = void *> class LArray: public LAllocator
{
public:
	LArray(void (* val_free_fn) (V), int elem_size = sizeof(V));
	~LArray();
    	void Append(V value);
    	void Insert(int index, V value);
    	bool_t Remove(V value);
	bool_t RemoveIndex(int index);
	bool_t RemoveRange(int index, int length);
    	V * GetItem(int index);
	V * Pop(int index);
	void Sort(int (* list_compare_func) (V value1, V value2));
	void Foreach(void (* list_foreach_func) (V value, void * user_arg), 
                    void * user_arg);
    	void SetItem(int index, V value);
	void Reverse();
	void Clear();
    	int Count();
//    	V & operator[](int i); 
    	V * GetData();
	LArrayImpl * GetImpl();
	LIter<V> GetIter();
private:
	LArrayImpl * m_impl;
};

template <class V> 
inline LArray<V>::LArray(void (* val_free_fn) (V), int elem_size)
{
	this->m_impl = LArrayImpl::CreateDefault((void (*) (void *))val_free_fn, elem_size);
}

template <class V> 
inline LArray<V>::~LArray()
{
	delete this->m_impl;
}

template <class V>
inline LArrayImpl * LArray<V>::GetImpl()
{
    	return this->m_impl;
}

template <class V>
inline void LArray<V>::Append(V value)
{
    	this->m_impl->Append((void *)&value);
}

template <class V>
inline void LArray<V>::Insert(int index, V value)
{
    	this->m_impl->Insert(index, (void *)&value);
}

template <class V>
inline bool_t LArray<V>::Remove(V value)
{
    	return this->m_impl->Remove((void *)&value);
}

template <class V>
inline bool_t LArray<V>::RemoveIndex(int index)
{
    	return this->m_impl->RemoveIndex(index);
}

template <class V>
inline bool_t LArray<V>::RemoveRange(int index, int length)
{
    	return this->m_impl->RemoveRange(index, length);
}

template <class V>
inline V * LArray<V>::GetItem(int index)
{
    	return (V *)this->m_impl->GetItem(index);
}

template <class V>
inline V * LArray<V>::Pop(int index)
{
    	return (V *)this->m_impl->Pop(index);
}

template <class V>
inline void LArray<V>::Sort(int (* list_compare_func) (V value1, V value2))
{
	/*int (* real_func) (const void *, const void *) = list_compare_func;
	this->m_impl->Sort(real_func);*/
}

template <class V>
inline void LArray<V>::Foreach(void (* list_foreach_func) (V value, void * user_arg), 
                    void * user_arg)
{
	//this->m_impl->Sort((void (*) (const void *, void *))list_foreach_func, user_arg);
}

template <class V>
inline void LArray<V>::SetItem(int index, V value)
{
    	this->m_impl->SetItem(index, (void *)&value);
}

template <class V>
inline V * LArray<V>::GetData()
{
    	return (V *)this->m_impl->GetData();
}

template <class V>
inline void LArray<V>::Reverse()
{
	this->m_impl->Reverse();
}

template <class V>
inline void LArray<V>::Clear()
{
	this->m_impl->Clear();
}

template <class V>
inline int LArray<V>::Count()
{
	return this->m_impl->Count();
}

template <class V>
inline LIter<V> LArray<V>::GetIter()
{
	return static_cast<LIter<V> >(this->m_impl->GetIter());
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
