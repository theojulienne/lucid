from zope.interface import Interface, implements
from enum import Enum

__all__ = ("WatchEventKind", "HandleKind", "WatchEvent", "IEventLoop")

### Interfaces

#FIXME: This actually needs to be some kind of 'bitflags' object..
WatchEventKind = Enum("None", "In", "Out", "Err", "Hup")
HandleKind = Enum("Invalid", "FD")

class OSHandle(object):
    __slots__ = ("handle", "kind")
    
    def __init__(self, handle = -1, kind = HandleKind.Invalid):
        self.handle = handle
        self.kind = kind
    
    def __int__(self):
        return self.handle
    

class WatchEvent(object):
    __slots__ = ("handle", "events")
    
    def __init__(self, handle, events):
        assert isinstance(handle, OSHandle)
        #FIXME: check events here
        self.handle = handle
        self.events = events
        
            
class IEventLoop(Interface):
    
    def add_watch(handle, events, handler, args): pass
    
    def update_watch(source, events): pass

    def add_timeout(timeout, handler, args): pass

    def remove_source(source): pass

    def run(): pass

    def run_iteration(block): pass

    def quit(): pass

    def has_pending(): pass


### Implementation Loading

try:
    from glib_impl import get_loop
except ImportError, ex:
    raise ex

