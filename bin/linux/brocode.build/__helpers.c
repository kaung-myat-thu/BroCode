// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 1; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 1 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 1; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 1 * sizeof(PyObject *));
            memcpy(python_pars + 1, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 1)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 1 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 1; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 1 * sizeof(PyObject *));
                memcpy(python_pars+1 + 1, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 1)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 1);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 1, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 1);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 1);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 2; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 2 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 2; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 2 * sizeof(PyObject *));
            memcpy(python_pars + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 2)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 2 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 2; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 2 * sizeof(PyObject *));
                memcpy(python_pars+1 + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 2)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 2);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 2, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 2);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 2);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 3; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 3 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 3; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 3 * sizeof(PyObject *));
            memcpy(python_pars + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 3)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 3 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 3; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 3 * sizeof(PyObject *));
                memcpy(python_pars+1 + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 3)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 3);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 3, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 3);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 3);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 4; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 4 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 4; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 4 * sizeof(PyObject *));
            memcpy(python_pars + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 4)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 4 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 4; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 4 * sizeof(PyObject *));
                memcpy(python_pars+1 + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 4)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 4);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 4, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 4);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 4);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 5; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 5 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 5; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 5 * sizeof(PyObject *));
            memcpy(python_pars + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 5)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 5 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 5; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 5 * sizeof(PyObject *));
                memcpy(python_pars+1 + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 5)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 5);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 5, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 5);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 5);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1(PyObject *source, PyObject *attr_name, PyObject **args) {
    CHECK_OBJECT(source);
    CHECK_OBJECT(attr_name);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 1; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    PyTypeObject *type = Py_TYPE(source);

    if (type->tp_getattro == PyObject_GenericGetAttr) {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely(type->tp_dict == NULL)) {
            if (unlikely(PyType_Ready(type) < 0)) {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup(type, attr_name);
        descrgetfunc func = NULL;

        if (descr != NULL)
        {
            Py_INCREF(descr);

#if PYTHON_VERSION < 300
            if (PyType_HasFeature(Py_TYPE(descr), Py_TPFLAGS_HAVE_CLASS)) {
#endif
                func = Py_TYPE(descr)->tp_descr_get;

                if (func != NULL && PyDescr_IsData(descr))
                {
                    PyObject *called_object = func(descr, source, (PyObject *)type);
                    Py_DECREF(descr);

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF(called_object);
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if (dict != NULL)
        {
            CHECK_OBJECT(dict);

            Py_INCREF(dict);

            PyObject *called_object = PyDict_GetItem(dict, attr_name);

            if (called_object != NULL)
            {
                Py_INCREF(called_object);
                Py_XDECREF(descr);
                Py_DECREF(dict);

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF(called_object);
                return result;
            }

            Py_DECREF(dict);
        }

        if (func != NULL) {
            if (func == Nuitka_Function_Type.tp_descr_get) {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF(descr);

                return result;
            } else {
                PyObject *called_object = func(descr, source, (PyObject *)type);
                CHECK_OBJECT(called_object);

                Py_DECREF(descr);

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF(called_object);

                return result;
            }
        }

        if (descr != NULL) {
            CHECK_OBJECT(descr);

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
            Py_DECREF(descr);

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if (type == &PyInstance_Type) {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert(attr_name != const_str_plain___dict__);
        assert(attr_name != const_str_plain___class__);

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            return CALL_FUNCTION_WITH_ARGS1(called_object, args);
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            descrgetfunc descr_get = Py_TYPE(called_object)->tp_descr_get;

            if (descr_get == Nuitka_Function_Type.tp_descr_get) {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            } else if (descr_get != NULL) {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely(method == NULL)) {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(method, args);
                Py_DECREF(method);
                return result;
            } else {
                return CALL_FUNCTION_WITH_ARGS1(called_object, args);
            }

        } else if (unlikely(source_instance->in_class->cl_getattr == NULL)) {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        } else {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely(called_object == NULL))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF(called_object);
            return result;
        }
    }
#endif
    else if (type->tp_getattro != NULL) {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely(called_object == NULL)) {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else if (type->tp_getattr != NULL) {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked(attr_name)
        );

        if (unlikely(called_object == NULL))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked(attr_name)
        );

        return NULL;
    }
}

PyObject *CALL_METHOD_WITH_ARGS2(PyObject *source, PyObject *attr_name, PyObject **args) {
    CHECK_OBJECT(source);
    CHECK_OBJECT(attr_name);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 2; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    PyTypeObject *type = Py_TYPE(source);

    if (type->tp_getattro == PyObject_GenericGetAttr) {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely(type->tp_dict == NULL)) {
            if (unlikely(PyType_Ready(type) < 0)) {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup(type, attr_name);
        descrgetfunc func = NULL;

        if (descr != NULL)
        {
            Py_INCREF(descr);

#if PYTHON_VERSION < 300
            if (PyType_HasFeature(Py_TYPE(descr), Py_TPFLAGS_HAVE_CLASS)) {
#endif
                func = Py_TYPE(descr)->tp_descr_get;

                if (func != NULL && PyDescr_IsData(descr))
                {
                    PyObject *called_object = func(descr, source, (PyObject *)type);
                    Py_DECREF(descr);

                    PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                        called_object,
                        args
                    );
                    Py_DECREF(called_object);
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if (dict != NULL)
        {
            CHECK_OBJECT(dict);

            Py_INCREF(dict);

            PyObject *called_object = PyDict_GetItem(dict, attr_name);

            if (called_object != NULL)
            {
                Py_INCREF(called_object);
                Py_XDECREF(descr);
                Py_DECREF(dict);

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF(called_object);
                return result;
            }

            Py_DECREF(dict);
        }

        if (func != NULL) {
            if (func == Nuitka_Function_Type.tp_descr_get) {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    2
                );

                Py_DECREF(descr);

                return result;
            } else {
                PyObject *called_object = func(descr, source, (PyObject *)type);
                CHECK_OBJECT(called_object);

                Py_DECREF(descr);

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF(called_object);

                return result;
            }
        }

        if (descr != NULL) {
            CHECK_OBJECT(descr);

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                descr,
                args
            );
            Py_DECREF(descr);

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if (type == &PyInstance_Type) {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert(attr_name != const_str_plain___dict__);
        assert(attr_name != const_str_plain___class__);

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            return CALL_FUNCTION_WITH_ARGS2(called_object, args);
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            descrgetfunc descr_get = Py_TYPE(called_object)->tp_descr_get;

            if (descr_get == Nuitka_Function_Type.tp_descr_get) {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    2
                );
            } else if (descr_get != NULL) {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely(method == NULL)) {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(method, args);
                Py_DECREF(method);
                return result;
            } else {
                return CALL_FUNCTION_WITH_ARGS2(called_object, args);
            }

        } else if (unlikely(source_instance->in_class->cl_getattr == NULL)) {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        } else {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely(called_object == NULL))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                called_object,
                args
            );
            Py_DECREF(called_object);
            return result;
        }
    }
#endif
    else if (type->tp_getattro != NULL) {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely(called_object == NULL)) {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else if (type->tp_getattr != NULL) {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked(attr_name)
        );

        if (unlikely(called_object == NULL))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked(attr_name)
        );

        return NULL;
    }
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
extern PyObject *modulecode___main__(PyObject *);
extern PyObject *modulecode_art(PyObject *);
extern PyObject *modulecode_art$art(PyObject *);
extern PyObject *modulecode_art$art_dic(PyObject *);
extern PyObject *modulecode_art$art_param(PyObject *);
extern PyObject *modulecode_art$text_dic1(PyObject *);
extern PyObject *modulecode_art$text_dic2(PyObject *);
extern PyObject *modulecode_art$text_dic3(PyObject *);
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] = {
    {"__main__", modulecode___main__, 0, 0, },
    {"_bsddb", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_hotshot", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_json", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"art", modulecode_art, 0, 0, NUITKA_PACKAGE_FLAG},
    {"art.art", modulecode_art$art, 0, 0, },
    {"art.art_dic", modulecode_art$art_dic, 0, 0, },
    {"art.art_param", modulecode_art$art_param, 0, 0, },
    {"art.text_dic1", modulecode_art$text_dic1, 0, 0, },
    {"art.text_dic2", modulecode_art$text_dic2, 0, 0, },
    {"art.text_dic3", modulecode_art$text_dic3, 0, 0, },
    {"bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"codecs", NULL, 2809561, 36628, NUITKA_BYTECODE_FLAG},
    {"dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"os", NULL, 2846189, 25575, NUITKA_BYTECODE_FLAG},
    {"parser", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"random", NULL, 2871764, 25610, NUITKA_BYTECODE_FLAG},
    {"readline", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"resource", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"termios", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"types", NULL, 2897374, 2703, NUITKA_BYTECODE_FLAG},
    {"BaseHTTPServer", NULL, 2900077, 21674, NUITKA_BYTECODE_FLAG},
    {"Bastion", NULL, 2921751, 6622, NUITKA_BYTECODE_FLAG},
    {"CGIHTTPServer", NULL, 2928373, 10984, NUITKA_BYTECODE_FLAG},
    {"Canvas", NULL, 2939357, 15396, NUITKA_BYTECODE_FLAG},
    {"ConfigParser", NULL, 2954753, 25087, NUITKA_BYTECODE_FLAG},
    {"Cookie", NULL, 2979840, 22574, NUITKA_BYTECODE_FLAG},
    {"Dialog", NULL, 3002414, 1902, NUITKA_BYTECODE_FLAG},
    {"DocXMLRPCServer", NULL, 3004316, 10149, NUITKA_BYTECODE_FLAG},
    {"FileDialog", NULL, 3014465, 9666, NUITKA_BYTECODE_FLAG},
    {"FixTk", NULL, 3024131, 2078, NUITKA_BYTECODE_FLAG},
    {"HTMLParser", NULL, 3026209, 13655, NUITKA_BYTECODE_FLAG},
    {"MimeWriter", NULL, 3039864, 7338, NUITKA_BYTECODE_FLAG},
    {"Queue", NULL, 3047202, 9360, NUITKA_BYTECODE_FLAG},
    {"ScrolledText", NULL, 3056562, 2646, NUITKA_BYTECODE_FLAG},
    {"SimpleDialog", NULL, 3059208, 4309, NUITKA_BYTECODE_FLAG},
    {"SimpleHTTPServer", NULL, 3063517, 7980, NUITKA_BYTECODE_FLAG},
    {"SimpleXMLRPCServer", NULL, 3071497, 22783, NUITKA_BYTECODE_FLAG},
    {"SocketServer", NULL, 3094280, 23965, NUITKA_BYTECODE_FLAG},
    {"StringIO", NULL, 3118245, 11434, NUITKA_BYTECODE_FLAG},
    {"Tix", NULL, 3129679, 95416, NUITKA_BYTECODE_FLAG},
    {"Tkconstants", NULL, 3225095, 2236, NUITKA_BYTECODE_FLAG},
    {"Tkdnd", NULL, 3227331, 12765, NUITKA_BYTECODE_FLAG},
    {"Tkinter", NULL, 3240096, 199185, NUITKA_BYTECODE_FLAG},
    {"UserDict", NULL, 3439281, 9613, NUITKA_BYTECODE_FLAG},
    {"UserList", NULL, 3448894, 6501, NUITKA_BYTECODE_FLAG},
    {"UserString", NULL, 3455395, 14720, NUITKA_BYTECODE_FLAG},
    {"_LWPCookieJar", NULL, 3470115, 5414, NUITKA_BYTECODE_FLAG},
    {"_MozillaCookieJar", NULL, 3475529, 4445, NUITKA_BYTECODE_FLAG},
    {"__future__", NULL, 3479974, 4203, NUITKA_BYTECODE_FLAG},
    {"_abcoll", NULL, 3484177, 25466, NUITKA_BYTECODE_FLAG},
    {"_osx_support", NULL, 3509643, 11712, NUITKA_BYTECODE_FLAG},
    {"_pyio", NULL, 3521355, 64343, NUITKA_BYTECODE_FLAG},
    {"_strptime", NULL, 3585698, 15124, NUITKA_BYTECODE_FLAG},
    {"_sysconfigdata", NULL, 3600822, 271, NUITKA_BYTECODE_FLAG},
    {"_sysconfigdata_nd", NULL, 3601093, 20788, NUITKA_BYTECODE_FLAG},
    {"_threading_local", NULL, 3621881, 6347, NUITKA_BYTECODE_FLAG},
    {"_weakrefset", NULL, 3628228, 9574, NUITKA_BYTECODE_FLAG},
    {"abc", NULL, 3637802, 6113, NUITKA_BYTECODE_FLAG},
    {"aifc", NULL, 3643915, 30305, NUITKA_BYTECODE_FLAG},
    {"anydbm", NULL, 3674220, 2786, NUITKA_BYTECODE_FLAG},
    {"argparse", NULL, 3677006, 64049, NUITKA_BYTECODE_FLAG},
    {"ast", NULL, 3741055, 12882, NUITKA_BYTECODE_FLAG},
    {"asynchat", NULL, 3753937, 8742, NUITKA_BYTECODE_FLAG},
    {"asyncore", NULL, 3762679, 18767, NUITKA_BYTECODE_FLAG},
    {"atexit", NULL, 3781446, 2183, NUITKA_BYTECODE_FLAG},
    {"audiodev", NULL, 3783629, 8407, NUITKA_BYTECODE_FLAG},
    {"base64", NULL, 3792036, 11253, NUITKA_BYTECODE_FLAG},
    {"bdb", NULL, 3803289, 18971, NUITKA_BYTECODE_FLAG},
    {"binhex", NULL, 3822260, 15350, NUITKA_BYTECODE_FLAG},
    {"bisect", NULL, 3837610, 3053, NUITKA_BYTECODE_FLAG},
    {"bsddb", NULL, 3840663, 12368, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"bsddb.db", NULL, 3853031, 582, NUITKA_BYTECODE_FLAG},
    {"bsddb.dbobj", NULL, 3853613, 18601, NUITKA_BYTECODE_FLAG},
    {"bsddb.dbrecio", NULL, 3872214, 5269, NUITKA_BYTECODE_FLAG},
    {"bsddb.dbshelve", NULL, 3877483, 12910, NUITKA_BYTECODE_FLAG},
    {"bsddb.dbtables", NULL, 3890393, 24402, NUITKA_BYTECODE_FLAG},
    {"bsddb.dbutils", NULL, 3914795, 1615, NUITKA_BYTECODE_FLAG},
    {"cProfile", NULL, 3916410, 6359, NUITKA_BYTECODE_FLAG},
    {"calendar", NULL, 3922769, 27747, NUITKA_BYTECODE_FLAG},
    {"cgi", NULL, 3950516, 32976, NUITKA_BYTECODE_FLAG},
    {"cgitb", NULL, 3983492, 12094, NUITKA_BYTECODE_FLAG},
    {"chunk", NULL, 3995586, 5572, NUITKA_BYTECODE_FLAG},
    {"cmd", NULL, 4001158, 13989, NUITKA_BYTECODE_FLAG},
    {"code", NULL, 4015147, 10294, NUITKA_BYTECODE_FLAG},
    {"codecs", NULL, 2809561, 36628, NUITKA_BYTECODE_FLAG},
    {"codeop", NULL, 4025441, 6569, NUITKA_BYTECODE_FLAG},
    {"collections", NULL, 4032010, 26051, NUITKA_BYTECODE_FLAG},
    {"colorsys", NULL, 4058061, 3967, NUITKA_BYTECODE_FLAG},
    {"commands", NULL, 4062028, 2449, NUITKA_BYTECODE_FLAG},
    {"compileall", NULL, 4064477, 6997, NUITKA_BYTECODE_FLAG},
    {"compiler", NULL, 4071474, 1287, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"compiler.ast", NULL, 4072761, 71339, NUITKA_BYTECODE_FLAG},
    {"compiler.consts", NULL, 4144100, 727, NUITKA_BYTECODE_FLAG},
    {"compiler.future", NULL, 4144827, 2928, NUITKA_BYTECODE_FLAG},
    {"compiler.misc", NULL, 4147755, 3687, NUITKA_BYTECODE_FLAG},
    {"compiler.pyassem", NULL, 4151442, 25783, NUITKA_BYTECODE_FLAG},
    {"compiler.pycodegen", NULL, 4177225, 56161, NUITKA_BYTECODE_FLAG},
    {"compiler.symbols", NULL, 4233386, 17557, NUITKA_BYTECODE_FLAG},
    {"compiler.syntax", NULL, 4250943, 1862, NUITKA_BYTECODE_FLAG},
    {"compiler.transformer", NULL, 4252805, 47387, NUITKA_BYTECODE_FLAG},
    {"compiler.visitor", NULL, 4300192, 4159, NUITKA_BYTECODE_FLAG},
    {"contextlib", NULL, 4304351, 4422, NUITKA_BYTECODE_FLAG},
    {"cookielib", NULL, 4308773, 54475, NUITKA_BYTECODE_FLAG},
    {"copy", NULL, 4363248, 12110, NUITKA_BYTECODE_FLAG},
    {"copy_reg", NULL, 4375358, 5137, NUITKA_BYTECODE_FLAG},
    {"csv", NULL, 4380495, 13441, NUITKA_BYTECODE_FLAG},
    {"ctypes", NULL, 4393936, 20224, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"ctypes._endian", NULL, 4414160, 2287, NUITKA_BYTECODE_FLAG},
    {"ctypes.util", NULL, 4416447, 8518, NUITKA_BYTECODE_FLAG},
    {"curses", NULL, 4424965, 1539, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"curses.ascii", NULL, 4426504, 5085, NUITKA_BYTECODE_FLAG},
    {"curses.has_key", NULL, 4431589, 5928, NUITKA_BYTECODE_FLAG},
    {"curses.panel", NULL, 4437517, 267, NUITKA_BYTECODE_FLAG},
    {"curses.textpad", NULL, 4437784, 7121, NUITKA_BYTECODE_FLAG},
    {"curses.wrapper", NULL, 4444905, 1206, NUITKA_BYTECODE_FLAG},
    {"dbhash", NULL, 4446111, 706, NUITKA_BYTECODE_FLAG},
    {"decimal", NULL, 4446817, 171637, NUITKA_BYTECODE_FLAG},
    {"difflib", NULL, 4618454, 61782, NUITKA_BYTECODE_FLAG},
    {"dircache", NULL, 4680236, 1560, NUITKA_BYTECODE_FLAG},
    {"dis", NULL, 4681796, 6204, NUITKA_BYTECODE_FLAG},
    {"distutils", NULL, 4688000, 405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"distutils.archive_util", NULL, 4688405, 7577, NUITKA_BYTECODE_FLAG},
    {"distutils.bcppcompiler", NULL, 4695982, 7856, NUITKA_BYTECODE_FLAG},
    {"distutils.ccompiler", NULL, 4703838, 36764, NUITKA_BYTECODE_FLAG},
    {"distutils.cmd", NULL, 4740602, 16722, NUITKA_BYTECODE_FLAG},
    {"distutils.command", NULL, 4757324, 655, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"distutils.command.bdist", NULL, 4757979, 5149, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_dumb", NULL, 4763128, 4965, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_msi", NULL, 4768093, 23919, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_rpm", NULL, 4792012, 17582, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_wininst", NULL, 4809594, 10882, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build", NULL, 4820476, 5124, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_clib", NULL, 4825600, 6319, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_ext", NULL, 4831919, 19253, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_py", NULL, 4851172, 11443, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_scripts", NULL, 4862615, 4455, NUITKA_BYTECODE_FLAG},
    {"distutils.command.check", NULL, 4867070, 6230, NUITKA_BYTECODE_FLAG},
    {"distutils.command.clean", NULL, 4873300, 3051, NUITKA_BYTECODE_FLAG},
    {"distutils.command.config", NULL, 4876351, 12637, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install", NULL, 4888988, 17884, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_data", NULL, 4906872, 3096, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_egg_info", NULL, 4909968, 4345, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_headers", NULL, 4914313, 2233, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_lib", NULL, 4916546, 6649, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_scripts", NULL, 4923195, 2906, NUITKA_BYTECODE_FLAG},
    {"distutils.command.register", NULL, 4926101, 10174, NUITKA_BYTECODE_FLAG},
    {"distutils.command.sdist", NULL, 4936275, 16652, NUITKA_BYTECODE_FLAG},
    {"distutils.command.upload", NULL, 4952927, 6288, NUITKA_BYTECODE_FLAG},
    {"distutils.config", NULL, 4959215, 3543, NUITKA_BYTECODE_FLAG},
    {"distutils.core", NULL, 4962758, 7520, NUITKA_BYTECODE_FLAG},
    {"distutils.cygwinccompiler", NULL, 4970278, 9793, NUITKA_BYTECODE_FLAG},
    {"distutils.debug", NULL, 4980071, 244, NUITKA_BYTECODE_FLAG},
    {"distutils.dep_util", NULL, 4980315, 3164, NUITKA_BYTECODE_FLAG},
    {"distutils.dir_util", NULL, 4983479, 6764, NUITKA_BYTECODE_FLAG},
    {"distutils.dist", NULL, 4990243, 39034, NUITKA_BYTECODE_FLAG},
    {"distutils.emxccompiler", NULL, 5029277, 7441, NUITKA_BYTECODE_FLAG},
    {"distutils.errors", NULL, 5036718, 6237, NUITKA_BYTECODE_FLAG},
    {"distutils.extension", NULL, 5042955, 7396, NUITKA_BYTECODE_FLAG},
    {"distutils.fancy_getopt", NULL, 5050351, 11908, NUITKA_BYTECODE_FLAG},
    {"distutils.file_util", NULL, 5062259, 6732, NUITKA_BYTECODE_FLAG},
    {"distutils.filelist", NULL, 5068991, 10714, NUITKA_BYTECODE_FLAG},
    {"distutils.log", NULL, 5079705, 2754, NUITKA_BYTECODE_FLAG},
    {"distutils.msvc9compiler", NULL, 5082459, 21427, NUITKA_BYTECODE_FLAG},
    {"distutils.msvccompiler", NULL, 5103886, 17465, NUITKA_BYTECODE_FLAG},
    {"distutils.spawn", NULL, 5121351, 6405, NUITKA_BYTECODE_FLAG},
    {"distutils.sysconfig", NULL, 5127756, 14893, NUITKA_BYTECODE_FLAG},
    {"distutils.text_file", NULL, 5142649, 9229, NUITKA_BYTECODE_FLAG},
    {"distutils.unixccompiler", NULL, 5151878, 8087, NUITKA_BYTECODE_FLAG},
    {"distutils.util", NULL, 5159965, 14350, NUITKA_BYTECODE_FLAG},
    {"distutils.version", NULL, 5174315, 7170, NUITKA_BYTECODE_FLAG},
    {"distutils.versionpredicate", NULL, 5181485, 5520, NUITKA_BYTECODE_FLAG},
    {"doctest", NULL, 5187005, 83387, NUITKA_BYTECODE_FLAG},
    {"dumbdbm", NULL, 5270392, 6700, NUITKA_BYTECODE_FLAG},
    {"dummy_thread", NULL, 5277092, 5356, NUITKA_BYTECODE_FLAG},
    {"dummy_threading", NULL, 5282448, 1275, NUITKA_BYTECODE_FLAG},
    {"email", NULL, 5283723, 2852, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"email._parseaddr", NULL, 5286575, 13831, NUITKA_BYTECODE_FLAG},
    {"email.base64mime", NULL, 5300406, 5305, NUITKA_BYTECODE_FLAG},
    {"email.charset", NULL, 5305711, 13499, NUITKA_BYTECODE_FLAG},
    {"email.encoders", NULL, 5319210, 2210, NUITKA_BYTECODE_FLAG},
    {"email.errors", NULL, 5321420, 3491, NUITKA_BYTECODE_FLAG},
    {"email.feedparser", NULL, 5324911, 11089, NUITKA_BYTECODE_FLAG},
    {"email.generator", NULL, 5336000, 10334, NUITKA_BYTECODE_FLAG},
    {"email.header", NULL, 5346334, 13622, NUITKA_BYTECODE_FLAG},
    {"email.iterators", NULL, 5359956, 2348, NUITKA_BYTECODE_FLAG},
    {"email.message", NULL, 5362304, 28576, NUITKA_BYTECODE_FLAG},
    {"email.mime", NULL, 5390880, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"email.mime.application", NULL, 5391000, 1570, NUITKA_BYTECODE_FLAG},
    {"email.mime.audio", NULL, 5392570, 2893, NUITKA_BYTECODE_FLAG},
    {"email.mime.base", NULL, 5395463, 1102, NUITKA_BYTECODE_FLAG},
    {"email.mime.image", NULL, 5396565, 2035, NUITKA_BYTECODE_FLAG},
    {"email.mime.message", NULL, 5398600, 1434, NUITKA_BYTECODE_FLAG},
    {"email.mime.multipart", NULL, 5400034, 1655, NUITKA_BYTECODE_FLAG},
    {"email.mime.nonmultipart", NULL, 5401689, 874, NUITKA_BYTECODE_FLAG},
    {"email.mime.text", NULL, 5402563, 1294, NUITKA_BYTECODE_FLAG},
    {"email.parser", NULL, 5403857, 3804, NUITKA_BYTECODE_FLAG},
    {"email.quoprimime", NULL, 5407661, 8816, NUITKA_BYTECODE_FLAG},
    {"email.utils", NULL, 5416477, 9284, NUITKA_BYTECODE_FLAG},
    {"encodings", NULL, 5425761, 4362, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"encodings.aliases", NULL, 5430123, 8760, NUITKA_BYTECODE_FLAG},
    {"encodings.ascii", NULL, 5438883, 2253, NUITKA_BYTECODE_FLAG},
    {"encodings.base64_codec", NULL, 5441136, 3829, NUITKA_BYTECODE_FLAG},
    {"encodings.big5", NULL, 5444965, 1748, NUITKA_BYTECODE_FLAG},
    {"encodings.big5hkscs", NULL, 5446713, 1788, NUITKA_BYTECODE_FLAG},
    {"encodings.bz2_codec", NULL, 5448501, 4721, NUITKA_BYTECODE_FLAG},
    {"encodings.charmap", NULL, 5453222, 3465, NUITKA_BYTECODE_FLAG},
    {"encodings.cp037", NULL, 5456687, 2829, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1006", NULL, 5459516, 2915, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1026", NULL, 5462431, 2843, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1140", NULL, 5465274, 2829, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1250", NULL, 5468103, 2866, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1251", NULL, 5470969, 2863, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1252", NULL, 5473832, 2866, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1253", NULL, 5476698, 2879, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1254", NULL, 5479577, 2868, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1255", NULL, 5482445, 2887, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1256", NULL, 5485332, 2865, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1257", NULL, 5488197, 2873, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1258", NULL, 5491070, 2871, NUITKA_BYTECODE_FLAG},
    {"encodings.cp424", NULL, 5493941, 2859, NUITKA_BYTECODE_FLAG},
    {"encodings.cp437", NULL, 5496800, 8064, NUITKA_BYTECODE_FLAG},
    {"encodings.cp500", NULL, 5504864, 2829, NUITKA_BYTECODE_FLAG},
    {"encodings.cp720", NULL, 5507693, 2926, NUITKA_BYTECODE_FLAG},
    {"encodings.cp737", NULL, 5510619, 8292, NUITKA_BYTECODE_FLAG},
    {"encodings.cp775", NULL, 5518911, 8078, NUITKA_BYTECODE_FLAG},
    {"encodings.cp850", NULL, 5526989, 7811, NUITKA_BYTECODE_FLAG},
    {"encodings.cp852", NULL, 5534800, 8080, NUITKA_BYTECODE_FLAG},
    {"encodings.cp855", NULL, 5542880, 8261, NUITKA_BYTECODE_FLAG},
    {"encodings.cp856", NULL, 5551141, 2891, NUITKA_BYTECODE_FLAG},
    {"encodings.cp857", NULL, 5554032, 7801, NUITKA_BYTECODE_FLAG},
    {"encodings.cp858", NULL, 5561833, 7781, NUITKA_BYTECODE_FLAG},
    {"encodings.cp860", NULL, 5569614, 8047, NUITKA_BYTECODE_FLAG},
    {"encodings.cp861", NULL, 5577661, 8058, NUITKA_BYTECODE_FLAG},
    {"encodings.cp862", NULL, 5585719, 8193, NUITKA_BYTECODE_FLAG},
    {"encodings.cp863", NULL, 5593912, 8058, NUITKA_BYTECODE_FLAG},
    {"encodings.cp864", NULL, 5601970, 8189, NUITKA_BYTECODE_FLAG},
    {"encodings.cp865", NULL, 5610159, 8058, NUITKA_BYTECODE_FLAG},
    {"encodings.cp866", NULL, 5618217, 8293, NUITKA_BYTECODE_FLAG},
    {"encodings.cp869", NULL, 5626510, 8105, NUITKA_BYTECODE_FLAG},
    {"encodings.cp874", NULL, 5634615, 2957, NUITKA_BYTECODE_FLAG},
    {"encodings.cp875", NULL, 5637572, 2826, NUITKA_BYTECODE_FLAG},
    {"encodings.cp932", NULL, 5640398, 1756, NUITKA_BYTECODE_FLAG},
    {"encodings.cp949", NULL, 5642154, 1756, NUITKA_BYTECODE_FLAG},
    {"encodings.cp950", NULL, 5643910, 1756, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jis_2004", NULL, 5645666, 1812, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jisx0213", NULL, 5647478, 1812, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jp", NULL, 5649290, 1764, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_kr", NULL, 5651054, 1764, NUITKA_BYTECODE_FLAG},
    {"encodings.gb18030", NULL, 5652818, 1772, NUITKA_BYTECODE_FLAG},
    {"encodings.gb2312", NULL, 5654590, 1764, NUITKA_BYTECODE_FLAG},
    {"encodings.gbk", NULL, 5656354, 1740, NUITKA_BYTECODE_FLAG},
    {"encodings.hex_codec", NULL, 5658094, 3781, NUITKA_BYTECODE_FLAG},
    {"encodings.hp_roman8", NULL, 5661875, 4112, NUITKA_BYTECODE_FLAG},
    {"encodings.hz", NULL, 5665987, 1732, NUITKA_BYTECODE_FLAG},
    {"encodings.idna", NULL, 5667719, 6368, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp", NULL, 5674087, 1801, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_1", NULL, 5675888, 1817, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_2", NULL, 5677705, 1817, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_2004", NULL, 5679522, 1841, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_3", NULL, 5681363, 1817, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_ext", NULL, 5683180, 1833, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_kr", NULL, 5685013, 1801, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_1", NULL, 5686814, 2868, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_10", NULL, 5689682, 2883, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_11", NULL, 5692565, 2977, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_13", NULL, 5695542, 2886, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_14", NULL, 5698428, 2904, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_15", NULL, 5701332, 2883, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_16", NULL, 5704215, 2885, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_2", NULL, 5707100, 2868, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_3", NULL, 5709968, 2875, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_4", NULL, 5712843, 2868, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_5", NULL, 5715711, 2869, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_6", NULL, 5718580, 2913, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_7", NULL, 5721493, 2876, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_8", NULL, 5724369, 2907, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_9", NULL, 5727276, 2868, NUITKA_BYTECODE_FLAG},
    {"encodings.johab", NULL, 5730144, 1756, NUITKA_BYTECODE_FLAG},
    {"encodings.koi8_r", NULL, 5731900, 2890, NUITKA_BYTECODE_FLAG},
    {"encodings.koi8_u", NULL, 5734790, 2876, NUITKA_BYTECODE_FLAG},
    {"encodings.latin_1", NULL, 5737666, 2283, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_arabic", NULL, 5739949, 8014, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_centeuro", NULL, 5747963, 2937, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_croatian", NULL, 5750900, 2945, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_cyrillic", NULL, 5753845, 2935, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_farsi", NULL, 5756780, 2849, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_greek", NULL, 5759629, 2889, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_iceland", NULL, 5762518, 2928, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_latin2", NULL, 5765446, 4907, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_roman", NULL, 5770353, 2906, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_romanian", NULL, 5773259, 2946, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_turkish", NULL, 5776205, 2929, NUITKA_BYTECODE_FLAG},
    {"encodings.mbcs", NULL, 5779134, 2019, NUITKA_BYTECODE_FLAG},
    {"encodings.palmos", NULL, 5781153, 3066, NUITKA_BYTECODE_FLAG},
    {"encodings.ptcp154", NULL, 5784219, 4890, NUITKA_BYTECODE_FLAG},
    {"encodings.punycode", NULL, 5789109, 7942, NUITKA_BYTECODE_FLAG},
    {"encodings.quopri_codec", NULL, 5797051, 3647, NUITKA_BYTECODE_FLAG},
    {"encodings.raw_unicode_escape", NULL, 5800698, 2202, NUITKA_BYTECODE_FLAG},
    {"encodings.rot_13", NULL, 5802900, 3656, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jis", NULL, 5806556, 1788, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jis_2004", NULL, 5808344, 1828, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jisx0213", NULL, 5810172, 1828, NUITKA_BYTECODE_FLAG},
    {"encodings.string_escape", NULL, 5812000, 2061, NUITKA_BYTECODE_FLAG},
    {"encodings.tis_620", NULL, 5814061, 2938, NUITKA_BYTECODE_FLAG},
    {"encodings.undefined", NULL, 5816999, 2589, NUITKA_BYTECODE_FLAG},
    {"encodings.unicode_escape", NULL, 5819588, 2150, NUITKA_BYTECODE_FLAG},
    {"encodings.unicode_internal", NULL, 5821738, 2176, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16", NULL, 5823914, 5160, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16_be", NULL, 5829074, 1990, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16_le", NULL, 5831064, 1990, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32", NULL, 5833054, 5724, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32_be", NULL, 5838778, 1883, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32_le", NULL, 5840661, 1883, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_7", NULL, 5842544, 1883, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_8", NULL, 5844427, 1942, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_8_sig", NULL, 5846369, 4977, NUITKA_BYTECODE_FLAG},
    {"encodings.uu_codec", NULL, 5851346, 4909, NUITKA_BYTECODE_FLAG},
    {"encodings.zlib_codec", NULL, 5856255, 4641, NUITKA_BYTECODE_FLAG},
    {"filecmp", NULL, 5860896, 9574, NUITKA_BYTECODE_FLAG},
    {"fileinput", NULL, 5870470, 14432, NUITKA_BYTECODE_FLAG},
    {"fnmatch", NULL, 5884902, 3594, NUITKA_BYTECODE_FLAG},
    {"formatter", NULL, 5888496, 19016, NUITKA_BYTECODE_FLAG},
    {"fpformat", NULL, 5907512, 4679, NUITKA_BYTECODE_FLAG},
    {"fractions", NULL, 5912191, 19625, NUITKA_BYTECODE_FLAG},
    {"ftplib", NULL, 5931816, 34609, NUITKA_BYTECODE_FLAG},
    {"functools", NULL, 5966425, 6561, NUITKA_BYTECODE_FLAG},
    {"genericpath", NULL, 5972986, 3487, NUITKA_BYTECODE_FLAG},
    {"getopt", NULL, 5976473, 6626, NUITKA_BYTECODE_FLAG},
    {"getpass", NULL, 5983099, 4722, NUITKA_BYTECODE_FLAG},
    {"gettext", NULL, 5987821, 17858, NUITKA_BYTECODE_FLAG},
    {"glob", NULL, 6005679, 2919, NUITKA_BYTECODE_FLAG},
    {"gzip", NULL, 6008598, 15168, NUITKA_BYTECODE_FLAG},
    {"hashlib", NULL, 6023766, 6883, NUITKA_BYTECODE_FLAG},
    {"heapq", NULL, 6030649, 14520, NUITKA_BYTECODE_FLAG},
    {"hmac", NULL, 6045169, 4514, NUITKA_BYTECODE_FLAG},
    {"hotshot", NULL, 6049683, 3454, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"hotshot.log", NULL, 6053137, 5507, NUITKA_BYTECODE_FLAG},
    {"hotshot.stats", NULL, 6058644, 3374, NUITKA_BYTECODE_FLAG},
    {"hotshot.stones", NULL, 6062018, 1144, NUITKA_BYTECODE_FLAG},
    {"htmlentitydefs", NULL, 6063162, 6357, NUITKA_BYTECODE_FLAG},
    {"htmllib", NULL, 6069519, 20119, NUITKA_BYTECODE_FLAG},
    {"httplib", NULL, 6089638, 37189, NUITKA_BYTECODE_FLAG},
    {"ihooks", NULL, 6126827, 21226, NUITKA_BYTECODE_FLAG},
    {"imaplib", NULL, 6148053, 44809, NUITKA_BYTECODE_FLAG},
    {"imghdr", NULL, 6192862, 4798, NUITKA_BYTECODE_FLAG},
    {"importlib", NULL, 6197660, 1488, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"imputil", NULL, 6199148, 15539, NUITKA_BYTECODE_FLAG},
    {"inspect", NULL, 6214687, 40083, NUITKA_BYTECODE_FLAG},
    {"io", NULL, 6254770, 3571, NUITKA_BYTECODE_FLAG},
    {"json", NULL, 6258341, 13908, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"json.decoder", NULL, 6272249, 11928, NUITKA_BYTECODE_FLAG},
    {"json.encoder", NULL, 6284177, 13679, NUITKA_BYTECODE_FLAG},
    {"json.scanner", NULL, 6297856, 2215, NUITKA_BYTECODE_FLAG},
    {"json.tool", NULL, 6300071, 1282, NUITKA_BYTECODE_FLAG},
    {"keyword", NULL, 6301353, 2093, NUITKA_BYTECODE_FLAG},
    {"lib2to3", NULL, 6303446, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.btm_matcher", NULL, 6303563, 5800, NUITKA_BYTECODE_FLAG},
    {"lib2to3.btm_utils", NULL, 6309363, 7529, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixer_base", NULL, 6316892, 7146, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixer_util", NULL, 6324038, 14607, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes", NULL, 6338645, 123, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.fixes.fix_apply", NULL, 6338768, 2070, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_asserts", NULL, 6340838, 1547, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_basestring", NULL, 6342385, 793, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_buffer", NULL, 6343178, 950, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_dict", NULL, 6344128, 3753, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_except", NULL, 6347881, 2993, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_exec", NULL, 6350874, 1418, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_execfile", NULL, 6352292, 2094, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_exitfunc", NULL, 6354386, 2739, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_filter", NULL, 6357125, 2256, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_funcattrs", NULL, 6359381, 1114, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_future", NULL, 6360495, 919, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_getcwdu", NULL, 6361414, 926, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_has_key", NULL, 6362340, 3184, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_idioms", NULL, 6365524, 4515, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_import", NULL, 6370039, 3233, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_imports", NULL, 6373272, 5352, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_imports2", NULL, 6378624, 622, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_input", NULL, 6379246, 1134, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_intern", NULL, 6380380, 1783, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_isinstance", NULL, 6382163, 1838, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_itertools", NULL, 6384001, 1791, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_itertools_imports", NULL, 6385792, 2016, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_long", NULL, 6387808, 841, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_map", NULL, 6388649, 3040, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_metaclass", NULL, 6391689, 6579, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_methodattrs", NULL, 6398268, 1138, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_ne", NULL, 6399406, 985, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_next", NULL, 6400391, 3531, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_nonzero", NULL, 6403922, 1086, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_numliterals", NULL, 6405008, 1249, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_operator", NULL, 6406257, 5112, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_paren", NULL, 6411369, 1543, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_print", NULL, 6412912, 2727, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_raise", NULL, 6415639, 2498, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_raw_input", NULL, 6418137, 936, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_reduce", NULL, 6419073, 1262, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_renames", NULL, 6420335, 2449, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_repr", NULL, 6422784, 1016, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_set_literal", NULL, 6423800, 1988, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_standarderror", NULL, 6425788, 853, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_sys_exc", NULL, 6426641, 1705, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_throw", NULL, 6428346, 1996, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_tuple_params", NULL, 6430342, 5430, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_types", NULL, 6435772, 2196, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_unicode", NULL, 6437968, 1716, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_urllib", NULL, 6439684, 7134, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_ws_comma", NULL, 6446818, 1382, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_xrange", NULL, 6448200, 3063, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_xreadlines", NULL, 6451263, 1152, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_zip", NULL, 6452415, 1346, NUITKA_BYTECODE_FLAG},
    {"lib2to3.main", NULL, 6453761, 9803, NUITKA_BYTECODE_FLAG},
    {"lib2to3.patcomp", NULL, 6463564, 6569, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2", NULL, 6470133, 164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.pgen2.conv", NULL, 6470297, 8169, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.driver", NULL, 6478466, 6434, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.grammar", NULL, 6484900, 7654, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.literals", NULL, 6492554, 2002, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.parse", NULL, 6494556, 7204, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.pgen", NULL, 6501760, 12166, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.token", NULL, 6513926, 2287, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.tokenize", NULL, 6516213, 16923, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pygram", NULL, 6533136, 1427, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pytree", NULL, 6534563, 30143, NUITKA_BYTECODE_FLAG},
    {"lib2to3.refactor", NULL, 6564706, 23822, NUITKA_BYTECODE_FLAG},
    {"linecache", NULL, 6588528, 3252, NUITKA_BYTECODE_FLAG},
    {"locale", NULL, 6591780, 56524, NUITKA_BYTECODE_FLAG},
    {"logging", NULL, 6648304, 57332, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"logging.config", NULL, 6705636, 25834, NUITKA_BYTECODE_FLAG},
    {"logging.handlers", NULL, 6731470, 39488, NUITKA_BYTECODE_FLAG},
    {"macpath", NULL, 6770958, 7635, NUITKA_BYTECODE_FLAG},
    {"macurl2path", NULL, 6778593, 2228, NUITKA_BYTECODE_FLAG},
    {"mailbox", NULL, 6780821, 76271, NUITKA_BYTECODE_FLAG},
    {"mailcap", NULL, 6857092, 7045, NUITKA_BYTECODE_FLAG},
    {"markupbase", NULL, 6864137, 9225, NUITKA_BYTECODE_FLAG},
    {"md5", NULL, 6873362, 368, NUITKA_BYTECODE_FLAG},
    {"mhlib", NULL, 6873730, 33613, NUITKA_BYTECODE_FLAG},
    {"mimetools", NULL, 6907343, 8153, NUITKA_BYTECODE_FLAG},
    {"mimetypes", NULL, 6915496, 18443, NUITKA_BYTECODE_FLAG},
    {"mimify", NULL, 6933939, 11963, NUITKA_BYTECODE_FLAG},
    {"modulefinder", NULL, 6945902, 19049, NUITKA_BYTECODE_FLAG},
    {"multifile", NULL, 6964951, 5382, NUITKA_BYTECODE_FLAG},
    {"multiprocessing", NULL, 6970333, 8405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"multiprocessing.connection", NULL, 6978738, 14310, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.dummy", NULL, 6993048, 5412, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"multiprocessing.dummy.connection", NULL, 6998460, 2732, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.forking", NULL, 7001192, 14400, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.heap", NULL, 7015592, 6861, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.managers", NULL, 7022453, 38147, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.pool", NULL, 7060600, 22468, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.process", NULL, 7083068, 9544, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.queues", NULL, 7092612, 11421, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.reduction", NULL, 7104033, 5972, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.sharedctypes", NULL, 7110005, 8572, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.synchronize", NULL, 7118577, 10939, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.util", NULL, 7129516, 10081, NUITKA_BYTECODE_FLAG},
    {"mutex", NULL, 7139597, 2494, NUITKA_BYTECODE_FLAG},
    {"netrc", NULL, 7142091, 4688, NUITKA_BYTECODE_FLAG},
    {"new", NULL, 7146779, 852, NUITKA_BYTECODE_FLAG},
    {"nntplib", NULL, 7147631, 20948, NUITKA_BYTECODE_FLAG},
    {"ntpath", NULL, 7168579, 13077, NUITKA_BYTECODE_FLAG},
    {"nturl2path", NULL, 7181656, 1801, NUITKA_BYTECODE_FLAG},
    {"numbers", NULL, 7183457, 13880, NUITKA_BYTECODE_FLAG},
    {"opcode", NULL, 7197337, 6127, NUITKA_BYTECODE_FLAG},
    {"optparse", NULL, 7203464, 53606, NUITKA_BYTECODE_FLAG},
    {"os", NULL, 2846189, 25575, NUITKA_BYTECODE_FLAG},
    {"os2emxpath", NULL, 7257070, 4499, NUITKA_BYTECODE_FLAG},
    {"pdb", NULL, 7261569, 43433, NUITKA_BYTECODE_FLAG},
    {"pickle", NULL, 7305002, 38344, NUITKA_BYTECODE_FLAG},
    {"pickletools", NULL, 7343346, 56960, NUITKA_BYTECODE_FLAG},
    {"pipes", NULL, 7400306, 9268, NUITKA_BYTECODE_FLAG},
    {"pkgutil", NULL, 7409574, 18885, NUITKA_BYTECODE_FLAG},
    {"platform", NULL, 7428459, 38541, NUITKA_BYTECODE_FLAG},
    {"plistlib", NULL, 7467000, 19115, NUITKA_BYTECODE_FLAG},
    {"popen2", NULL, 7486115, 8985, NUITKA_BYTECODE_FLAG},
    {"poplib", NULL, 7495100, 13271, NUITKA_BYTECODE_FLAG},
    {"posixfile", NULL, 7508371, 7620, NUITKA_BYTECODE_FLAG},
    {"posixpath", NULL, 7515991, 11408, NUITKA_BYTECODE_FLAG},
    {"pprint", NULL, 7527399, 10148, NUITKA_BYTECODE_FLAG},
    {"profile", NULL, 7537547, 16370, NUITKA_BYTECODE_FLAG},
    {"pstats", NULL, 7553917, 24885, NUITKA_BYTECODE_FLAG},
    {"pty", NULL, 7578802, 4938, NUITKA_BYTECODE_FLAG},
    {"py_compile", NULL, 7583740, 6603, NUITKA_BYTECODE_FLAG},
    {"pyclbr", NULL, 7590343, 9617, NUITKA_BYTECODE_FLAG},
    {"pydoc", NULL, 7599960, 92384, NUITKA_BYTECODE_FLAG},
    {"pydoc_data", NULL, 7692344, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"pydoc_data.topics", NULL, 7692464, 420808, NUITKA_BYTECODE_FLAG},
    {"quopri", NULL, 8113272, 6544, NUITKA_BYTECODE_FLAG},
    {"random", NULL, 2871764, 25610, NUITKA_BYTECODE_FLAG},
    {"re", NULL, 8119816, 13363, NUITKA_BYTECODE_FLAG},
    {"repr", NULL, 8133179, 5343, NUITKA_BYTECODE_FLAG},
    {"rexec", NULL, 8138522, 23681, NUITKA_BYTECODE_FLAG},
    {"rfc822", NULL, 8162203, 31685, NUITKA_BYTECODE_FLAG},
    {"rlcompleter", NULL, 8193888, 6054, NUITKA_BYTECODE_FLAG},
    {"robotparser", NULL, 8199942, 7947, NUITKA_BYTECODE_FLAG},
    {"runpy", NULL, 8207889, 8755, NUITKA_BYTECODE_FLAG},
    {"sched", NULL, 8216644, 4968, NUITKA_BYTECODE_FLAG},
    {"sets", NULL, 8221612, 16775, NUITKA_BYTECODE_FLAG},
    {"sgmllib", NULL, 8238387, 15334, NUITKA_BYTECODE_FLAG},
    {"sha", NULL, 8253721, 411, NUITKA_BYTECODE_FLAG},
    {"shelve", NULL, 8254132, 10194, NUITKA_BYTECODE_FLAG},
    {"shlex", NULL, 8264326, 7524, NUITKA_BYTECODE_FLAG},
    {"shutil", NULL, 8271850, 19191, NUITKA_BYTECODE_FLAG},
    {"site", NULL, 8291041, 16582, NUITKA_BYTECODE_FLAG},
    {"sitecustomize", NULL, 8307623, 224, NUITKA_BYTECODE_FLAG},
    {"smtpd", NULL, 8307847, 15813, NUITKA_BYTECODE_FLAG},
    {"smtplib", NULL, 8323660, 30186, NUITKA_BYTECODE_FLAG},
    {"sndhdr", NULL, 8353846, 7319, NUITKA_BYTECODE_FLAG},
    {"socket", NULL, 8361165, 16084, NUITKA_BYTECODE_FLAG},
    {"sqlite3", NULL, 8377249, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"sqlite3.dbapi2", NULL, 8377403, 2682, NUITKA_BYTECODE_FLAG},
    {"sqlite3.dump", NULL, 8380085, 2057, NUITKA_BYTECODE_FLAG},
    {"sre", NULL, 8382142, 509, NUITKA_BYTECODE_FLAG},
    {"sre_compile", NULL, 8382651, 12522, NUITKA_BYTECODE_FLAG},
    {"sre_constants", NULL, 8395173, 6177, NUITKA_BYTECODE_FLAG},
    {"sre_parse", NULL, 8401350, 21076, NUITKA_BYTECODE_FLAG},
    {"ssl", NULL, 8422426, 32032, NUITKA_BYTECODE_FLAG},
    {"stat", NULL, 8454458, 2723, NUITKA_BYTECODE_FLAG},
    {"statvfs", NULL, 8457181, 610, NUITKA_BYTECODE_FLAG},
    {"string", NULL, 8457791, 20349, NUITKA_BYTECODE_FLAG},
    {"stringold", NULL, 8478140, 12487, NUITKA_BYTECODE_FLAG},
    {"stringprep", NULL, 8490627, 14439, NUITKA_BYTECODE_FLAG},
    {"struct", NULL, 8505066, 229, NUITKA_BYTECODE_FLAG},
    {"subprocess", NULL, 8505295, 32282, NUITKA_BYTECODE_FLAG},
    {"sunau", NULL, 8537577, 18290, NUITKA_BYTECODE_FLAG},
    {"sunaudio", NULL, 8555867, 1969, NUITKA_BYTECODE_FLAG},
    {"symbol", NULL, 8557836, 3014, NUITKA_BYTECODE_FLAG},
    {"symtable", NULL, 8560850, 11678, NUITKA_BYTECODE_FLAG},
    {"sysconfig", NULL, 8572528, 18805, NUITKA_BYTECODE_FLAG},
    {"tabnanny", NULL, 8591333, 8199, NUITKA_BYTECODE_FLAG},
    {"tarfile", NULL, 8599532, 75786, NUITKA_BYTECODE_FLAG},
    {"telnetlib", NULL, 8675318, 23080, NUITKA_BYTECODE_FLAG},
    {"tempfile", NULL, 8698398, 20228, NUITKA_BYTECODE_FLAG},
    {"test", NULL, 8718626, 114, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"test.pystone", NULL, 8718740, 7981, NUITKA_BYTECODE_FLAG},
    {"textwrap", NULL, 8726721, 12061, NUITKA_BYTECODE_FLAG},
    {"this", NULL, 8738782, 1210, NUITKA_BYTECODE_FLAG},
    {"threading", NULL, 8739992, 42406, NUITKA_BYTECODE_FLAG},
    {"timeit", NULL, 8782398, 12149, NUITKA_BYTECODE_FLAG},
    {"tkColorChooser", NULL, 8794547, 1402, NUITKA_BYTECODE_FLAG},
    {"tkCommonDialog", NULL, 8795949, 1496, NUITKA_BYTECODE_FLAG},
    {"tkFileDialog", NULL, 8797445, 5121, NUITKA_BYTECODE_FLAG},
    {"tkFont", NULL, 8802566, 7124, NUITKA_BYTECODE_FLAG},
    {"tkMessageBox", NULL, 8809690, 3864, NUITKA_BYTECODE_FLAG},
    {"tkSimpleDialog", NULL, 8813554, 9053, NUITKA_BYTECODE_FLAG},
    {"toaiff", NULL, 8822607, 3090, NUITKA_BYTECODE_FLAG},
    {"token", NULL, 8825697, 3798, NUITKA_BYTECODE_FLAG},
    {"tokenize", NULL, 8829495, 14465, NUITKA_BYTECODE_FLAG},
    {"trace", NULL, 8843960, 22717, NUITKA_BYTECODE_FLAG},
    {"traceback", NULL, 8866677, 11631, NUITKA_BYTECODE_FLAG},
    {"ttk", NULL, 8878308, 62344, NUITKA_BYTECODE_FLAG},
    {"tty", NULL, 8940652, 1303, NUITKA_BYTECODE_FLAG},
    {"turtle", NULL, 8941955, 139105, NUITKA_BYTECODE_FLAG},
    {"types", NULL, 2897374, 2703, NUITKA_BYTECODE_FLAG},
    {"unittest", NULL, 9081060, 2954, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"unittest.case", NULL, 9084014, 40207, NUITKA_BYTECODE_FLAG},
    {"unittest.loader", NULL, 9124221, 11295, NUITKA_BYTECODE_FLAG},
    {"unittest.main", NULL, 9135516, 7970, NUITKA_BYTECODE_FLAG},
    {"unittest.result", NULL, 9143486, 7869, NUITKA_BYTECODE_FLAG},
    {"unittest.runner", NULL, 9151355, 7581, NUITKA_BYTECODE_FLAG},
    {"unittest.signals", NULL, 9158936, 2753, NUITKA_BYTECODE_FLAG},
    {"unittest.suite", NULL, 9161689, 10460, NUITKA_BYTECODE_FLAG},
    {"unittest.util", NULL, 9172149, 4494, NUITKA_BYTECODE_FLAG},
    {"urllib", NULL, 9176643, 50939, NUITKA_BYTECODE_FLAG},
    {"urllib2", NULL, 9227582, 47019, NUITKA_BYTECODE_FLAG},
    {"urlparse", NULL, 9274601, 15473, NUITKA_BYTECODE_FLAG},
    {"user", NULL, 9290074, 1714, NUITKA_BYTECODE_FLAG},
    {"uu", NULL, 9291788, 4294, NUITKA_BYTECODE_FLAG},
    {"uuid", NULL, 9296082, 23095, NUITKA_BYTECODE_FLAG},
    {"warnings", NULL, 9319177, 13456, NUITKA_BYTECODE_FLAG},
    {"wave", NULL, 9332633, 19903, NUITKA_BYTECODE_FLAG},
    {"weakref", NULL, 9352536, 16327, NUITKA_BYTECODE_FLAG},
    {"webbrowser", NULL, 9368863, 19652, NUITKA_BYTECODE_FLAG},
    {"whichdb", NULL, 9388515, 2229, NUITKA_BYTECODE_FLAG},
    {"wsgiref", NULL, 9390744, 719, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"wsgiref.handlers", NULL, 9391463, 16172, NUITKA_BYTECODE_FLAG},
    {"wsgiref.headers", NULL, 9407635, 7429, NUITKA_BYTECODE_FLAG},
    {"wsgiref.simple_server", NULL, 9415064, 6199, NUITKA_BYTECODE_FLAG},
    {"wsgiref.util", NULL, 9421263, 5961, NUITKA_BYTECODE_FLAG},
    {"wsgiref.validate", NULL, 9427224, 16767, NUITKA_BYTECODE_FLAG},
    {"xdrlib", NULL, 9443991, 9810, NUITKA_BYTECODE_FLAG},
    {"xml", NULL, 9453801, 1068, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.dom", NULL, 9454869, 6427, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.dom.NodeFilter", NULL, 9461296, 1112, NUITKA_BYTECODE_FLAG},
    {"xml.dom.domreg", NULL, 9462408, 3366, NUITKA_BYTECODE_FLAG},
    {"xml.dom.expatbuilder", NULL, 9465774, 32810, NUITKA_BYTECODE_FLAG},
    {"xml.dom.minicompat", NULL, 9498584, 3394, NUITKA_BYTECODE_FLAG},
    {"xml.dom.minidom", NULL, 9501978, 65141, NUITKA_BYTECODE_FLAG},
    {"xml.dom.pulldom", NULL, 9567119, 12986, NUITKA_BYTECODE_FLAG},
    {"xml.dom.xmlbuilder", NULL, 9580105, 15222, NUITKA_BYTECODE_FLAG},
    {"xml.etree", NULL, 9595327, 119, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.etree.ElementInclude", NULL, 9595446, 1951, NUITKA_BYTECODE_FLAG},
    {"xml.etree.ElementPath", NULL, 9597397, 7560, NUITKA_BYTECODE_FLAG},
    {"xml.etree.ElementTree", NULL, 9604957, 34966, NUITKA_BYTECODE_FLAG},
    {"xml.etree.cElementTree", NULL, 9639923, 166, NUITKA_BYTECODE_FLAG},
    {"xml.parsers", NULL, 9640089, 304, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.parsers.expat", NULL, 9640393, 277, NUITKA_BYTECODE_FLAG},
    {"xml.sax", NULL, 9640670, 3725, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.sax._exceptions", NULL, 9644395, 6127, NUITKA_BYTECODE_FLAG},
    {"xml.sax.expatreader", NULL, 9650522, 15081, NUITKA_BYTECODE_FLAG},
    {"xml.sax.handler", NULL, 9665603, 12970, NUITKA_BYTECODE_FLAG},
    {"xml.sax.saxutils", NULL, 9678573, 14685, NUITKA_BYTECODE_FLAG},
    {"xml.sax.xmlreader", NULL, 9693258, 19078, NUITKA_BYTECODE_FLAG},
    {"xmllib", NULL, 9712336, 26732, NUITKA_BYTECODE_FLAG},
    {"xmlrpclib", NULL, 9739068, 43824, NUITKA_BYTECODE_FLAG},
    {"zipfile", NULL, 9782892, 42007, NUITKA_BYTECODE_FLAG},
    {NULL, NULL, 0, 0, 0}
};

void setupMetaPathBasedLoader(void) {
    static bool init_done = false;

    if (init_done == false) {
        registerMetaPathBasedUnfreezer(meta_path_loader_entries);
        init_done = true;
    }
}
