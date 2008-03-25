namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection


abstract class EventObject(BaseObject):

    private events as (Delegate) = null
        
    def constructor():
        count = self.TypeRecord.Events.Count
        if count > 0:
            self.events = array(Delegate, count)

    def AddHandler(event_name as string, handler):
        index = TypeManager.GetRecord(self.GetType()).GetEventIndex(event_name)  
        self.AddHandler(index, handler)

    def RemoveHandler(event_name as string, handler):
        index = TypeManager.GetRecord(self.GetType()).GetEventIndex(event_name)  
        self.RemoveHandler(index, handler)

    protected def AddHandler(index as int, handler):
        try:
            d = self.events[index]
            d = Delegate.Combine(d, handler)
            self.events[index] = d         
        except:
            raise

    protected def RemoveHandler(index as int, handler):
        try:
            d = self.events[index]
            d = Delegate.Remove(d, handler)
            self.events[index] = d         
        except:
            raise
                  
    protected def Emit(event_name as string, *args):
        index = TypeManager.GetRecord(GetType()).GetEventIndex(event_name)
        self.Emit(index, args)      
            
    protected def Emit(index as int, *args):
        d = self.events[index]
        if d is not null:
            d.DynamicInvoke(args)

