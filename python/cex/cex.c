#include "Python.h"

static PyObject * ex_foo(PyObject *self, PyObject *args)
{
  printf("Hello, world\n");
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef cex_methods[] = {
  {"foo", ex_foo, METH_VARARGS, "foo() doc string"},
  {NULL, NULL}
};

PyMODINIT_FUNC initcex(void)
{
  Py_InitModule("cex", cex_methods);
}
