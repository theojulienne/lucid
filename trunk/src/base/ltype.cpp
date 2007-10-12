#include <stdarg.h>

#include <lucid-base.h>

#define LT_CHECK_TYPE(type) G_STMT_START { \
	g_assert(type != NULL); \
    	g_assert(type->m_klass_name != NULL); \
    	g_assert(type->m_create_func != NULL); \
    	} \
    	G_STMT_END

//LObject -> lt_object .. Does this even need to be public? Probably...
extern "C" char * _lt_type_format_name(const char * klass_name)
{
	GString * name = g_string_new(klass_name);
	g_assert(name->str[0] == 'L');
	name->str[0] = g_ascii_tolower(name->str[0]);
	name = g_string_insert_c(name, 1, 't');
	int i = 2;
	do
	{
		if(g_ascii_isupper(name->str[i]))
	    	{
			name->str[i] = g_ascii_tolower(name->str[i]);
			name = g_string_insert_c(name, i, '_');
	    	}
	    	i++;
	}
	while(i < (int)name->len -1);

	return g_string_free(name, FALSE);
}

extern "C"  
{

void lt_type_init();
void * lt_type_create_instance(LType * type);
LType * lt_type_from_name(const char * klass_name);

}

static LHashtable<const char *, LType *> * _lt_type_table = NULL;
static LModule * _lt_this_module = NULL;

void lt_type_init()
{
	g_assert(_lt_type_table == NULL && _lt_this_module == NULL);
    	_lt_type_table = new LHashtable<const char *, LType *>(FALSE, NULL);
    	_lt_this_module = LModule::Load(NULL);
    	g_assert(_lt_type_table != NULL && _lt_this_module != NULL);
//TEST
//    	printf("%s: %s\n", __FUNCTION__, _lt_type_format_name("LObject"));
//    	printf("%s: %s\n", __FUNCTION__, _lt_type_format_name("LColorDialog"));
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

//This may look nasty...but is is extremely powerful. Our type system only exists at runtime.
//So we have a 'chicken and the egg' issue of needing the LType,
//before we have created an instance of the given type.
//
// We are actually putting a call to _register_type() in the .init section, so this can actually be avoided.
// But LModule::GetSymbolInProcess() STILL should be implemented. 
LType * LType::FromName(const char * klass_name)
{
    	g_assert(_lt_type_table != NULL && _lt_this_module != NULL);
    	g_assert(klass_name != NULL);
    	LType * type = _lt_type_table->Lookup(klass_name);
    	if(type == NULL)
    	{
		char * func_name, * c_abi_name = _lt_type_format_name(klass_name);
		func_name = g_strconcat(c_abi_name, "_register_type", NULL);
		LType * ( * register_type) () = (LType * ( * )())_lt_this_module->GetSymbol(func_name);
		g_free(c_abi_name);
		g_free(func_name);
		if(register_type == NULL)
			return NULL;
		else
			return register_type();
    	}
    	else
		return type;
}

void * lt_type_create_instance(LType * type)
{
    	return LType::CreateInstance(type);
}

LType * lt_type_from_name(const char * klass_name)
{
    	return LType::FromName(klass_name);
}

