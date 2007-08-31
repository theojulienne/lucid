#ifndef __LUCID_LOBJECT_H__
#define __LUCID_LOBJECT_H__

#include <lbase.hpp>

class LTypebox;
class LObject;

#ifndef lt_object_event_func
typedef void lt_object_event_func(LObject * sender, LHashtable<char *, LTypebox *> * args, void * user_data);
#endif


class LEventHandler
{
public:
    lt_object_event_func * event_func;
    void * user_data;
};


class LObject: public LBase
{
public:
    LObject();
    ~LObject();

    //REQUIRED
    static bool_t RegisterEvents(LType * type);

    LType * GetType();    
    LObject * GetParent();    
    LArray<LObject *> * GetChildren();

    void AddHandler(char * event_name, lt_object_event_func * event_func, void * user_data);
    void RemoveHandler (char *event_name, lt_object_event_func * event_func, void * user_data);
    
protected:
    void Construct();
    void SendRaw(char * event_name, LHashtable<char *, LTypebox *> * args);

private:
    void SetupEvents();
    
    LType * m_type;
    LObject * m_parent;
    LArray<LObject *> * m_children;    

    LHashtable<char *, void *> * m_events_map;
    LArray<GList *> * m_events;
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

