#ifndef __LUCID_LDISPATCHLIST_H__
#define __LUCID_LDISPATCHLIST_H__

#include "FastDelegate.h"

class LObject;
class LEvent;

typedef fastdelegate::FastDelegate<void (LObject *, LEvent *, void *)> LDelegate;

class LDispatchList
{
protected:
    LDispatchList();
public:
    ~LDispatchList();
    static LDispatchList * Add(LDispatchList * list, LDelegate func, void * user_data, 
        void (* val_free_fn) (void *));
    static LDispatchList * Remove(LDispatchList * list, LDispatchList * link);
    static void Invoke(LDispatchList * list, LObject * object, LEvent * event);
    static void Destroy(LDispatchList * list);
private:
    LDispatchList * m_next;
    LDelegate m_func;
    void * m_user_data;
    void (* m_val_free_fn) (void *);  

    static LDispatchList * FindPrevious(LDispatchList * list, LDispatchList * link);              
};

inline LDispatchList::LDispatchList(): m_next(NULL), m_func(NULL), m_user_data(NULL), m_val_free_fn(NULL)
{      
}    

inline LDispatchList::~LDispatchList()
{    
    if(this->m_val_free_fn)
        this->m_val_free_fn(this->m_user_data);
}

inline LDispatchList * LDispatchList::Add(LDispatchList * list, LDelegate func, void * user_data, 
        void (* val_free_fn) (void *))
{
    LDispatchList * new_list = new LDispatchList();

    new_list->m_func = func;
    new_list->m_user_data = user_data;
    new_list->m_val_free_fn = val_free_fn;
    new_list->m_next = list;

    return new_list;
}

inline LDispatchList * LDispatchList::FindPrevious(LDispatchList * list, LDispatchList * link)
{
    LDispatchList * prev = NULL;
	
    while (list) 
    {
        if (list == link)
			break;
		prev = list;
		list = list->m_next;
	}

	return prev;
}

inline LDispatchList * LDispatchList::Remove(LDispatchList * list, LDispatchList * link)
{
    LDispatchList * prev = FindPrevious(list, link);
	LDispatchList * current = prev ? prev->m_next : list;

	if (current) 
    {
		if (prev)
			prev->m_next = current->m_next;
		else
			list = current->m_next;
		
        current->m_next = NULL;
	}

    delete link;
	
    return list;
}

inline void LDispatchList::Invoke(LDispatchList * list, LObject * object, LEvent * event)
{
    while(list)
    {
        list->m_func(object, event, list->m_user_data);
        list = list->m_next;
    }                       
}

inline void LDispatchList::Destroy(LDispatchList * list)
{
    while(list)
    {
        LDispatchList * next = list->m_next;
        delete list;
        list = next;
    }     
}

#endif

