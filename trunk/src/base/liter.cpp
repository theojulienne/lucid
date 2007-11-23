#include <lucid-base.h>

LIterImpl::LIterImpl(bool_t (* move_next_fn) (void *), 
			void * (* get_cur_fn) (void *),
			void (* val_free_fn) (void *), 
			void * obj)
{
	g_assert(move_next_fn != NULL);
	g_assert(get_cur_fn != NULL);
	this->m_move_next_fn = move_next_fn;
	this->m_get_cur_fn = get_cur_fn;
	this->m_val_free_fn = val_free_fn;
	this->m_obj = obj;
}

static bool_t _lt_iter_dummy_move_next(void * user_data)
{
	return FALSE;
}

static void * _lt_iter_dummy_current(void * user_data)
{
	return NULL;
}

LIterImpl lt_iter_dummy()
{
	return LIterImpl(_lt_iter_dummy_move_next, _lt_iter_dummy_current, NULL, NULL); 
}
