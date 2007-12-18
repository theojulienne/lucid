#include <lucid-base.h>

#include <ldispatchlist.hpp>

extern "C"
{

LEventID lt_object_add_handler(LObject * self, char * event_name, 
	lt_object_event_func * event_func, void * user_data, void (* val_free_fn) (void *));
bool_t lt_object_remove_handler(LObject * self, LEventID event_id);
LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data); 

}

static void _lt_dispatchlist_destroy(LDispatchList * list)
{
    LDispatchList::Destroy(list);
}

LT_DEFINE_TYPE(LObject, lt_object, NULL);

bool_t LObject::RegisterEvents(LType * type)
{
	return type->AddEvents("foo", "bar", NULL);
}

void LObject::Construct()
{
    this->m_children = NULL;
    this->m_parent = NULL;
}

LObject::~LObject()
{
    delete this->m_events;
}

void LObject::SetupEvents()
{
    this->m_events = new LHashtable<char *, LDispatchList *>(FALSE, _lt_dispatchlist_destroy);

    for(LType * type = this->m_type; type != NULL; type = type->GetParent())
    {
        LArray<char *> * events = type->GetEvents();   
        for(int i = 0, len = events->Count(); i < len; i++)
			this->m_events->Insert(* events->GetItem(i), NULL);
    }    
}

LEventID LObject::AddHandler(char * event_name, lt_object_event_func * event_func, 
	void * user_data, void (* val_free_fn) (void *))
{
	LDispatchList * list;
    char * orig_key;

    g_return_val_if_fail(event_name != NULL, 0);
    g_return_val_if_fail(event_func != NULL, 0);
    
    if(! this->m_events->LookupExtended(event_name, &orig_key, &list))
        return 0;

    list = LDispatchList::Add(list, LDelegate(event_func), user_data, val_free_fn);
    this->m_events->Insert(orig_key, list);

	return (LEventID)list;
}

//TODO- How is this going to work!?
bool_t LObject::RemoveHandler (LEventID event_id)
{
    LDispatchList * list = (LDispatchList *)event_id;

    g_return_val_if_fail(list != NULL, FALSE);

    return FALSE;
}

LEventID LObject::FindHandler(lt_object_event_func * event_func, void * user_data)
{
	//FIXME- Is this implement-able ? Do we even care?
	return (LEventID)NULL;
}

void LObject::EmitRaw(char * event_name, LEvent * args)
{
	LDispatchList * list = this->m_events->Lookup(event_name);
	// FIXME - SILENTLY FAIL ?
    LDispatchList::Invoke(list, this, args);
}

LEventID lt_object_add_handler(LObject * self, char * event_name, 
    lt_object_event_func * event_func, void * user_data, void (* val_free_fn) (void *))
{
   	LT_RET_CALL_CPP(AddHandler, 0, event_name, event_func, user_data, val_free_fn);
}

bool_t lt_object_remove_handler(LObject * self, LEventID event_id)
{
    LT_RET_CALL_CPP(RemoveHandler, FALSE, event_id);
}

LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data)
{
    LT_RET_CALL_CPP(FindHandler, FALSE, event_func, user_data);
}

