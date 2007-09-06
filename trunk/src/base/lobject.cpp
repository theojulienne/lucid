#include <lucid-base.h>

extern "C"
{
LEventID lt_object_add_handler(LObject * self, char * event_name, 
    lt_object_event_func * event_func, void * user_data, void (* val_free_fn) (void *));
bool_t lt_object_remove_handler(LObject * self, LEventID event_id);
LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data); 
}

class LEventHandler
{
public:
    lt_object_event_func * event_func;
    void * user_data;
    void (* val_free_fn) (void *);
};

static void _lt_object_events_free(void * value)
{
    
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
    LArray<GSList *> * handlers = this->m_events->GetValues();
    GSList * handler;

    for(int i = 0, len = handlers->Count(); i < len; i++)   
    {
        handler = handlers->GetItem(i);
	    
        if(! handler)
            continue;
        
        for (GSList * l = handler; l; l = l->next)
            g_free(l->data);
        
        g_slist_free(handler);                    
    }

    delete this->m_events, handlers;
}

void LObject::SetupEvents()
{
    this->m_events = new LHashtable<char *, GSList *>(TRUE, NULL);

    for(LType * type = this->m_type; type != NULL; type = type->GetParent())
    {
        LArray<char *> * events = type->GetEvents();   
        for(int i = 0, len = events->Count(); i < len; i++)
            this->m_events->Insert(sstrdup(events->GetItem(i)), NULL);
    }    
}

LEventID LObject::AddHandler(char * event_name, lt_object_event_func * event_func, 
    void * user_data, void (* val_free_fn) (void *))
{
    char * orig_key;
    GSList * event_handlers;
    LEventHandler * handler;

    g_return_val_if_fail(event_name != NULL, 0);
    g_return_val_if_fail(event_func != NULL, 0);

    if(! this->m_events->LookupExtended(event_name, &orig_key, &event_handlers))
        return 0;

    handler = g_new(LEventHandler, 1);
    g_assert(handler);    

    handler->event_func = event_func;
    handler->user_data = user_data;
    handler->val_free_fn = val_free_fn;

    event_handlers = g_slist_append(event_handlers, handler);
    this->m_events->Insert(sstrdup(event_name), event_handlers);

    return (LEventID)handler;
}

bool_t LObject::RemoveHandler (LEventID event_id)
{
    LArray<LHashtableEntry<char *, GSList *> *> * pairs; 
    LEventHandler * handler = NULL;
    GSList * l, * event_handlers;
    char * event_name = NULL;

    g_return_val_if_fail(event_id > 0, FALSE);
    
    pairs = this->m_events->GetPairs();

    for(int i = 0, len = pairs->Count(); i < len; i++)
    {
        l = event_handlers = pairs->GetItem(i)->Value;     
        event_name = pairs->GetItem(i)->Key;

        if(l == NULL || event_name == NULL)
            continue;
    
        for (; l; l = l->next)
        {
            if((uint64_t)l->data == event_id)
            {
                handler = (LEventHandler *)l->data;
                goto found_handler;
            }
        }
    }

found_handler:

    delete pairs;

	if (handler == NULL || event_name == NULL) 
		return FALSE;

    if(handler->val_free_fn)
        handler->val_free_fn(handler->user_data);
    g_free(handler);

    event_handlers = g_slist_delete_link (event_handlers, l); 
    this->m_events->Insert(sstrdup(event_name), event_handlers);

    return TRUE;
}

LEventID LObject::FindHandler(lt_object_event_func * event_func, void * user_data)
{
    LArray<GSList *> * values = this->m_events->GetValues();
    LEventID event_id = 0;

    for(int i = 0, len = values->Count(); i < len; i++)
    {
        for(GSList * l = values->GetItem(i); l; l = l->next)
        {
            LEventHandler * handler = (LEventHandler *)l->data;
            if(handler->event_func == event_func && handler->user_data == user_data)
            {
                event_id = (LEventID)handler;
                goto found_id;            
            }
        }    
    }

found_id:
    delete values;    
    return event_id;
}

void LObject::SendRaw(char * event_name, LEvent * args)
{
    char * orig_key;
    GSList * event_handlers, * l;
    LEventHandler * handler;

    // FIXME - SILENTLY FAIL ?
    if(! this->m_events->LookupExtended(event_name, &orig_key, &event_handlers))
        return;

    if(event_handlers == NULL)
        return;

    for(l = event_handlers; l; l = l->next)
    {
        handler = (LEventHandler *)l->data;
        handler->event_func(this, args, handler->user_data);
    }    
}

LEventID lt_object_add_handler(LObject * self, char * event_name, 
    lt_object_event_func * event_func, void * user_data, void (* val_free_fn) (void *))
{
    LT_RETURN_CALL_SELF_CPP(AddHandler, 0, event_name, event_func, user_data, val_free_fn);
}

bool_t lt_object_remove_handler(LObject * self, LEventID event_id)
{
    LT_RETURN_CALL_SELF_CPP(RemoveHandler, FALSE, event_id);
}

LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data)
{
    LT_RETURN_CALL_SELF_CPP(FindHandler, FALSE, event_func, user_data);
}

