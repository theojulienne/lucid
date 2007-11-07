#include <lucid-base.h>

#define LT_ARRAY_INDEX(i) (void *)(((this->m_impl)->data) + this->m_elem_size * (i))

class GLibArrayImpl: public LArrayImpl
{
public:
	GLibArrayImpl(void (* val_free_fn) (void *), int elem_size);
	~GLibArrayImpl();
	void Append(void * value);
    	void Insert(int i, void * value);
    	bool_t RemoveIndex(int i);
	bool_t RemoveRange(int i, int len);
    	void Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2));
    	void * GetItem(int i);
    	void SetItem(int i, void * value);      
    	int Count(); 
    	char * GetData();
private:
	GArray * m_impl;
};

GLibArrayImpl::GLibArrayImpl(void (* val_free_fn) (void *), int elem_size): 
	LArrayImpl(val_free_fn, elem_size)
{
	this->m_impl = g_array_new(FALSE, TRUE, elem_size);
	g_assert(this->m_impl != NULL);
}

GLibArrayImpl::~GLibArrayImpl()
{
	if(this->m_val_free_fn != NULL)
    	{        
        	for(int i = 0; i < this->Count(); i++)
            		this->m_val_free_fn(LT_ARRAY_INDEX(i));    
    	}
    	g_array_free(this->m_impl, TRUE);    
}

void GLibArrayImpl::Append(void * value)
{
	g_array_append_vals(this->m_impl, value, 1);
}

void GLibArrayImpl::Insert(int index, void * value)
{
//TODO- Should we use this slightly more sane implementation?
/*
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

bool_t GLibArrayImpl::RemoveIndex(int i)
{
	g_array_remove_index(this->m_impl, i);
	return TRUE;
}

bool_t GLibArrayImpl::RemoveRange(int i, int len)
{
	g_array_remove_range(this->m_impl, i, len);
	return TRUE;
}

void GLibArrayImpl::Sort(int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
	g_return_if_fail(list_compare_func != NULL);
	g_array_sort(this->m_impl, (GCompareFunc)list_compare_func);    
}

//FIXME-confirm that my bounds checks are sane (and not useless!)
void * GLibArrayImpl::GetItem(int i)
{
	g_return_val_if_fail(i >= 0, NULL);
	g_return_val_if_fail(i <= (int)this->m_impl->len, NULL);
	return LT_ARRAY_INDEX(i);
}
    
//FIXME-confirm that my bounds checks are sane (and not useless!)
void GLibArrayImpl::SetItem(int i, void * value)
{
	g_return_if_fail(i >= 0);
	g_return_if_fail(i <= (int)this->m_impl->len);
	g_return_if_fail(value != NULL);
	memmove(LT_ARRAY_INDEX(i), value, this->m_elem_size);
}

int GLibArrayImpl::Count()
{
	return this->m_impl->len;
}

char * GLibArrayImpl::GetData()
{
	return this->m_impl->data;
}

LArrayImpl * LArrayImpl::CreateDefault(void (* val_free_fn) (void *), int elem_size)
{
	GLibArrayImpl * impl = new GLibArrayImpl(val_free_fn, elem_size);
	return dynamic_cast<LArrayImpl *>(impl);	
}

