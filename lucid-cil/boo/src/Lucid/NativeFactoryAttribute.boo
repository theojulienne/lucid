namespace Lucid

import System


[AttributeUsage(AttributeTargets.Assembly)]
class NativeFactoryAttribute(Attribute):

    type as Type

    def constructor(type):
        self.type = type
        
    Type as Type:
        get:
            return self.type
