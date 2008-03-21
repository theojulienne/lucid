using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;

namespace Lucid
{
    public abstract class EventObject: BaseObject
	{
        private Delegate[] m_events = null;	
        
		public EventObject()
		{
            int count = TypeRecord.Events.Count;
            if(count > 0)
                m_events = new Delegate[count]; 
        }	
      
        //TODO: Public...?
        public void AddHandler(string event_name, Delegate handler)
        {
            int index = TypeManager.GetRecord(GetType()).GetEventIndex(event_name);  
            AddHandler(index, handler);
        }

        public void RemoveHandler(string event_name, Delegate handler)
        {
            int index = TypeManager.GetRecord(GetType()).GetEventIndex(event_name);  
            RemoveHandler(index, handler);
        }

        protected void AddHandler(int index, Delegate handler)
        {
            try
            {
                Delegate d = m_events[index];
                d = Delegate.Combine(d, handler);
                m_events[index] = d;            
            }
            catch(Exception e)
            {
                throw e;
            }
        }

        protected void RemoveHandler(int index, Delegate handler)
        {
            try
            {   
                Delegate d = m_events[index];
                d = Delegate.Remove(d, handler);
                m_events[index] = d;    
            }
            catch(Exception e)
            {
                throw e;
            }
        }

        public void Emit(string event_name, params object[] args)
        {
            int index = TypeManager.GetRecord(GetType()).GetEventIndex(event_name);
            EmitRaw(index, args);        
        }

        protected void Emit(int index, params object[] args)
        {
            EmitRaw(index, args);
        }

        protected void EmitRaw(int index, object[] args)
        {
            Delegate d = m_events[index];
            if(d != null)
                d.DynamicInvoke(args);
        }
	}
}

