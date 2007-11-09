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
	LArrayIter(LArrayInst * inst);
	bool_t MoveNext();
	void ** Current();
private:
	volatile int m_cur;
	LArrayInst * m_inst;
};

inline LArrayIter::LArrayIter(LArrayInst * inst)
{
	this->m_inst = inst;
	this->m_cur = 0;
}

inline bool_t LArrayIter::MoveNext()
{
	if(this->m_cur == this->m_inst->GetImpl()->Count(this->m_inst) - 1)
		return FALSE;
	else
	{
		this->m_cur += 1;	
		return TRUE;
	}
}

inline void ** LArrayIter::Current()
{
	return (void **)this->m_inst->GetImpl()->GetItem(this->m_inst, this->m_cur);
}

void	_lt_array_iter_destroy(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	delete iter;
}

bool_t LArrayInst::MoveNext(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	return iter->MoveNext();
}

void ** LArrayInst::Current(void * self)
{
	LArrayIter * iter = static_cast<LArrayIter *>(self);
	return iter->Current();
}

void * LArrayInst::GetArrayIter()
{
	return (void *)new LArrayIter(this);
}

LArrayInst::LArrayInst(LArrayImpl * impl, void (* val_free_fn) (void *), int elem_size): 
	m_elem_size(elem_size), m_impl(impl)
{
	g_assert(impl != NULL);
}

LArrayImpl::LArrayImpl()
{
}

//FIXME- THIS SERIOUSLY NEEDS TO BE IMPLEMENTED!
bool_t LArrayImpl::Remove(LArrayInst * inst, void * value)
{
	int elem_size;

	g_return_val_if_fail(inst != NULL, FALSE);
	g_return_val_if_fail(value != NULL, FALSE);

	elem_size = inst->GetElemSize();

	for (int i = 0; i < this->Count(inst); i ++)
	{
		const void * p1 = (const void *)this->GetItem(inst, i), * p2 = (const void *)value;
      		if (memcmp(p1, p2, elem_size) == 0)
	  		return this->RemoveIndex(inst, i);
    	}
  	return FALSE;
}

//FIXME- Test this a bit more thoroughly - and make more generalized
void LArrayImpl::Reverse(LArrayInst * inst)
{
    	void * tmp;
    	int i, j;

	g_return_if_fail(inst != NULL);

    	tmp = g_malloc(inst->GetElemSize());
    	g_assert(tmp != NULL);

    	j = inst->GetImpl()->Count(inst) - 1;

    	for(i = 0; i < j; i++, j--)
    	{	
		void * i_p = inst->GetImpl()->GetItem(inst, i);
		void * j_p = inst->GetImpl()->GetItem(inst, j);
        	memmove(tmp, i_p, inst->GetElemSize()); 
       		inst->GetImpl()->SetItem(inst, i, j_p);
        	inst->GetImpl()->SetItem(inst, j, tmp);        
    	}    

    	g_free(tmp);
}

void * LArrayImpl::Pop(LArrayInst * inst, int index)
{
	void * value;
	
	g_return_val_if_fail(inst != NULL, NULL);

	if (this->Count(inst) == 0)
		return NULL;

	if (index < 0)
		index = this->Count(inst) - 1;

	if (index >= this->Count(inst))
		return NULL;

	value = this->GetItem(inst, index);

	if(! this->RemoveIndex(inst, index))
		return NULL;

	return value;
}

void LArrayImpl::Foreach(LArrayInst * inst, void (* list_foreach_func) (const void * value, 
	void * user_arg), void * user_arg)
{
	g_return_if_fail(inst != NULL);
    	g_return_if_fail(list_foreach_func != NULL);
    
    	for(int i = 0, len = this->Count(inst); i < len; i++)
        	list_foreach_func((const void *)this->GetItem(inst, i), user_arg);
}

void LArrayImpl::Clear(LArrayInst * inst)
{
	g_return_if_fail(inst != NULL);
	this->RemoveRange(inst, 0, this->Count(inst));
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
    	self->GetInst()->GetImpl()->Append(self->GetInst(), (void *)value);
}

void lt_array_insert(LArray<> * self, int index, void * value)
{
	g_return_if_fail(self != NULL);
    	self->GetInst()->GetImpl()->Insert(self->GetInst(), index, (void *)value);
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

