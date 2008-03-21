using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;

namespace Lucid
{
    using EventTable = KeyedValuePairCollection<string, int>;

    internal class KeyedValuePairCollection<TKey, TValue> : KeyedCollection<TKey, KeyValuePair<TKey, TValue>>
    {
        /// <summary>
        /// When implemented in a derived class, extracts the key from the specified element.
        /// </summary>
        /// <param name="item">The element from which to extract the key.</param>
        /// <returns>The key for the specified element.</returns>
        protected override TKey GetKeyForItem(KeyValuePair<TKey, TValue> item)
        {
            return item.Key;
        }
    }
     
    internal class TypeRecord
    {
        public TypeRecord(): this(new EventTable())
        {
        }

        public TypeRecord(EventTable table)
        {
            Events = table;
        }

        public int GetEventIndex(string name)
        {
            try
            {
                return Events[name].Value;
            }
            catch(KeyNotFoundException e)
            {
                throw e;
            }
        }

        public string GetEventName(int index)
        {
            if(Events.Count > index + 1)
                throw new ArgumentOutOfRangeException("index");

            try
            {            
                return Events[index].Key;
            }
            catch(ArgumentOutOfRangeException e)
            {
                throw e;
            }
        }
        
        public Type CLRType;
        public TypeRecord ParentType;
        //TODO: Redo as a linked list (?)
        public List<TypeRecord> ChildTypes;
        public EventTable Events;
        public int TypeEventCount;        
    }
}

