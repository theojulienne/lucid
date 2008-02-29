using System;

namespace Lucid
{
    [AttributeUsage(AttributeTargets.Assembly)]
    public class NativeFactoryAttribute: Attribute
    {
        private Type m_type;

        public NativeFactoryAttribute(Type type)
        {
            m_type = type;
        }

        public Type Type
        {
            get
            {
                return m_type;
            }
        }
    }
}

