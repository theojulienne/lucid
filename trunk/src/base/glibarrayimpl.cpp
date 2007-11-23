#include <lucid-base.h>

#define LT_ARRAY_INDEX(i) (void *)(((dynamic_cast<GLibArrayInst *>(inst)->m_array)->data) + inst->GetElemSize() * (i))

class GLibArrayInst: public LArrayInst
{
public:
	GLibArrayInst(LArrayImpl * impl, void (* val_free_fn) (void *), int elem_size);
	~GLibArrayInst();
	
	GArray * m_array;
private:
    void (* m_val_free_fn) (void *);
};

GLibArrayInst::GLibArrayInst(LArrayImpl * impl, void (* val_free_fn) (void *), int elem_size): 		LArrayInst(impl, val_free_fn, elem_size)
{
	g_assert(elem_size > 0);
	this->m_array = g_array_new(FALSE, TRUE, elem_size);
	g_assert(this->m_array != NULL);
	this->m_val_free_fn = val_free_fn;
	this->m_elem_size = elem_size;
}

GLibArrayInst::~GLibArrayInst()
{
	if(this->m_val_free_fn != NULL)
    {        
        for(int i = 0; i < (int)this->m_array->len; i++)
            this->m_val_free_fn((void *)((this->m_array->data) + this->m_elem_size * (i)));    
    }
    g_array_free(this->m_array, TRUE);    
}

class GLibArrayImpl: public LArrayImpl
{
public:
	GLibArrayImpl();
	~GLibArrayImpl();
	LArrayInst * Create(void (* val_free_fn) (void *), int elem_size);
	void Append(LArrayInst * inst, void * value);
    void Insert(LArrayInst * inst, int i, void * value);
    bool_t RemoveIndex(LArrayInst * inst, int i);
	bool_t RemoveRange(LArrayInst * inst, int i, int len);
    void Sort(LArrayInst * inst, int (* list_compare_func) (const void * value1, 	
                                            const void * value2));
    void * GetItem(LArrayInst * inst, int i);
    void SetItem(LArrayInst * inst, int i, void * value);      
    int Count(LArrayInst * inst); 
    char * GetData(LArrayInst * inst);
	
	static LArrayImpl * Get();
	static LArrayImpl * g_singleton;
};

GLibArrayImpl::GLibArrayImpl()
{
}

GLibArrayImpl::~GLibArrayImpl()
{
}

LArrayImpl * GLibArrayImpl::g_singleton = NULL;

LArrayImpl * GLibArrayImpl::Get()
{
	if(g_singleton == NULL)
		g_singleton = new GLibArrayImpl;
	return g_singleton;
}

void GLibArrayImpl::Append(LArrayInst * inst, void * value)
{
	g_return_if_fail(inst != NULL);
	g_array_append_vals(dynamic_cast<GLibArrayInst *>(inst)->m_array, value, 1);
}

void GLibArrayImpl::Insert(LArrayInst * inst, int index, void * value)
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
	g_return_if_fail(inst != NULL);
	g_array_insert_vals(dynamic_cast<GLibArrayInst *>(inst)->m_array, index, value, 1);
}

bool_t GLibArrayImpl::RemoveIndex(LArrayInst * inst, int i)
{
	g_return_val_if_fail(inst != NULL, FALSE);
	g_array_remove_index(dynamic_cast<GLibArrayInst *>(inst)->m_array, i);
	return TRUE;
}

bool_t GLibArrayImpl::RemoveRange(LArrayInst * inst, int i, int len)
{
	g_return_val_if_fail(inst != NULL, FALSE);
	g_array_remove_range(dynamic_cast<GLibArrayInst *>(inst)->m_array, i, len);
	return TRUE;
}

void GLibArrayImpl::Sort(LArrayInst * inst, int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
	g_return_if_fail(inst != NULL);
	g_return_if_fail(list_compare_func != NULL);
	g_array_sort(dynamic_cast<GLibArrayInst *>(inst)->m_array, (GCompareFunc)list_compare_func);    
}

//FIXME-confirm that my bounds checks are sane (and not useless!)
void * GLibArrayImpl::GetItem(LArrayInst * inst, int i)
{
	g_return_val_if_fail(inst != NULL, NULL);
	g_return_val_if_fail(i >= 0, NULL);
	g_return_val_if_fail(i <= (int)dynamic_cast<GLibArrayInst *>(inst)->m_array->len, NULL);
	return LT_ARRAY_INDEX(i);
}
    
//FIXME-confirm that my bounds checks are sane (and not useless!)
void GLibArrayImpl::SetItem(LArrayInst * inst, int i, void * value)
{
	g_return_if_fail(inst != NULL);
	g_return_if_fail(i >= 0);
	g_return_if_fail(i <= (int)dynamic_cast<GLibArrayInst *>(inst)->m_array->len);
	g_return_if_fail(value != NULL);
	memmove(LT_ARRAY_INDEX(i), value, inst->GetElemSize());
}

int GLibArrayImpl::Count(LArrayInst * inst)
{
	g_return_val_if_fail(inst != NULL, -1);
	return dynamic_cast<GLibArrayInst *>(inst)->m_array->len;
}

char * GLibArrayImpl::GetData(LArrayInst * inst)
{
	g_return_val_if_fail(inst != NULL, NULL);
	return dynamic_cast<GLibArrayInst *>(inst)->m_array->data;
}

LArrayInst * GLibArrayImpl::Create(void (* val_free_fn) (void *), int elem_size)
{
	return dynamic_cast<LArrayInst *>(new GLibArrayInst(this, val_free_fn, elem_size));
}

LArrayInst * LArrayImpl::CreateDefault(void (* val_free_fn) (void *), int elem_size)
{
	return GLibArrayImpl::Get()->Create(val_free_fn, elem_size);
}

