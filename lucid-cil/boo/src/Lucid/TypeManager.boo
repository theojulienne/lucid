using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;

namespace Lucid
{
    using EventTable = KeyedValuePairCollection<string, int>;

    internal static class TypeManager
    {
        private static Dictionary<Type, TypeRecord> type_table;
    
        static TypeManager()
        {
            type_table = new Dictionary<Type, TypeRecord>();
        }

        //FIXME: catch, and then throw the proper exception, et all.
        public static TypeRecord GetRecord(Type type)
        {
            //TODO: Figure out why Contains() doesn't work here.
            if(! type_table.ContainsKey(type))
                type_table[type] = AddRecord(type);
            return type_table[type];
        }

        private static TypeRecord AddRecord(Type type)
        {
            if(type == null)
                throw new ArgumentNullException("type");

            TypeRecord record = new TypeRecord(null);
            record.TypeEventCount = 0;
            record.CLRType = type;

            if(type != typeof(Lucid.BaseObject))
            {
                record.ParentType = GetRecord(type.BaseType);
                if(record.ParentType.ChildTypes == null)
                    record.ParentType.ChildTypes = new List<TypeRecord>();
                record.ParentType.ChildTypes.Add(record);
                record.Events = record.ParentType.Events;
            }
            else
            {
                record.Events = new EventTable();      
                return record;
            }
            
            //Split
/*            if( )
            {
                record.Events = new EventTable();
                foreach(KeyValuePair<string, int> event_record in record.ParentType.Events)
                    record.Events.Add(event_record);
            }
*/
            List<FieldInfo> fields = new List<FieldInfo>();

            foreach(FieldInfo field in type.GetFields())
            {
                object[] attrs = field.GetCustomAttributes(false);
                foreach(Attribute attr in attrs)
                {
                    if(attr.GetType() == typeof(EventAttribute))
                    {
                        //TODO: readonly too
                        if(field.IsPublic && field.IsStatic && field.Name.EndsWith("Event") && field.FieldType == typeof(int))
                            fields.Add(field);     
                        else
                            throw new System.NotSupportedException(String.Format("Field \"{0}.{1}\" is fail.", type, field.Name));
                    }   
                }
            }

            if(fields.Count > 0)
            {
                if(record.ParentType.ChildTypes.Count > 1)
                {
                    record.Events = new EventTable();
                    for(int i = 0, j = LastEventIndex(record); i < j; i++)
                        record.Events.Add(record.ParentType.Events[i]);
                }
            
                foreach(FieldInfo field in fields)
                {
                    int index = AddEvent(record, field.Name);
                    field.SetValue(null, index);
                }            
            }
/*
                              Console.WriteLine("{0} [{1}]", type, record.ParentType.CLRType);
                    foreach(KeyValuePair<string, int> event_record in record.Events)
                        Console.WriteLine("\t{0} = {1}", event_record.Key, event_record.Value);
*/

            return record;
        }

        private static int LastEventIndex(TypeRecord type_record)
        {
            int index = 0;
            TypeRecord parent = type_record.ParentType;
            
            while(parent != null)
            {
                index += parent.TypeEventCount;
                parent = parent.ParentType;
            }
            return index;
        }

        public static int AddEvent(TypeRecord type_record, string name)
        {
            int index = LastEventIndex(type_record) + type_record.TypeEventCount;
            type_record.Events.Add(new KeyValuePair<string, int>(name, index));
            type_record.TypeEventCount += 1;
            return index;
        } 
    }
}

