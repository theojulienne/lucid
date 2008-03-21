namespace Lucid

import System
import System.Collections
import System.Collections.Generic
import System.Collections.ObjectModel
import System.Reflection


abstract class EventObject(BaseObject):

    private m_events as (Delegate)
        
    def constructor():
        count = self.TypeRecord.Events.Count
        if count > 0:
            m_events = array(Delegate, count)
        else:
            m_events = null
            
    def AddHandler(event_name, handler)
        index = TypeManager.GetRecord(self.GetType()).GetEventIndex(event_name)  
        self.AddHandler(index, handler)

    def RemoveHandler(event_name, handler)
        index = TypeManager.GetRecord(self.GetType()).GetEventIndex(event_name)  
        self.RemoveHandler(index, handler)

    protected def AddHandler(index as int, handler)
        try:
            d = m_events[index]
            d = Delegate.Combine(d, handler)
            m_events[index] = d         
        except:
            raise
            
    # converted up to here
    
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

