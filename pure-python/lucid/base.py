""" """

#metatype needs: parent, list of child types, event table, event count
class _TypeRecord(object):
    __slots__ = ("parent", "children", "event_table", "event_count")

#Does this even need to be public?
class LucidType(type):

    def __init__(cls, name, bases, dct):
        cls._ltype = _TypeRecord()    

        # This is NOT safe (but base.Object doesn't exist yet..)      
        if object in bases:
            cls._ltype.parent = None
            cls._ltype.children = []
            cls._ltype.event_table = _EventTable()
            cls._ltype.event_count = 0    
        else:
            evts = ()
            try:
                evts = cls.__events__
            except AttributeError:
                pass
            for evt in evts:
                # add event 
                pass
    
        super(LucidType, cls).__init__(name, bases, dct)

class _EventTable(dict):    
    __slots__ = ("_table", )

    def __init__(self):
        self._table = []

    def add(self, name):
        if self.get(name):
            raise KeyError("Event name '%s' already exists" % (name, ))
        self._table.append(name)
        self[name] = len(self._table) - 1

    def get_index(self, name):
        try:
            return self[name]
        except KeyError:
            raise KeyError("Event name '%s' doesn't exist" % (name, ))

    def get_name(self, index):
        try:
            return self._table[index]        
        except IndexError:
            raise KeyError("Event index '%d' doesn't exist" % (index, ))

class Object(object):
    __metaclass__ = LucidType

    def __init__(self):
        pass

