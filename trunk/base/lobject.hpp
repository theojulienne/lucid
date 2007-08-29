#ifndef __LUCID_LOBJECT_H__
#define __LUCID_LOBJECT_H__

#include <lbase.hpp>

class LObject: public LBase
{
public:
    LObject();
    ~LObject();

    void Construct();

    LType * GetType();    
    LObject * GetParent();    
    LArray<LObject *> * GetChildren();

private:
    LType * m_type;
    LObject * m_parent;
    LArray<LObject *> * m_children;    
};

inline LType * LObject::GetType()
{
    return this->m_type;
}

inline LObject * LObject::GetParent()
{
    return this->m_parent;
}    

inline LArray<LObject *> * LObject::GetChildren()
{
    return this->m_children;
}


#endif

