import slist
    
#metatype needs: parent, list of child types, event table, event count
#FIXME: What is the optimum number of slots before going to a dict? eight? 
# Though slots provides us some more static behavior.
class _TypeRecord(object):
    __slots__ = ("parent", "children", "event_table", "event_count", "pytype")


def _last_evt_idx(rec):
    index = 0
    parent = rec.parent
    while parent is not None:
        index += parent.event_count
        parent = parent.parent
    return index    

def _parent_rec(rec):
    return rec.pytype.__mro__[1]._ltype     

def _split_rec(rec):
    rec.event_table = _EventTable()
    for i in xrange(_last_evt_idx(rec)):
        rec.event_table.add(rec.parent.event_table.get_name(i))

def _add_evt(rec, evt):
    index = _last_evt_idx(rec) + rec.event_count
    rec.event_table.add(evt[0], index)    
    rec.event_count += 1
    return index
    
#Does this even need to be public?
class LucidType(type):

    def __init__(cls, name, bases, dct):
        if len(bases) > 1:
            raise TypeError("Lucid doesn't currently support multiple inheritance")
        
        rec = cls._ltype = _TypeRecord()        
        rec.pytype = cls
        rec.event_count = 0    
        rec.children = None
        rec.parent = None
                 
        # This is NOT safe (but base.Object doesn't exist yet..)      
        if object not in bases:
            rec.parent = _parent_rec(rec)
            print "making %s [%s]" % (name, rec.parent.pytype)
            if not rec.parent.children:
                rec.parent.children = []
            rec.parent.children.append(rec)      
            rec.event_table = rec.parent.event_table
        else:
            rec.event_table = _EventTable()
        # ALERT: getattr fails hard because it will chain to MRO 
        evts = cls.__dict__.get("__levents__", ())
        if len(evts) > 0:
            if len(rec.parent.children) > 1:
                print "splitting %s" % (name, )
                _split_rec(rec)    
            for evt in evts:
                index = _add_evt(rec, evt)
                setattr(cls, evt[0], index)
        print "%s defines %d events [%x, %d]" % (name, len(evts), id(rec.event_table), len(rec.event_table))        
        for n in rec.event_table:
            print "\t%s" % (n, )
        super(LucidType, cls).__init__(name, bases, dct)


class _EventTable(dict):    
    __slots__ = ("_table", )

    def __init__(self):
        self._table = []

    def add(self, name, index_ = -1):
        if not self.get(name) is None:
            raise KeyError("Event name '%s' already exists" % (name, ))
        self._table.append(name)
        index = len(self._table) - 1
        self[name] = index
        print "_EventTable.add(): %s : %d" % (name, index)
        return index

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

    def __init__(self, parent = None):
        count = len(self.__class__._ltype.event_table)
        self._events = [None for i in xrange(count)]
        self._parent = parent
        self._children = None
    
    # Events    
    def add_handler(self, index, func):
        try:
            if isinstance(index, str):
                index = self.__class__._ltype.event_table[index]      
            n = slist.prepend(self._events[index], func)
            self._events[index] = n
        except Exception, ex:
            raise ex

    def remove_handler(self, index, func):
        try:
            if isinstance(index, str):
                index = self.__class__._ltype.event_table[index]   
            n = slist.remove(self._events[index], func)
            self._events[index] = n
        except Exception, ex:
            raise ex
            
    def emit(self, index, *args):
        try:
            if isinstance(index, str):
                index = self.__class__._ltype.event_table[index]   
            n = self._events[index]
            while n is not None:
                n.data(*args)
                n = n.next
        except Exception, ex:
            raise ex
    
    # Object Hierarchy
    # We use a linked-list for the children because sequential access isn't more
    # important than low memory usage. Notice that if this turns out to be false, 
    # we can easily switch back to an array. 
    
    @property
    def parent(self):
        return self._parent    

    def add_child(self, child):
        assert isinstance(child, Object)
        self._children = slist.prepend(self._children, child)

    def remove_child(self, child):
        assert isinstance(child, Object)
        self._children = slist.remove(self._children, child)
    
    def get_children(self):
        l = []
        n = self._children
        while n is not None:
            l.append(n.data)
            n = n.next
        return l
        
    def foreach_children(self, callback, *args):
        assert callable(callback)
        n = self._children
        l_args = list(args)
        l_args.insert(0, None)
        while n is not None:
            l_args[0] = n.data
            callback(*l_args)
            n = n.next
    
                                                       
