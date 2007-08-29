#include <lucid-base.h>

LT_DEFINE_TYPE(LObject, lt_object, NULL);

LObject::LObject(): LT_TYPE_INIT(lt_object)
{
}

LObject::~LObject()
{
}
