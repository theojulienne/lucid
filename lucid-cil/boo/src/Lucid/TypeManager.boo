namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection


internal abstract class TypeManager:

    static type_table = Dictionary[of Type, TypeRecord]()
    
    //FIXME: catch, and then throw the proper exception, et all.
    static def GetRecord(type as Type) as TypeRecord:
        //TODO: Figure out why Contains() doesn't work here.
        if not type_table.ContainsKey(type):
            type_table[type] = AddRecord(type)
        return type_table[type]
    
    static def AddRecord(type as Type):
        if type is null:
            raise ArgumentNullException("type")

        record = TypeRecord(null)
        record.TypeEventCount = 0
        record.CLRType = type

        if type is not typeof(Lucid.BaseObject):
            record.ParentType = GetRecord(type.BaseType)
            if record.ParentType.ChildTypes is null:
                record.ParentType.ChildTypes = List of TypeRecord()
            record.ParentType.ChildTypes.Add(record)
            record.Events = record.ParentType.Events
        else:
            record.Events = EventTable()  
            return record

        fields = List of FieldInfo()
        for field in type.GetFields():
            attrs = field.GetCustomAttributes(false)
            for attr in attrs:
                if attr.GetType() == typeof(EventAttribute):
                    //TODO: readonly too
                    if field.IsPublic and field.IsStatic and field.Name.EndsWith("Event") and field.FieldType == typeof(int):
                        fields.Add(field)    
                    else:
                        raise System.NotSupportedException(String.Format("Field \"{0}.{1}\" is fail.", type, field.Name));

        if fields.Count > 0:
            if record.ParentType.ChildTypes.Count > 1:
                record.Events = EventTable()
                for i in range(LastEventIndex(record)):
                    record.Events.Add(record.ParentType.Events[i])
                    
            for field in fields:
                index = AddEvent(record, field.Name)
                field.SetValue(null, index)

        return record

    static def LastEventIndex(type_record as TypeRecord) as int:
        index = 0
        parent = type_record.ParentType

        while parent is not null:
            index += parent.TypeEventCount
            parent = parent.ParentType
        return index;

    static def AddEvent(type_record as TypeRecord, name as string) as int:
        index = LastEventIndex(type_record) + type_record.TypeEventCount
        type_record.Events.Add(KeyValuePair[of string, int](name, index))
        type_record.TypeEventCount += 1
        return index
            
