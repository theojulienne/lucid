namespace Lucid

import System

//FIXME: This needs to have all the helper methods for threading support.
abstract class BaseObject(IDisposable):

    type_record as TypeRecord

    def constructor():
        self.type_record = TypeManager.GetRecord(GetType())

    virtual def Dispose():
        pass
        
    internal TypeRecord as TypeRecord:
        get:
            return self.type_record
