#include <stdarg.h>

#include <lucid-base.h>

#define LT_CHECK_TYPE(type) G_STMT_START { \
    g_assert(type != NULL); \
    g_assert(type->m_klass_name != NULL); \
    g_assert(type->m_create_func != NULL); \
    } \
    G_STMT_END

extern "C" 
{
void lt_type_init();
void * lt_type_create_instance(LType * type);
LType * lt_type_from_name(const char * klass_name);
}

static LHashtable<const char *, LType *> * _lt_type_table = NULL;

void lt_type_init()
{
    g_assert(_lt_type_table == NULL);
    _lt_type_table = new LHashtable<const char *, LType *>(FALSE, NULL);
}

LType * LType::Register(const char * klass_name, 
        LType * super_klass, lt_type_create_func * create_func)
{
    LType * type;

    g_assert(klass_name != NULL);
    g_assert(create_func != NULL);
    g_assert(_lt_type_table != NULL);
    
    type = new LType();
    type->m_klass_name = klass_name;
    type->m_super_klass = super_klass;
    type->m_create_func = create_func;
    type->m_events = new LArray<char *>(g_free);

    _lt_type_table->Insert(klass_name, type);

    return type;       
}

// ADD A CHECK FOR A NAME BEING ADDED TWICE
bool_t LType::AddEvents(char * first_event, ...)
{
    va_list argp;
    char * event;

    va_start(argp, first_event);

    event = first_event;

    while(event != NULL)
    {
        this->m_events->Append(sstrdup(event));   
        event = va_arg(argp, char *);
    }
       
    va_end(argp);
    
    return TRUE;
}

void * LType::CreateInstance(LType * type)
{
    LT_CHECK_TYPE(type);
    return type->m_create_func();
}

LType * LType::FromName(const char * klass_name)
{
    g_assert(_lt_type_table != NULL);
    g_assert(klass_name != NULL);
    return _lt_type_table->Lookup(klass_name);
}

void * lt_type_create_instance(LType * type)
{
    return LType::CreateInstance(type);
}

LType * lt_type_from_name(const char * klass_name)
{
    return LType::FromName(klass_name);
}

