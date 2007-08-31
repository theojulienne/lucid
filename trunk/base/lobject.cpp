#include <lucid-base.h>

extern "C"
{
void lt_object_add_handler(LObject * self, char * event_name, lt_object_event_func * event_func, void * user_data);
void lt_object_remove_handler(LObject * self, char *event_name, lt_object_event_func * event_func, void * user_data);
}

static void _lt_object_events_free(void * value)
{
    GList *l;

	for (l = (GList *)value; l; l = l->next)
        g_free(l->data);

    g_list_free((GList *)value);
}

LT_DEFINE_TYPE(LObject, lt_object, NULL);

bool_t LObject::RegisterEvents(LType * type)
{
    return type->AddEvent("foo");
}

void LObject::Construct()
{
    this->m_children = NULL;
    this->m_parent = NULL;
}

LObject::~LObject()
{
    delete this->m_events, this->m_events_map;
}

void LObject::SetupEvents()
{
    LType * type;

    this->m_events_map = new LHashtable<char *, void *>(TRUE, NULL);
    this->m_events = new LArray<GList *>(_lt_object_events_free);

    for(type = this->m_type; type != NULL; type = type->GetParent())
    {
        LArray<char *> * events;    
        int i, j, len;

        events = type->GetEvents();   
        len = events->Count();

        for(i = 0; i < len; i++)
        {
            j = this->m_events->Count();
            this->m_events_map->Insert(sstrdup(events->GetItem(i)), GINT_TO_POINTER(j));
            this->m_events->Append(NULL);
        }        
    }    
}

void LObject::AddHandler(char * event_name, lt_object_event_func * event_func, void * user_data)
{
    char * orig_key;
    void * value;
    GList * event_handlers;
    LEventHandler * handler;

    g_return_if_fail(event_name != NULL);
    g_return_if_fail(event_func != NULL);

    // FIXME - SILENTLY FAIL ?
    if(! this->m_events_map->LookupExtended(event_name, &orig_key, &value))
        return;

    handler = g_new(LEventHandler, 1);
    g_assert(handler);    

    handler->event_func = event_func;
    handler->user_data = user_data;

    event_handlers = this->m_events->GetItem(GPOINTER_TO_INT(value));
    event_handlers = g_list_append(event_handlers, handler);
    this->m_events->SetItem(GPOINTER_TO_INT(value), event_handlers);
}

void LObject::RemoveHandler (char *event_name, lt_object_event_func * event_func, void * user_data)
{
    g_return_if_fail(event_name != NULL);
    g_return_if_fail(event_func != NULL);        
}

void LObject::SendRaw(char * event_name, LHashtable<char *, LTypebox *> * args)
{
    char * orig_key;
    void * value;
    GList * event_handlers, * l;
    LEventHandler * handler;

    // FIXME - SILENTLY FAIL ?
    if(! this->m_events_map->LookupExtended(event_name, &orig_key, &value))
        return;

    event_handlers = this->m_events->GetItem(GPOINTER_TO_INT(value));
    
    if(event_handlers == NULL)
        return;

    for(l = event_handlers; l; l = l->next)
    {
        handler = (LEventHandler *)l->data;
        handler->event_func(this, args, handler->user_data);
    }    
}

void lt_object_add_handler(LObject * self, char * event_name, lt_object_event_func * event_func, void * user_data)
{
    LT_CALL_SELF_CPP(AddHandler, event_name, event_func, user_data);
}

void lt_object_remove_handler(LObject * self, char *event_name, lt_object_event_func * event_func, void * user_data)
{
    LT_CALL_SELF_CPP(RemoveHandler, event_name, event_func, user_data);
}

