#ifndef __LUCID_LBASE_H__
#define __LUCID_LBASE_H__

#include <stdio.h>
#include <stdint.h>

#include <glib.h>

#ifndef bool_t
typedef int bool_t;
#endif

class LBase
{
public:
    LBase();
    virtual ~LBase()  { }
    void Ref();
    void Unref();
private:
    volatile uint32_t ref_count;        
};

#endif
