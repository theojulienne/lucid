#ifndef __LUCID_LARRAY_H__
#define __LUCID_LARRAY_H__

#include <string.h>
#include <glib.h>

#include <liter.h>

void	_lt_array_iter_destroy(void * self);

class LArrayImpl;

class LArrayInst: public LAllocator
{
public:
	LArrayInst(LArrayImpl * impl, void (* val_free_fn) (void *), int elem_size);
	virtual ~LArrayInst() {}
	LArrayImpl * GetImpl();
	int GetElemSize();
	LT_DEFINE_ITER_FULL(LArrayInst, void *, _lt_array_iter_destroy, GetArrayIter());     
protected:
	int m_elem_size;
	LArrayImpl * m_impl;
	void * GetArrayIter();	
};

inline LArrayImpl * LArrayInst::GetImpl()
{
	return this->m_impl;
}

inline int LArrayInst::GetElemSize()
{
	return this->m_elem_size;
}

class LArrayImpl: public LAllocator
{
public:
	LArrayImpl();
	virtual ~LArrayImpl() {}
	virtual LArrayInst * Create(void (* val_free_fn) (void *), int elem_size) = 0;
	virtual void Append(LArrayInst * inst, void * value) = 0;
    virtual void Insert(LArrayInst * inst, int i, void * value) = 0;
    bool_t Remove(LArrayInst * inst, void * value);
    virtual bool_t RemoveIndex(LArrayInst * inst, int i) = 0;
	virtual bool_t RemoveRange(LArrayInst * inst, int i, int len) = 0;
    void Reverse(LArrayInst * inst);
	void * Pop(LArrayInst * inst, int index);
    virtual void Sort(LArrayInst * inst, int (* list_compare_func) (const void * value1, 
                                            const void * value2)) = 0;
    virtual void * GetItem(LArrayInst * inst, int i) = 0;
    virtual void SetItem(LArrayInst * inst, int i, void * value) = 0;      
    void Clear(LArrayInst * inst);
    virtual int Count(LArrayInst * inst) = 0; 
    virtual char * GetData(LArrayInst * inst) = 0;

	static LArrayInst * CreateDefault(void (* val_free_fn) (void *), int elem_size);
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
	void Foreach(void (* foreach_func) (V * value, void * user_arg), 
                    void * user_arg);
    void SetItem(int index, V value);
	void Reverse();
	void Clear();
    int Count();
//  V & operator[](int i); 
    V * GetData();
	LArrayInst * GetInst();
	LIter<V> GetIter();
private:
	LArrayInst * m_inst;
};

template <class V> 
inline LArray<V>::LArray(void (* val_free_fn) (V), int elem_size)
{
	this->m_inst = LArrayImpl::CreateDefault((void (*) (void *))val_free_fn, elem_size);
}

template <class V> 
inline LArray<V>::~LArray()
{
	delete this->m_inst;
}

template <class V>
inline LArrayInst * LArray<V>::GetInst()
{
    return this->m_inst;
}

template <class V>
inline void LArray<V>::Append(V value)
{
    this->m_inst->GetImpl()->Append(this->m_inst, (void *)&value);
}

template <class V>
inline void LArray<V>::Insert(int index, V value)
{
    this->m_inst->GetImpl()->Insert(this->m_inst, index, (void *)&value);
}

template <class V>
inline bool_t LArray<V>::Remove(V value)
{
    return this->m_inst->GetImpl()->Remove(this->m_inst, (void *)&value);
}

template <class V>
inline bool_t LArray<V>::RemoveIndex(int index)
{
    return this->m_inst->GetImpl()->RemoveIndex(this->m_inst, index);
}

template <class V>
inline bool_t LArray<V>::RemoveRange(int index, int length)
{
    return this->m_inst->GetImpl()->RemoveRange(this->m_inst, index, length);
}

template <class V>
inline V * LArray<V>::GetItem(int index)
{
    return (V *)this->m_inst->GetImpl()->GetItem(this->m_inst, index);
}

template <class V>
inline V * LArray<V>::Pop(int index)
{
    return (V *)this->m_inst->GetImpl()->Pop(this->m_inst, index);
}

template <class V>
inline void LArray<V>::Sort(int (* compare_func) (V value1, V value2))
{
	int (* real_func) (const void *, const void *) = (int (*) (const void *, const void *))compare_func;
	this->m_inst->GetImpl()->Sort(this->m_inst, real_func);
}

template <class V>
void LArray<V>::Foreach(void (* foreach_func) (V * value, void * user_arg), 
                    void * user_arg)
{
    g_return_if_fail(foreach_func != NULL);
    
    for(int i = 0; i < this->Count(); i++)
        foreach_func(this->GetItem(i), user_arg);        
}

template <class V>
inline void LArray<V>::SetItem(int index, V value)
{
    this->m_inst->GetImpl()->SetItem(this->m_inst, index, (void *)&value);
}

template <class V>
inline V * LArray<V>::GetData()
{
    return (V *)this->m_inst->GetImpl()->GetData(this->m_inst);
}

template <class V>
inline void LArray<V>::Reverse()
{
	this->m_inst->GetImpl()->Reverse(this->m_inst);
}

template <class V>
inline void LArray<V>::Clear()
{
	this->m_inst->GetImpl()->Clear(this->m_inst);
}

template <class V>
inline int LArray<V>::Count()
{
	return this->m_inst->GetImpl()->Count(this->m_inst);
}


template <class V>
inline LIter<V> LArray<V>::GetIter()
{
	return static_cast<LIter<V> >(this->m_inst->GetIter());
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
