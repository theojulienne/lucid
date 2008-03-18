import _enum

__all__ = ("Enum", )

# Hack so that our enum values are explicitly convertible to integers.
class IntEnumValue(_enum.EnumValue):
    def __int__(self):
        return self.index

        
_kwds = { "value_type" : IntEnumValue }        

def Enum(*keys):
    return _enum.Enum(*keys, **_kwds)        
