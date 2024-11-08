#pragma once

#define compare_helper(value1, value2, compare_sign) ({ \
    __typeof__(value1) __v1, __v2; \
    __v1 = value1; \
    __v2 = value2; \
    (__v1 compare_sign __v2) ? __v1 : __v2; \
})

#define compare_helper_min(value1, value2) \
    compare_helper(value1, value2, <)

#define compare_helper_max(value1, value2) \
    compare_helper(value1, value2, >)

/*
 * File: COMPARE_HELPER_H
 */
