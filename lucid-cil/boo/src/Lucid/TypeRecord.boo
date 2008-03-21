namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection


internal class EventTable(KeyedCollection[of string, KeyValuePair[of string, int]]):
    protected override def GetKeyForItem(item as KeyValuePair[of string, int]) as string:
        return item.Key
                 
internal class TypeRecord:
    def constructor():
        self(EventTable())

    def constructor(table):
        self.Events = table
        
    def GetEventIndex(name as string) as int:
        try:
            return self.Events[name].Value
        except ex as KeyNotFoundException:
            raise

    def GetEventName(index as int) as string:
        if self.Events.Count > index + 1:
            raise ArgumentOutOfRangeException("index")
        try:        
            return Events[index].Key
        except ex as ArgumentOutOfRangeException:
            raise
            
    public CLRType as Type 
    public ParentType as TypeRecord 
    //TODO: Redo as a smart linked list
    public ChildTypes as List of TypeRecord
    public Events as EventTable
    public TypeEventCount as int   

