from lucid.base import _EventTable, Object

class Bar(Object):
    __events__ = (("mooed", None), )
    pass

def main():
    t = _EventTable()
    t.add("foo")
    t.add("bar")
    print t.get_name(0)
    print len(t)

    o = Bar()
    print o
    print dir(Bar)

if __name__ == "__main__":
    main()

