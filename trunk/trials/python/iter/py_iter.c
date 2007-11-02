#include <Python.h>

static PyObject * _iter_me(PyObject *self, PyObject *args)
{
	PyObject * obj;

	if (!PyArg_ParseTuple(args, "O", &obj))
        	return NULL;

	return PyCallIter_New(obj, Py_None);
}

static PyMethodDef py_iter_methods[] = 
{
	{ "iter_me",  _iter_me, METH_VARARGS, NULL },	
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
initpy_iter(void)
{
	PyObject *m;

	m = Py_InitModule("py_iter", py_iter_methods);
	if (m == NULL)
        	return;
}
