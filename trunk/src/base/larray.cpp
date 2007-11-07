#include <lucid-base.h>

extern "C"
{

LArray<> * lt_array_create(void (* val_free_fn) (void *), int elem_size);
void lt_array_destroy(LArray<> * self);
void lt_array_append(LArray<> * self, void * value);
void lt_array_insert(LArray<> * self, int index, void * value);
bool_t lt_array_remove(LArray<> * self, void * value);
bool_t lt_array_remove_index(LArray<> * self, int index);
void lt_array_reverse(LArray<> * self);
void lt_array_sort(LArray<> * self, int (* list_compare_func) (void * value1, void * value2));
void lt_array_foreach(LArray<> * self, void (* list_foreach_func) (void * value, 
                                            void * user_arg), 
                                            void * user_arg);
void * lt_array_get_item(LArray<> * self, int index);
void lt_array_set_item(LArray<> * self, int index, void * value);
void lt_array_clear(LArray<> * self);
int lt_array_count(LArray<> * self);
char * lt_array_get_data(LArray<> * self);
LIter<>  lt_array_get_iter(LArray<> * self);

}

class LArrayIter: public LAllocator
{
public:
	LArrayIter(LArrayImpl * array);
	bool_t MoveNext();
	void ** Current();
private:
	volatile int m_cur;
	LArrayImpl * m_array;
};

inline LArrayIter::LArrayIter(LArrayImpl * array)
{
	this->m_array = array;
	this->m_cur = 0;
}

inline bool_t LArrayIter::MoveNext()
{
	if(this->m_cur == this->m_array->Count() - 1)
		return FALSE;
	else
	{
		this->m_cur += 1;	
		return TRUE;
	}
}

inline void ** LArrayIter::Current()
{
	return (void **)this->m_array->GetItem(this->m_cur);
}

void	_lt_array_iter_destroy(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	delete iter;
}

bool_t LArrayImpl::MoveNext(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	return iter->MoveNext();
}

void ** LArrayImpl::Current(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	return iter->Current();
}

void * LArrayImpl::GetArrayIter()
{
	return (void *)new LArrayIter(this);
}

LArrayImpl::LArrayImpl(void (* val_free_fn) (void *), int elem_size): m_elem_size(elem_size)
{
	g_assert(elem_size > 0);
	this->m_val_free_fn = val_free_fn;
}

//FIXME- THIS SERIOUSLY NEEDS TO BE IMPLEMENTED!
bool_t LArrayImpl::Remove(void * value)
{
	g_return_val_if_fail(value != NULL, FALSE);

	for (int i = 0; i < this->Count(); i ++)
	{
		const void * p1 = (const void *)this->GetItem(i), * p2 = (const void *)value;
      		if (memcmp(p1, p2, this->m_elem_size) == 0)
	  		return this->RemoveIndex(i);
    	}
  	return FALSE;
}

void LArrayImpl::Reverse()
{
//FIXME- Test this a bit more thoroughly - and make generalized
/*	
    	void * tmp;
    	int i, j;

    	tmp = g_malloc(this->m_elem_size);
    	g_assert(tmp != NULL);

    	j = this->m_impl->len - 1;

    	for(i = 0; i < j; i++, j--)
    	{
        	memmove(tmp, LT_ARRAY_INDEX(i), this->m_elem_size); 
       		memmove(LT_ARRAY_INDEX(i), LT_ARRAY_INDEX(j), this->m_elem_size);  
        	memmove(LT_ARRAY_INDEX(j), tmp, this->m_elem_size);                
    	}    

    	g_free(tmp);
*/
}

void * LArrayImpl::Pop(int index)
{
	void * value;

	if (this->Count() == 0)
		return NULL;

	if (index < 0)
		index = this->Count() - 1;

	if (index >= this->Count())
		return NULL;

	value = this->GetItem(index);

	if(! this->RemoveIndex(index))
		return NULL;

	return value;
}

void LArrayImpl::Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
    	g_return_if_fail(list_foreach_func != NULL);
    
    	for(int i = 0, len = this->Count(); i < len; i++)
        	list_foreach_func((const void *)this->GetItem(i), user_arg);
}

void LArrayImpl::Clear()
{
	this->RemoveRange(0, this->Count());
}

LArray<> * lt_array_create(void (* val_free_fn) (void *), int elem_size)
{
	return new LArray<>(val_free_fn, elem_size);
}

void lt_array_destroy(LArray<> * self)
{
	LT_DELETE_CPP();
}

void lt_array_append(LArray<> * self, void * value)
{
	g_return_if_fail(self != NULL);
	self->GetImpl()->Append(value);
}

void lt_array_insert(LArray<> * self, int index, void * value)
{
	g_return_if_fail(self != NULL);
	self->GetImpl()->Insert(index, value);
}

bool_t lt_array_remove(LArray<> * self, void * value)
{
	LT_RET_CALL_CPP(Remove, FALSE, value);
}

bool_t lt_array_remove_index(LArray<> * self, int index)
{
	LT_RET_CALL_CPP(RemoveIndex, FALSE, index);
}

void lt_array_reverse(LArray<> * self)
{
	LT_CALL_CPP(Reverse);
}

void lt_array_sort(LArray<> * self, int (* list_compare_func) (void * value1, void * value2))
{
	LT_CALL_CPP(Sort, list_compare_func);
}

void lt_array_foreach(LArray<> * self, void (* list_foreach_func) (void * value, void * user_arg), 
	void * user_arg)
{
	LT_CALL_CPP(Foreach, list_foreach_func, user_arg);
}

void * lt_array_get_item(LArray<> * self, int index)
{
	LT_RET_CALL_CPP(GetItem, NULL, index);
}

void lt_array_set_item(LArray<> * self, int index, void * value)
{
	LT_CALL_CPP(SetItem, index, value);
}

void lt_array_clear(LArray<> * self)
{
	LT_CALL_CPP(Clear);
}

int lt_array_count(LArray<> * self)
{
	LT_RET_CALL_CPP(Count, -1);
}
 
char * lt_array_get_data(LArray<> * self)
{
	return (char *)self->GetData();
}
   
//FIXME- We need a "dummy" iterator declaration - lt_iter_dummy
LIter<>  lt_array_get_iter(LArray<> * self)
{
	LT_RET_CALL_CPP(GetIter, static_cast<LIter<> >(lt_iter_dummy()));
}

