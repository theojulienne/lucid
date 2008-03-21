import _enum

__all__ = ("Enum", "Flags")

# FIXME: Our flags system is still *terrible*..

def _get_flags_dict(keys):
    d = {}
    i = 0
    for k in keys:
        d[k] = 1 << i
        i += 1
    return d    

def _get_flag_val(flags, i, key):
    val = flags._flags[key]
    return IntEnumValue(flags, val, key)
        
class Flags(_enum.Enum):
    def __init__(self, *keys):
        object.__setattr__(self, "_flags", _get_flags_dict(keys))
        super(Flags, self).__init__(*keys, **_kwds)


# Hack so that our enum values are explicitly convertible to integers.
class IntEnumValue(_enum.EnumValue):
    def __int__(self):
        return self.index
    def __and__(self, other):
        return self.index & int(other)
    def __or__(self, other):
        return self.index | int(other)
    def __rand__(self, other):
        return self.__and__(other)
    def __ror__(self, other):
        return self.__or__(other)
        
        
_kwds = { "value_type" : IntEnumValue }        

def Enum(*keys):
    return _enum.Enum(*keys, **_kwds)        
