import lucid
from lucid import base, io

class Foo(base.Object):
    __levents__ = (("mooed", None), )

    # less annoying formatting
    def __str__(self):
        cls_name = str(self.__class__)[17:-2]
        return "%s [0x%x]" % (cls_name, id(self), )


class Bar(Foo):
    __levents__ = (("blicked", None), )
    

def blick(sender, evt_name):
    print "blick(%s, %s)" % (sender, evt_name)

def enum_test():
    x = io.WatchEventKind.In | io.WatchEventKind.Out
    print x & io.WatchEventKind.In

def lobject_test():
    o = Bar()
    
    o.add_handler("mooed", blick)
    o.add_handler(o.mooed, blick)
    o.remove_handler("mooed", blick)
    
    o.add_handler("blicked", blick)
    
    o.emit("mooed", o, "mooed")
    o.emit("blicked", o, "blicked")
    
    o.add_child(Foo())
    o.add_child(Foo())
    o.foreach_children(blick, "foreach_children")

def loop_test():
    print io.get_loop()
     

def do_test(f):
    print "\n%s:\n" % (f.__name__, )
    f()

def main():
    do_test(enum_test)
    do_test(lobject_test)
    do_test(loop_test)
   
if __name__ == "__main__":
    main()

