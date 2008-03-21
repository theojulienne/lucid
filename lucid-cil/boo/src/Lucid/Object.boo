using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;

namespace Lucid
{
    public class Object: EventObject
    {
        private Lucid.Object m_parent = null;
        //FIXME: This will actually be a custom class to handle parent/child semantics.
		private List<Lucid.Object> m_children = null;
        private bool m_disposed = false;

        public Object()
        {
        }
 
        public Object(Lucid.Object parent): this()
		{
			m_parent = parent;
		}
     
        //FIXME
        public virtual void Dispose()
        {
            if(m_disposed)
                return;
            
            GC.SuppressFinalize(this);            
            m_disposed = true;
        }
       
		public Lucid.Object Parent
		{
			get
			{
				return m_parent;
			}
		}

		public IList Children
		{
			get
			{
				if(m_children == null)
					m_children = new List<Lucid.Object>();
				return (m_children as IList);
			}
		}	
    }
}

