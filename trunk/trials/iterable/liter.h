#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus

#define	LT_DEFINE_ITER_FULL(klass, item, free_fn, obj)	\
			static bool MoveNext(klass * self); \
			static item * Current(klass * self); \
			inline LIter<item> GetIter() \
			{	\
				return LIter<item>((bool(*)(void *))&klass::MoveNext, \
				(void *(*)(void *))&klass::Current, \
				(void (*) (void *))free_fn, (void *)obj); \
			}	\

#define	LT_DEFINE_ITER(klass, item)	LT_DEFINE_ITER_FULL(klass, item, NULL, this)

class LIterImpl
{
public:
	LIterImpl(bool (* move_next_fn) (void *), 
			void * (* get_cur_fn) (void *),
			void (* val_free_fn) (void *), 
			void * obj);
	~LIterImpl();
	bool MoveNext();
	void * Current();
private:
	bool (* m_move_next_fn) (void *);
	void * (* m_get_cur_fn) (void *);
	void (* m_val_free_fn) (void *);
	void * m_obj;
};

LIterImpl::LIterImpl(bool (* move_next_fn) (void *), 
			void * (* get_cur_fn) (void *),
			void (* val_free_fn) (void *), 
			void * obj)
{
	assert(move_next_fn != NULL);
	assert(get_cur_fn != NULL);
	this->m_move_next_fn = move_next_fn;
	this->m_get_cur_fn = get_cur_fn;
	this->m_val_free_fn = val_free_fn;
	this->m_obj = obj;
}

inline LIterImpl::~LIterImpl()
{
	if(this->m_val_free_fn)
		this->m_val_free_fn(this->m_obj);
}

inline bool LIterImpl::MoveNext()
{
	return this->m_move_next_fn(this->m_obj);
}

inline void * LIterImpl::Current()
{
	return this->m_get_cur_fn(this->m_obj);
}

template <class V = void *> class LIter: LIterImpl
{
public:
	LIter(bool (* move_next_fn) (void *), 
			void * (* get_cur_fn) (void *),
			void (* val_free_fn) (void *), 
			void * obj);
	bool MoveNext();
	V Current();
};

template <class V>
inline LIter<V>::LIter(bool (* move_next_fn) (void *), 
			void * (* get_cur_fn) (void *),
			void (* val_free_fn) (void *), 
			void * obj): LIterImpl(move_next_fn, get_cur_fn, val_free_fn, obj)
{
}

template <class V>
inline bool LIter<V>::MoveNext()
{
	return LIterImpl::MoveNext();
}

//FIXME- I'm not so sure I like the idea of being able
// to take down the whole system this easily. e.g. SEGFAULT :-P
template <class V>
inline V LIter<V>::Current()
{
	return * (V *)LIterImpl::Current();
}

#else

typedef struct 
{
	int  (* m_move_next_fn) (void *);
	void * (* m_get_cur_fn) (void *);
	void (* m_val_free_fn) (void *);
	void * m_obj;
}	LIter;

#define	lt_iter_move_next(it)	(it)->m_move_next_fn((it)->m_obj)
#define	lt_iter_current(it)	(it)->m_get_cur_fn((it)->m_obj)
#define	lt_iter_destroy(it)	do { if((it)->m_val_free_fn) { \
					(it)->m_val_free_fn((it)->m_obj); } } while(0)

#endif

