#ifndef __LUCID_LTYPE_H__
#define __LUCID_LTYPE_H__

typedef void * lt_type_create_func();

struct LType
{
protected:
    LType();
public:    
    static LType * Register(const char * klass_name, 
        LType * super_klass, lt_type_create_func * create_func);
    static void * CreateInstance(LType * type);  
    static LType * FromName(const char * klass_name);     
private:
    const char * m_klass_name; 
    LType * m_super_klass;
    lt_type_create_func * m_create_func;    
};

inline LType::LType(): m_klass_name(NULL),  m_super_klass(NULL), m_create_func(NULL)
{
}

#define LT_DEFINE_TYPE(cpp_klass, c_name, super_klass) \
extern "C" LType * c_name##_register_type(); \
cpp_klass::cpp_klass () \
{ \
    m_type = c_name##_register_type(); \
    this->Construct(); \
} \
static void * c_name##_ctor() \
{ \
    return new cpp_klass(); \
} \
extern "C" cpp_klass * c_name##_create() \
{ \
    return (cpp_klass *)LType::CreateInstance(c_name##_register_type()); \
} \
extern "C" LType * c_name##_register_type() \
{ \
    static LType * type = NULL; \
    if(type == NULL) \
    { \
        type = LType::Register(#cpp_klass, super_klass, c_name##_ctor); \
    } \
    return type; \
} 

#endif
