using System;

namespace Lucid
{
    //FIXME: This needs to have all the helper methods for threading support.
    public abstract class BaseObject
    {
        private TypeRecord m_type_record;

        public BaseObject()
        {
            m_type_record = TypeManager.GetRecord(GetType());
        }

        internal TypeRecord TypeRecord
        {
            get
            {
                return m_type_record;    
            }
        }
    }
}

