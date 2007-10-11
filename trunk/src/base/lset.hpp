#ifndef __LUCID_LSET_H__
#define __LUCID_LSET_H__

#include <lhashtable.hpp>

template <class V = void *> class LSet: public LBase
{
public:
	LSet(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *));
	LSet(bool_t owns_strings);	
	void Add(V element);
	bool_t Remove(V element);
	bool_t Contains(V element);
	void Clear();
	int Count();
	LArray<V> * GetElements();
	void Foreach(void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);
	void * GetImpl();
private:
	LHashtable<V, void *> m_data;
};

void _lt_set_foreach_impl(LSet<> * self, void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg);

template <class V> 
inline LSet<V>::LSet(unsigned int (* hash_fn) (const void *),
                bool_t (* key_eq_fn) (const void *, const void *),
                void (* key_free_fn) (void *)): m_data(hash_fn, key_eq_fn, key_free_fn, NULL)
{
}

template <class V> 
inline LSet<V>::LSet(bool_t own_strings): m_data(own_strings, NULL)
{
}

template <class V> 
inline void LSet<V>::Add(V element)
{
	if(this->m_data.Lookup(element))
		return;
	this->m_data.Insert(element, GINT_TO_POINTER(TRUE));
}


template <class V> 
inline bool_t LSet<V>::Remove(V element)
{
	return this->m_data.Remove(element);
}

template <class V> 
inline bool_t LSet<V>::Contains(V element)
{
	return this->m_data.Lookup(element) == GINT_TO_POINTER(TRUE);
}

template <class V> 
inline void LSet<V>::Clear()
{
	this->m_data.Clear();
}

template <class V> 
inline int LSet<V>::Count()
{
	return this->m_data.Count();
}

template <class V>
inline LArray<V> * LSet<V>::GetElements()
{   
    return this->m_data.GetKeys();
}

template <class V>
inline void LSet<V>::Foreach(void (* set_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
	_lt_set_foreach_impl(static_cast<LSet<> *>(this), set_foreach_func, user_arg);
}

template <class V>
inline void * LSet<V>::GetImpl()
{   
    return this->m_data.GetImpl();
}

#endif

