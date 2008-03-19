from zope.interface import Interface, implements
from io import IEventLoop, HandleKind, WatchEventKind

import gobject

# FIXME: HACK HACK HACK
# This is all fundamentally broken because the current gobject module doesn't 
# actually expose the GMainLoop functions for instances besides the default. 

class GSource(object):
    __slots__ = ("_id", )
    
    def __init__(self, id):
        self._id = id


def _make_cond(events):
    cond = 0
    if WatchEventKind.In & events:
        cond |= gobject.IO_IN
    if WatchEventKind.Out & events:
        cond |= gobject.IO_OUT    
    if WatchEventKind.Err & events:
        cond |= gobject.IO_ERR
    if WatchEventKind.Hup & events:
        cond |= gobject.IO_HUP
    return cond
    
            
class GMainLoop(object):
    implements(IEventLoop)
    __slots__ = ("_loop", )

    def __init__(self):
        ctx = gobject.main_context_default()
        self._loop = gobject.MainLoop(ctx)
    
    def add_watch(handle, events, handler, args):
        assert handle.kind == HandleKind.FD
        cond = _make_cond(events)     
        id = gobject.io_add_watch(int(handle), cond, handler, *args)
        return GSource(id)

    def update_watch(source, events):
        assert isinstance(source, GSource)
        # FIXME

    def add_timeout(timeout, handler, args):
        pass#def _cb(f,*args):
        #    f(*args)
        #    return True                       
        #gobject.timeout_add(

    def remove_source(source): pass

    def run():
        self._loop.run()

    def run_iteration(block):
        return self._loop.get_context().iteration(may_block = block)

    def quit(self):
        self._loop.quit()

    def has_pending(self):
        return self._loop.get_context().pending()    


_loop = GMainLoop()

def get_loop():
    return _loop

