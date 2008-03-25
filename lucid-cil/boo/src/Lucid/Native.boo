namespace Lucid

import System
import System.IO
import System.Reflection
import System.Collections.Generic


#FIXME: static classes in Boo?
internal abstract class Native:

    public static Factory as INativeFactory = null
    
    static def Load():
        if Factory is not null:
            return

        asm = Assembly.GetExecutingAssembly()
        this_asm_path = asm.Location
        dirs = [Path.GetDirectoryName(this_asm_path)]
        
        impl_attr as NativeFactoryAttribute = null

        for dir in dirs:
            for file in Directory.GetFiles(dir, "Lucid.*.dll"):
                if(Path.GetFileName(file) != Path.GetFileName(this_asm_path)): 
                    provider_asm as Assembly = null
                    try:
                        provider_asm = Assembly.LoadFile(file)
                    except:
                        pass                       
                    for attr in provider_asm.GetCustomAttributes(false):
                        if attr isa NativeFactoryAttribute:
                            impl_attr = attr
                            break   
                     
        if impl_attr is null or impl_attr.Type is null:
            Native.Factory = FallbackFactory()
    
        t = impl_attr.Type
        method = t.GetMethod("Get", BindingFlags.Static | BindingFlags.Public)
        Native.Factory = method.Invoke(null, null)

