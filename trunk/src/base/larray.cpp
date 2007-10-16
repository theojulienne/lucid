#include <lucid-base.h>

extern "C"
{

LArrayImpl * lt_array_create(void (* val_free_fn) (void *), int elem_size);
void lt_array_destroy(LArrayImpl * self);
void lt_array_append(LArrayImpl * self, void * value);
void lt_array_insert(LArrayImpl * self, int index, void * value);
bool_t lt_array_remove(LArrayImpl * self, void * value);
bool_t lt_array_remove_index(LArrayImpl * self, int index);
void lt_array_reverse(LArrayImpl * self);
void lt_array_sort(LArrayImpl * self, int (* list_compare_func) (const void * value1, 
                                            const void * value2));
void lt_array_foreach(LArrayImpl * self, void (* list_foreach_func) (const void * value, 
                                            void * user_arg), 
                                            void * user_arg);
void * lt_array_get_item(LArrayImpl * self, int index);
void lt_array_set_item(LArrayImpl * self, int index, void * value);
void lt_array_clear(LArrayImpl * self);
int lt_array_count(LArrayImpl * self);
char * lt_array_get_data(LArrayImpl * self);

}

LArrayImpl::LArrayImpl(void (* val_free_fn) (void *), int elem_size): m_elem_size(elem_size)
{
	g_assert(elem_size > 0);
	
	this->m_impl = g_array_new(FALSE, TRUE, elem_size);
	g_assert(this->m_impl != NULL);

	this->m_val_free_fn = val_free_fn;
}

LArrayImpl::~LArrayImpl()
{
	if(this->m_val_free_fn != NULL)
    	{        
        	for(int i = 0; i < this->Count(); i++)
            		this->m_val_free_fn(LT_ARRAY_INDEX(i));    
    	}
    	g_array_free(this->m_impl, TRUE);    
}

void LArrayImpl::Foreach(void (* list_foreach_func) (const void * value, void * user_arg), 
                    void * user_arg)
{
    	g_return_if_fail(list_foreach_func != NULL);
    
    	for(int i = 0, len = this->m_impl->len; i < len; i++)
        	list_foreach_func((const void *)LT_ARRAY_INDEX(i), user_arg);
}

void LArrayImpl::Insert(int index, void * value)
{/*
    GPtrArray *array = (GPtrArray *)this->m_impl;
    if ((index + 1) == array->len) 
    {
        // add to the end of the array 
        g_ptr_array_add (array, value);
        return;
    }      
    if (index >= array->len)
    {
        // extend and add PAST the end of the array
        g_ptr_array_set_size (array, index + 1);
        array->pdata[index] = value;
        return;
    }       
    // normal case - shift all elements starting at @index 1 position to the right 
    g_ptr_array_set_size (array, array->len + 1);
    for (int i = array->len - 2; i >= index; i--)
        array->pdata[i + 1] = array->pdata[i];
    array->pdata[index] = value;
*/
	g_array_insert_vals(this->m_impl, index, value, 1);
}

void LArrayImpl::Reverse()
{
	//FIXME- Test this a bit more thoroughly
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

LArrayImpl * lt_array_create(void (* val_free_fn) (void *), int elem_size)
{
	LT_NEW_CPP(LArrayImpl, val_free_fn, elem_size);
}

void lt_array_destroy(LArrayImpl * self)
{
	LT_DELETE_CPP();
}

void lt_array_append(LArrayImpl * self, void * value)
{
	LT_CALL_CPP(Append, value);
}

void lt_array_insert(LArrayImpl * self, int index, void * value)
{
	LT_CALL_CPP(Insert, index, value);
}

bool_t lt_array_remove(LArrayImpl * self, void * value)
{
	LT_RET_CALL_CPP(Remove, FALSE, value);
}

bool_t lt_array_remove_index(LArrayImpl * self, int index)
{
	LT_RET_CALL_CPP(RemoveIndex, FALSE, index);
}

void lt_array_reverse(LArrayImpl * self)
{
	LT_CALL_CPP(Reverse);
}

void lt_array_sort(LArrayImpl * self, int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
	LT_CALL_CPP(Sort, list_compare_func);
}

void lt_array_foreach(LArrayImpl * self, void (* list_foreach_func) (const void * value, 
                                            void * user_arg), 
                                            void * user_arg)
{
	LT_CALL_CPP(Foreach, list_foreach_func, user_arg);
}

void * lt_array_get_item(LArrayImpl * self, int index)
{
	LT_RET_CALL_CPP(GetItem, NULL, index);
}

void lt_array_set_item(LArrayImpl * self, int index, void * value)
{
	LT_CALL_CPP(SetItem, index, value);
}

void lt_array_clear(LArrayImpl * self)
{
	LT_CALL_CPP(Clear);
}

int lt_array_count(LArrayImpl * self)
{
	LT_RET_CALL_CPP(Count, -1);
}
 
char * lt_array_get_data(LArrayImpl * self)
{
	LT_RET_CALL_CPP(GetData, NULL);
}
   
