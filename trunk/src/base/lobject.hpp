#ifndef __LUCID_LOBJECT_H__
#define __LUCID_LOBJECT_H__

#include <lbase.hpp>

class LObject;
class LEvent;

typedef void lt_object_event_func(LObject * sender, LEvent * args, void * user_data);

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

    	LEventID AddHandler(char * event_name, lt_object_event_func * event_func, 
        	void * user_data, void (* val_free_fn) (void *));
    	bool_t RemoveHandler (LEventID event_id);
    	LEventID FindHandler(lt_object_event_func * event_func, void * user_data);
    
protected:
    	void Construct();
    	void SendRaw(char * event_name, LEvent * args);

private:
    	void SetupEvents();
    
    	LType * m_type;
    	LObject * m_parent;
    	LArray<LObject *> * m_children;    
    	LHashtable<char *, GSList *> * m_events;
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

