#include <boost/python.hpp>

using namespace boost::python;

class Foo: public object
{
public:
	object __next__();
	PyObject * __iter__();
};

object Foo::__next__()
{
	return object();
}

PyObject * Foo::__iter__()
{
	object next = attr("__next__"); //make_function(&Foo::__next__);
	return PyCallIter_New(next.ptr(), Py_None);
}

BOOST_PYTHON_MODULE(boost_py)
{
	class_<Foo>("Foo")
		.def("__next__", &Foo::__next__)
		.def("__iter__", &Foo::__iter__)	
	;
}

