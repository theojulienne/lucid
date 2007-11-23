#ifndef __LUCID_LMODULE_H__
#define __LUCID_LMODULE_H__

#include <lbase.hpp>

#include <gmodule.h>

//FIXME- This can really just be a thin wrapper over g_module. No class is needed.  
//We still need to implement GetSymbolInProcess though.

class LModule: public LBase
{
public:
	static LModule * Load(const char * file_name);
    static void * GetSymbolInProcess(const char * symbol_name);
    const char * GetName();
    void * GetSymbol(const char * symbol_name);
protected:
    LModule(GModule * handle);        
    ~LModule();   
private:
    GModule * m_handle;
};

inline LModule::LModule(GModule * handle): m_handle(handle)
{
}

inline LModule::~LModule()
{
    	g_module_close(this->m_handle);
}

inline const char * LModule::GetName()
{
    	return g_module_name(this->m_handle);
}

inline void * LModule::GetSymbol(const char * symbol_name)
{
   	void * symbol = NULL;
    	g_module_symbol(this->m_handle, symbol_name, &symbol);
    	return symbol;        
}

#endif

