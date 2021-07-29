#define PY_SSIZE_T_CLEAN
#include <python.h>
#include "structmember.h" // needed for PyMemberMethod

typedef struct 
{
    PyObject_HEAD
    double x;
    double y;
}Vector;

static PyTypeObject Vector_type;
Vector* Vector_New(void);
#define Vector2d_CheckExact(ob) (Py_TYPE(ob) == &Vector_type)

static PyObject* vector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Vector *self;
    self = (Vector *) type->tp_alloc(type,0);
    if (self != NULL)
    {
        self->x = 0;
        self->y = 0;
    }
    return (PyObject*) self; // problem if self == NULL Right?
}
static int vector_init(Vector *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"x", "y"};
    if (!PyArg_ParseTupleAndKeywords(args, kwds,"|dd", kwlist, &self->x, &self->y))
        return -1;
    return 0;
}


static void vector_dealloc(Vector *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject* vector_GetX(Vector *self, void *closure)
{
    return Py_BuildValue("d", self->x);
}


static int vector_SetX(Vector *self, PyObject *value, void *closure)
{
    
    double x = PyFloat_AsDouble(value);
    if (PyErr_Occurred())
    {
        return -1;
    }
    self->x = x;
    return 0;
}

static PyObject* vector_GetY(Vector *self, void *closure)
{
    return Py_BuildValue("d", self->y);
}

static int vector_SetY(Vector *self, PyObject *value, void *closure)
{
    // add check to see if float first, then check for int.
    double y = PyFloat_AsDouble(value);
    if (PyErr_Occurred())
    {
        return -1;
    }
    self->y = y;
    return 0;
}

static PyObject* vector_GetLength(Vector *self, void *closure)
{
    double length = sqrt(self->x * self->x + self->y * self->y);
    return Py_BuildValue("d", length);
}

static int vector_SetLength(Vector *self, PyObject *length,  void *closure)
{
    double new_length = PyFloat_AsDouble(length);
    double old_length = sqrt(self->x * self->x + self->y * self->y);
    if (PyErr_Occurred())
    {
        return -1;
    }
    self->x = new_length / old_length * self->x;
    self->y = new_length / old_length * self->y;
    return 0;
}

static PyGetSetDef vector2d_GetSet[] = {
    {"x", (getter) vector_GetX, (setter) vector_SetX ,"Get/sets x", NULL},
    {"y", (getter) vector_GetY, (setter) vector_SetY, "Get/sets x", NULL},
    {"length", (getter) vector_GetLength, (setter) vector_SetLength, "Get/Set Length", NULL},
    {NULL}
};

/* wont work!
static PyObject* vector_math(PyObject *v, PyObject *w, PyObject* (*foo)(PyObject* v, PyObject* w)) 
{
    Vector *new_vector = NULL;
    double number;
    if (new_vector == NULL)
    {
        return NULL;
    }
    if (Vector2d_CheckExact(v) && Vector2d_CheckExact(w))
    {
        new_vector = foo(v, w) Should crate a new vector
        return (PyObject *) new_vector;
    }
    if (PyFloat_Check(v) || PyLong_Check(v))
    {
        PyObject* temp = v;
        v = w;
        w = temp;
    }
    /
    ==============Insert this into the Pynumb_vector_add============
    number = PyFloat_AsDouble(w);
    if (PyErr_Occurred())
    {
        Py_DECREF(new_vector);
        return NULL;
    }
    ================================================================
    *
    new_vector = foo(v , w);
    return (PyObject *) new_vector;
}
*/




static PyObject* Vector_add(PyObject *v, PyObject* w)
{
    double number;
    Vector *new_vector = Vector_New();
    if (new_vector == NULL)
    {
        /* raise exception */
        return NULL;
    }
    if (Vector2d_CheckExact(v) && Vector2d_CheckExact(w))
    {
        new_vector->x = ((Vector *) v)->x + ((Vector *) w)->x;
        new_vector->y = ((Vector *) v)->y + ((Vector *) w)->y;
        return (PyObject *) new_vector;
    }
    if (PyFloat_Check(v) || PyLong_Check(v))
    {
        PyObject* temp = v;
        v = w;
        w = temp;
    }
    number = PyFloat_AsDouble(w);
    if (PyErr_Occurred())
    {
        Py_DECREF(new_vector);
        return NULL;
    }
    new_vector->x = ((Vector *) v)->x + number;
    new_vector->y = ((Vector *) v)->y + number;

    return (PyObject *) new_vector;  
}
/* just do mathoperation(v,w, "operator")
and ad if checks, maybe test the speed */
static PyObject* Vector_element_times(PyObject* v, PyObject * w)
{
    Vector *new_vector = Vector_New();
    double number;
    if (new_vector == NULL)
    {
        /* raise exception */
        return NULL;
    }
    if (Vector2d_CheckExact(v) && Vector2d_CheckExact(w))
    {
        new_vector->x = ((Vector *) v)->x * ((Vector *) w)->x;
        new_vector->y = ((Vector *) v)->y * ((Vector *) w)->y;
        return (PyObject *) new_vector;
    }
    if (PyFloat_Check(v) || PyLong_Check(v))
    {
        PyObject* temp = v;
        v = w;
        w = temp;
    }
    number = PyFloat_AsDouble(w);
    if (PyErr_Occurred())
    {
        Py_DECREF(new_vector);
        return NULL;
    }
    new_vector->x = ((Vector *) v)->x * number;
    new_vector->y = ((Vector *) v)->y * number;

    return (PyObject *) new_vector;  
}



static PyNumberMethods vector_as_number= {
    .nb_add = Vector_add,
    .nb_multiply = Vector_element_times,
};

static PyTypeObject Vector_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "Vector2d.Vector",
    .tp_basicsize = sizeof(Vector),
    .tp_itemsize  = 0,
    .tp_dealloc   = (destructor) vector_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_as_number = &vector_as_number,
    //.tp_as_sequence,
    //.tp_as_mapping,
    .tp_getset    = vector2d_GetSet, 
    .tp_init      = (initproc) vector_init,
    .tp_new       = vector_new,
};
//write create new down here then define it in the header.

Vector* Vector_New()
{
    Vector *self;
    self = (Vector *) Vector_type.tp_alloc(&Vector_type, 0);
    if (self != NULL)
    {
        self->x = 0;
        self->y = 0;
    }
    Py_XINCREF(self);
    return self;
}


static PyObject* test_func(PyObject *self,PyObject *args, PyObject **kwards)
{
    PyObject *value1, *value2;
    if (!PyArg_ParseTuple(args,"OO", &value1, &value2))
        return NULL;
    if (PyType_IsSubtype(value1->ob_type, value2->ob_type))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyMethodDef test_methods[] = {
	{"ListAdd", (PyCFunction) test_func, METH_VARARGS,"..."},//read source for PyCFunction
	{NULL,NULL,0,NULL}
};


static PyModuleDef VectorMod = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Vector2d",
    .m_doc = "Rudematary 2d vecfor of c doubles.",
    .m_size = -1,
    .m_methods = test_methods,
};

PyMODINIT_FUNC
PyInit_Vector2d(void)
{
    PyObject *m;
    if (PyType_Ready(&Vector_type) < 0)
        return NULL;
    m = PyModule_Create(&VectorMod);
    if (m == NULL)
        return NULL;
    Py_INCREF(&Vector_type);
    PyModule_AddObject(m ,"Vector", (PyObject *) &Vector_type);
    return m;
}