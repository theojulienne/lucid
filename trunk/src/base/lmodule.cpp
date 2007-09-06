#include <lucid-base.h>

extern "C"
{
LModule * lt_module_load(const char * file_name);
const char * lt_module_get_name(LModule * self);
void * lt_module_get_symbol(LModule * self, const char * symbol_name);
}

LModule * LModule::Load(const char * file_name)
{
    GModule * module;

    module = g_module_open(file_name, (GModuleFlags)0);

    if(module)
        return new LModule(module);
    else
        return NULL;
}

//FIXME- Add LError support.
LModule * lt_module_load(const char * file_name)
{
    return LModule::Load(file_name);
}

const char * lt_module_get_name(LModule * self)
{
    LT_RETURN_CALL_SELF_CPP(GetName, NULL);    
}

void * lt_module_get_symbol(LModule * self, const char * symbol_name)
{
    LT_RETURN_CALL_SELF_CPP(GetSymbol, NULL, symbol_name);
}

