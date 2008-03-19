import slist

class Moo(object):
    def __del__(self):
        print self

def pprint(n):
    while n is not None:
        print n.data
        n = n.next
  
n = None

n = slist.prepend(n, Moo())
n = slist.prepend(n, Moo())

pprint(n)

#n = slist.remove(n, 1)
#n = slist.remove(n, 2)

#pprint(n)

