#include <lucid-base.h>

#define LBASE_FLOAT_BIT 1L << 31

extern "C"
{

LBase * lt_base_ref(LBase * self);
void lt_base_unref(LBase * self);

}

LBase::LBase(): ref_count(LBASE_FLOAT_BIT)
{
}

void LBase::Ref ()
{
	if (this->ref_count & LBASE_FLOAT_BIT)
		this->ref_count = 1;
	else
        	this->ref_count += 1;
}

void LBase::Unref ()
{
	if ((int)this->ref_count == LBASE_FLOAT_BIT || (int)this->ref_count == 1) 
		delete this;
    	else 
        	this->ref_count -= 1;
}

LBase * lt_base_ref(LBase * self)
{
	g_return_val_if_fail(self != NULL, NULL);
	self->Ref();
	return self;
}

void lt_base_unref(LBase * self)
{
	LT_CALL_CPP(Unref);
}

