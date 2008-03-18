import thread

# This file is gonna be a really nasty mess.. :-P

class ThreadObject(object):
    def __init__(self):
        self._thread = thread.get_ident()
        
