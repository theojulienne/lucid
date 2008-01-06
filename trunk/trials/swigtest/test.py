import vtest

class Foot(vtest.Foo):
	def bar( self, a, b ):
		print "Foot.bar(%s,%s) called in Python!" % (a,b)


f = Foot( )
vtest.baz( f, 3, 4 )
