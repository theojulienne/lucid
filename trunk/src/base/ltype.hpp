#ifndef __LUCID_LTYPE_H__
#define __LUCID_LTYPE_H__

#include <larray.hpp>

typedef void * lt_type_create_func();

struct LType
{
protected:
	LType();

public:    
    	const char * GetName();
    	LType * GetParent();
    
    	bool_t AddEvents(char * first_event, ...);
    	LArray<char *> * GetEvents();

    	static LType * Register(const char * klass_name, 
        	LType * super_klass, lt_type_create_func * create_func);
    	static void * CreateInstance(LType * type);  
    	static LType * FromName(const char * klass_name);     

private:
    	LArray<char *> * m_events;
    	const char * m_klass_name; 
    	LType * m_super_klass;
    	lt_type_create_func * m_create_func;    
};


inline LType::LType()/*: m_klass_name(NULL),  m_super_klass(NULL), m_create_func(NULL), m_events(NULL)*/
{
}


inline const char * LType::GetName()
{
    	return this->m_klass_name;
}

inline LType * LType::GetParent()
{
    	return this->m_super_klass;
}

inline LArray<char *> * LType::GetEvents()
{
    	return this->m_events;
}
 
#define LT_DEFINE_TYPE(cpp_klass, c_name, super_klass) \
extern "C" LType * c_name##_get_type(); \
cpp_klass::cpp_klass () \
{ \
    m_type = c_name##_get_type(); \
    this->SetupEvents(); \
    this->Construct(); \
} \
static void * c_name##_ctor() \
{ \
    LT_NEW_CPP(cpp_klass); \
} \
extern "C" cpp_klass * c_name##_create() \
{ \
	return (cpp_klass *)LType::CreateInstance(c_name##_get_type()); \
} \
extern "C" LType * c_name##_get_type() \
{ \
	static LType * type = NULL; \
    if(type == NULL) \
    { \
        type = LType::Register(#cpp_klass, super_klass, c_name##_ctor); \
        g_assert(cpp_klass::RegisterEvents(type)); \
    } \
    return type; \
} 

#endif
