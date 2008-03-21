namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection

class Object(EventObject):

    parentas as Lucid.Object = null
    //FIXME: This will actually be a custom linked-list that handles parent/child semantics.
    children as List of Lucid.Object = List of Lucid.Object()
    disposed as bool = false

    def constructor():
        pass
 
    def constructor(parent as Lucid.Object):
        self.parent = parent
        
    //FIXME
    virtual def Dispose():
        if self.diposed:
            return           
        GC.SuppressFinalize(self)            
        self.disposed = true
       
    property Parent as Lucid.Object:
        get:
            return self.parent

    property Children as List of Lucid.Object:
        get:
            return self.children

