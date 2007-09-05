#include <lucid-base.h>

extern "C"
{
LArray<> * lt_array_create(void (* val_free_fn) (void *));
void lt_array_destroy(LArray<> * self);
void lt_array_append(LArray<> * self, void * value);
void lt_array_insert(LArray<> * self, int index, void * value);
bool_t lt_array_remove(LArray<> * self, void * value);
bool_t lt_array_remove_index(LArray<> * self, int index);
void lt_array_reverse(LArray<> * self);
void lt_array_sort(LArray<> * self, int (* list_compare_func) (const void * value1, 
                                            const void * value2));
void lt_array_foreach(LArray<> * self, void (* list_foreach_func) (const void * value, 
                                            void * user_arg), 
                                            void * user_arg);
void * lt_array_get_item(LArray<> * self, int index);
void lt_array_set_item(LArray<> * self, int index, void * value);
void lt_array_clear(LArray<> * self);
int lt_array_count(LArray<> * self);
}

LArrayImpl::LArrayImpl(void (* val_free_fn) (void *))
{
    this->impl = (void *)g_ptr_array_new();
    g_assert(this->impl != NULL);
    this->val_free_fn = val_free_fn;
}

LArrayImpl::~LArrayImpl()
{
    GPtrArray * ptr_array = (GPtrArray *)this->impl;
    if(this->val_free_fn)
    {        
        for(int i = 0; i < ptr_array->len; i++)
            this->val_free_fn(g_ptr_array_index(ptr_array, i));    
    }
    g_ptr_array_free(ptr_array, TRUE);    
}

void LArrayImpl::Insert(int index, void * value)
{
    GPtrArray *array = (GPtrArray *)this->impl;
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
}

void LArrayImpl::Reverse()
{
    GPtrArray * array = (GPtrArray *)this->impl;
    gpointer tmp;
    int i, j;

    j = array->len - 1;

    for(i = 0; i < j; i++, j--)
    {
        tmp = array->pdata[i];
        array->pdata[i] = array->pdata[j];
        array->pdata[j] = tmp;
    }    
}

LArray<> * lt_array_create(void (* val_free_fn) (void *))
{
    return new LArray<>(val_free_fn);
}

void lt_array_destroy(LArray<> * self)
{
    g_return_if_fail(self != NULL);
    delete self;
}

void lt_array_append(LArray<> * self, void * value)
{
    g_return_if_fail(self != NULL);
    self->Append(value);
}

void lt_array_insert(LArray<> * self, int index, void * value)
{
    g_return_if_fail(self != NULL);
    self->Insert(index, value);
}

bool_t lt_array_remove(LArray<> * self, void * value)
{
    g_return_val_if_fail(self != NULL, FALSE);
    return self->Remove(value);
}

bool_t lt_array_remove_index(LArray<> * self, int index)
{
    g_return_val_if_fail(self != NULL, FALSE);
    return self->RemoveIndex(index);
}

void lt_array_reverse(LArray<> * self)
{
    g_return_if_fail(self != NULL);
    return self->Reverse();
}

void lt_array_sort(LArray<> * self, int (* list_compare_func) (const void * value1, 
                                            const void * value2))
{
    g_return_if_fail(self != NULL);
    self->Sort(list_compare_func);
}

void lt_array_foreach(LArray<> * self, void (* list_foreach_func) (const void * value, 
                                            void * user_arg), 
                                            void * user_arg)
{
    g_return_if_fail(self != NULL);
    self->Foreach(list_foreach_func, user_arg);
}

void * lt_array_get_item(LArray<> * self, int index)
{
    g_return_val_if_fail(self != NULL, NULL);
    return self->GetItem(index);
}

void lt_array_set_item(LArray<> * self, int index, void * value)
{
    g_return_if_fail(self != NULL);
    self->SetItem(index, value);
}

void lt_array_clear(LArray<> * self)
{
    g_return_if_fail(self != NULL);
    self->Clear();
}

int lt_array_count(LArray<> * self)
{
    g_return_val_if_fail(self != NULL, -1);
    return self->Count();
}
    
