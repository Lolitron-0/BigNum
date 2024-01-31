#pragma once
#include <cassert>
#include <iostream>

#ifdef TEST_BN_OPS
#define ASSERT_BN_OP(a, b, op)                                           \
    do                                                                   \
    {                                                                    \
        std::cout << "a\n";                                              \
        std::string code{ "x = " + (std::string)(a) + (#op) +            \
                          (std::string)(b) };                            \
        PyRun_SimpleString(code.c_str());                                \
        PyObject* m{ PyImport_AddModule("__main__") };                   \
        PyObject* result{ PyObject_GetAttrString(m, "x") };              \
        PyObject* repr{ PyObject_Repr(result) };                         \
        std::string resultStr{ PyUnicode_AsUTF8(repr) };                 \
        resultStr = resultStr.substr(0, resultStr.size());               \
        auto bnRes{ (std::string)((a)op(b)) };                           \
        if (resultStr != bnRes)                                          \
        {                                                                \
            std::cout << "expected \n"                                   \
                      << bnRes << " equal\n"                             \
                      << resultStr << '\n';                              \
            exit(123);                                                   \
        }                                                                \
        Py_DECREF(result);                                               \
        Py_DECREF(repr);                                                 \
    } while (0);
#else
#define ASSERT_BN_OP(a, b, op)
#endif
