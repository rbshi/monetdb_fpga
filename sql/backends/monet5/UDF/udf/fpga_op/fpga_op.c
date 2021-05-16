#include "fpga_op.h"

str
UDFdoubleit_(int *dst, const int *src)
{
    *dst = *src << 2;
    return MAL_SUCCEED;
}