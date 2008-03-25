namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection


class Object(EventObject):

    parent as Lucid.Object = null
    //FIXME: This will be a custom singly linked-list that handles parent/child semantics.
    children as LinkedList of Lucid.Object = LinkedList of Lucid.Object()
    disposed as bool = false

    def constructor():
        self(null)
 
    def constructor(parent as Lucid.Object):
        self.parent = parent
       
    //FIXME
    /*virtual def Dispose():
        if self.disposed:
            return           
        GC.SuppressFinalize(self)            
        self.disposed = true
    */
    
    def Reparent(new_parent as Lucid.Object):
        raise NotImplementedException()
       
    Parent as Lucid.Object:
        get:
            return self.parent

    def AddChild(child as Lucid.Object):
        assert child is not null
        self.children.AddFirst(child)
    
    def RemoveChild(child as Lucid.Object):
        assert self.children.Remove(child)
        
    Children as IEnumerator of Lucid.Object:
        get:
            return self.children.GetEnumerator()    
                 
