#include "utils/types.h"
#include <stdio.h>

void printBufferHex(const I8 *title, const U8 *buf, U32 buf_len)
{
    printf("%s [ ", title);
    for (U32 i = 0 ; i < buf_len ; ++i) {
        printf("%02X%s", buf[i], ( i + 1 ) % 16 == 0 ? "\r\n" : " " );
    }
    printf("]\n");
}

I8 getI8(U8** buffer)  {
    I8 data = (I8)**buffer;
    (*buffer)++;

    return data;
}

U8 getU8(U8** buffer) {
    U8 data = (U8)**buffer;
    (*buffer)++;

    return data;
}

I16 getI16(U8** buffer) {
    I16 data = (I16)(
        ((*buffer)[0] << 8) |
        ((*buffer)[1])
    );
    (*buffer) += 2;

    return data;
}

U16 getU16(U8** buffer) {
    U16 data = (U16)(
        ((*buffer)[0] << 8) |
        ((*buffer)[1])
    );
    (*buffer) += 2;

    return data;
}

I32 getI32(U8** buffer) {
    I32 data = (I32)(
        ((*buffer)[0] << 24) |
        ((*buffer)[1] << 16) |
        ((*buffer)[2] <<  8) |
        ((*buffer)[3]      )
    );
    (*buffer) += 4;

    return data;
}

U32 getU32(U8** buffer) {
    U32 data = (U32)(
        ((*buffer)[0] << 24) |
        ((*buffer)[1] << 16) |
        ((*buffer)[2] <<  8) |
        ((*buffer)[3]      )
    );
    (*buffer) += 4;

    return data;
}

I64 getI64(U8** buffer) {
    I64 data = (I64)(
        ((I64)((*buffer)[0]) << 56) |
        ((I64)((*buffer)[1]) << 48) |
        ((I64)((*buffer)[2]) << 40) |
        ((I64)((*buffer)[3]) << 32) |
        ((I64)((*buffer)[4]) << 24) |
        ((I64)((*buffer)[5]) << 16) |
        ((I64)((*buffer)[6]) <<  8) |
        ((I64)((*buffer)[7])      )
    );
    (*buffer) += 8;

    return data;
}

U64 getU64(U8** buffer) {
    U64 data = (U64)(
        ((U64)((*buffer)[0]) << 56) |
        ((U64)((*buffer)[1]) << 48) |
        ((U64)((*buffer)[2]) << 40) |
        ((U64)((*buffer)[3]) << 32) |
        ((U64)((*buffer)[4]) << 24) |
        ((U64)((*buffer)[5]) << 16) |
        ((U64)((*buffer)[6]) <<  8) |
        ((U64)((*buffer)[7])      )
    );
    (*buffer) += 8;

    return data;
}

F32 getF32(U8** buffer) {
    union {
        I32 i;
        F32 f;
    } u;

    u.i = getU32(buffer);
    return u.f;
}

F64 getF64(U8** buffer) {
    union {
        I64 i;
        F64 f;
    } u;

    u.i = getU64(buffer);
    return u.f;
}


