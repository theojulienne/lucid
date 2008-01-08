import vtest

def pybaz( inst, a, b ):
	inst.bar( a, b )
	inst.boo( a, b )
	inst.bee( a, b )

class PyFoot(vtest.Foo):
	def __init__(self):
		super(PyFoot, self).__init__()
	def bar( self, a, b ):
		print "Foot.bar(%s,%s) called in Python!" % (a,b)
	def boo( self, a, b ):
		print "Foot.boo(%s,%s) called in Python!" % (a,b)
	def bee( self, a, b ):
		print "Foot.bee(%s,%s) called in Python!" % (a,b)

def main():
	f = PyFoot( )
	vtest.baz( f, 3, 4 )
	pybaz( f, 1, 2 )

	g = vtest.makeAFoo( )
	vtest.baz( g, 5, 6 )
	pybaz( g, 7, 8 )

if __name__ == "__main__":
	main()

