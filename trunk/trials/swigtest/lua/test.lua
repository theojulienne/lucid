require "vtest"

function lbaz(f, a, b)
	f:bar(a, b)
	f:boo(a, b)
	f:bee(a, b)
end

function main()	
	g = vtest.makeAFoo()
	vtest.baz(g, 5, 6)
	lbaz(g, 7, 8)
end

main()
