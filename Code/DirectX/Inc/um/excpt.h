#ifndef _EXCPT_H
#define _EXCPT_H
#include <windef.h>

typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;

#define EXCEPTION_ACCESS_VIOLATION          0xC0000005
#define EXCEPTION_DATATYPE_MISALIGNMENT     0x80000002
#define EXCEPTION_BREAKPOINT                0x80000003
#define EXCEPTION_SINGLE_STEP               0x80000004
#define EXCEPTION_STACK_OVERFLOW            0xC00000FD
#define EXCEPTION_INVALID_DISPOSITION       0xC0000026
#endif
