#include <lucid-base.h>

extern "C"
{

LEventID lt_object_add_handler(LObject * self, char * event_name, 
	lt_object_event_func * event_func, void * user_data, void * (* val_free_fn) (void *));
bool_t lt_object_remove_handler(LObject * self, LEventID event_id);
LEventID lt_object_find_handler(LObject * self, lt_object_event_func * event_func, void * user_data); 

}

static void _lt_signal_destroy(LSignal * signal)
{
	delete signal;
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
    	this->m_events = new LHashtable<char *, LSignal *>(FALSE, _lt_signal_destroy);

    	for(LType * type = this->m_type; type != NULL; type = type->GetParent())
    	{
        	LArray<char *> * events = type->GetEvents();   
        	for(int i = 0, len = events->Count(); i < len; i++)
			this->m_events->Insert(* events->GetItem(i), new LSignal());
    	}    
}

LEventID LObject::AddHandler(char * event_name, lt_object_event_func * event_func, 
	void * user_data, void * (* val_free_fn) (void *))
{
	LSignal * signal;
    	sigc::slot_base ** event_id;

    	g_return_val_if_fail(event_name != NULL, 0);
    	g_return_val_if_fail(event_func != NULL, 0);

    	signal = this->m_events->Lookup(event_name);

	if(! signal)
        	return 0;

	sigc::connection c = signal->connect(sigc::bind(sigc::ptr_fun(event_func), user_data));	
	event_id = (sigc::slot_base **)&c;
	
	if(val_free_fn)    	
		(* event_id)->add_destroy_notify_callback(user_data, val_free_fn);
	
	return (LEventID) * event_id;
}

bool_t LObject::RemoveHandler (LEventID event_id)
{
	sigc::slot_base * base = (sigc::slot_base *)event_id;

    	g_return_val_if_fail(base != NULL, FALSE);
    
	base->disconnect();
	
    	return TRUE;
}

LEventID LObject::FindHandler(lt_object_event_func * event_func, void * user_data)
{
	//FIXME- Is this implement-able ? Do we even care?
	return (LEventID)NULL;
}

void LObject::SendRaw(char * event_name, LEvent * args)
{
	LSignal * signal = this->m_events->Lookup(event_name);
	// FIXME - SILENTLY FAIL ?
	if(signal)
		signal->emit(this, args);
}

LEventID lt_object_add_handler(LObject * self, char * event_name, 
    lt_object_event_func * event_func, void * user_data, void * (* val_free_fn) (void *))
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

