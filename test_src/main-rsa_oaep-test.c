/* main-dsa-test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * RSA test-suit
 *
*/
#include "main-test-common.h"

#include "noekeon.h"
#include "noekeon_prng.h"
#include "bigint.h"
#include "bigint_io.h"
#include "random_dummy.h"
#include "rsa_basic.h"
#include "rsa_oaep.h"

#include "performance_test.h"

const char* algo_name = "RSA-OAEP";

#define BIGINT_CEIL(x) ((((x) + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t)) *  sizeof(bigint_word_t))
#define BIGINT_OFF(x) ((sizeof(bigint_word_t) - (x) % sizeof(bigint_word_t)) % sizeof(bigint_word_t))

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
#if 0
/* ==================================
 * Example 1: A 1024-bit RSA Key Pair
 * ================================== */

/* ------------------------------
 * Components of the RSA Key Pair
 * ------------------------------ */

/* RSA modulus n: */
const uint8_t modulus[] = {
0xa8, 0xb3, 0xb2, 0x84, 0xaf, 0x8e, 0xb5, 0x0b, 0x38, 0x70, 0x34, 0xa8, 0x60, 0xf1, 0x46, 0xc4,
0x91, 0x9f, 0x31, 0x87, 0x63, 0xcd, 0x6c, 0x55, 0x98, 0xc8, 0xae, 0x48, 0x11, 0xa1, 0xe0, 0xab,
0xc4, 0xc7, 0xe0, 0xb0, 0x82, 0xd6, 0x93, 0xa5, 0xe7, 0xfc, 0xed, 0x67, 0x5c, 0xf4, 0x66, 0x85,
0x12, 0x77, 0x2c, 0x0c, 0xbc, 0x64, 0xa7, 0x42, 0xc6, 0xc6, 0x30, 0xf5, 0x33, 0xc8, 0xcc, 0x72,
0xf6, 0x2a, 0xe8, 0x33, 0xc4, 0x0b, 0xf2, 0x58, 0x42, 0xe9, 0x84, 0xbb, 0x78, 0xbd, 0xbf, 0x97,
0xc0, 0x10, 0x7d, 0x55, 0xbd, 0xb6, 0x62, 0xf5, 0xc4, 0xe0, 0xfa, 0xb9, 0x84, 0x5c, 0xb5, 0x14,
0x8e, 0xf7, 0x39, 0x2d, 0xd3, 0xaa, 0xff, 0x93, 0xae, 0x1e, 0x6b, 0x66, 0x7b, 0xb3, 0xd4, 0x24,
0x76, 0x16, 0xd4, 0xf5, 0xba, 0x10, 0xd4, 0xcf, 0xd2, 0x26, 0xde, 0x88, 0xd3, 0x9f, 0x16, 0xfb
};

/* RSA public exponent e: */
const uint8_t public_exponent[] = {
0x00, 0x01, 0x00, 0x01
};

/* RSA private exponent d: */
const uint8_t private_exponent[] = {
0x53, 0x33, 0x9c, 0xfd, 0xb7, 0x9f, 0xc8, 0x46, 0x6a, 0x65, 0x5c, 0x73, 0x16, 0xac, 0xa8, 0x5c,
0x55, 0xfd, 0x8f, 0x6d, 0xd8, 0x98, 0xfd, 0xaf, 0x11, 0x95, 0x17, 0xef, 0x4f, 0x52, 0xe8, 0xfd,
0x8e, 0x25, 0x8d, 0xf9, 0x3f, 0xee, 0x18, 0x0f, 0xa0, 0xe4, 0xab, 0x29, 0x69, 0x3c, 0xd8, 0x3b,
0x15, 0x2a, 0x55, 0x3d, 0x4a, 0xc4, 0xd1, 0x81, 0x2b, 0x8b, 0x9f, 0xa5, 0xaf, 0x0e, 0x7f, 0x55,
0xfe, 0x73, 0x04, 0xdf, 0x41, 0x57, 0x09, 0x26, 0xf3, 0x31, 0x1f, 0x15, 0xc4, 0xd6, 0x5a, 0x73,
0x2c, 0x48, 0x31, 0x16, 0xee, 0x3d, 0x3d, 0x2d, 0x0a, 0xf3, 0x54, 0x9a, 0xd9, 0xbf, 0x7c, 0xbf,
0xb7, 0x8a, 0xd8, 0x84, 0xf8, 0x4d, 0x5b, 0xeb, 0x04, 0x72, 0x4d, 0xc7, 0x36, 0x9b, 0x31, 0xde,
0xf3, 0x7d, 0x0c, 0xf5, 0x39, 0xe9, 0xcf, 0xcd, 0xd3, 0xde, 0x65, 0x37, 0x29, 0xea, 0xd5, 0xd1
};

/* Prime p: */
const uint8_t p[] = {
0xd3, 0x27, 0x37, 0xe7, 0x26, 0x7f, 0xfe, 0x13, 0x41, 0xb2, 0xd5, 0xc0, 0xd1, 0x50, 0xa8, 0x1b,
0x58, 0x6f, 0xb3, 0x13, 0x2b, 0xed, 0x2f, 0x8d, 0x52, 0x62, 0x86, 0x4a, 0x9c, 0xb9, 0xf3, 0x0a,
0xf3, 0x8b, 0xe4, 0x48, 0x59, 0x8d, 0x41, 0x3a, 0x17, 0x2e, 0xfb, 0x80, 0x2c, 0x21, 0xac, 0xf1,
0xc1, 0x1c, 0x52, 0x0c, 0x2f, 0x26, 0xa4, 0x71, 0xdc, 0xad, 0x21, 0x2e, 0xac, 0x7c, 0xa3, 0x9d
};

/* Prime q: */
const uint8_t q[] = {
0xcc, 0x88, 0x53, 0xd1, 0xd5, 0x4d, 0xa6, 0x30, 0xfa, 0xc0, 0x04, 0xf4, 0x71, 0xf2, 0x81, 0xc7,
0xb8, 0x98, 0x2d, 0x82, 0x24, 0xa4, 0x90, 0xed, 0xbe, 0xb3, 0x3d, 0x3e, 0x3d, 0x5c, 0xc9, 0x3c,
0x47, 0x65, 0x70, 0x3d, 0x1d, 0xd7, 0x91, 0x64, 0x2f, 0x1f, 0x11, 0x6a, 0x0d, 0xd8, 0x52, 0xbe,
0x24, 0x19, 0xb2, 0xaf, 0x72, 0xbf, 0xe9, 0xa0, 0x30, 0xe8, 0x60, 0xb0, 0x28, 0x8b, 0x5d, 0x77
};

/* p's CRT exponent dP: */
const uint8_t dp[] = {
0x0e, 0x12, 0xbf, 0x17, 0x18, 0xe9, 0xce, 0xf5, 0x59, 0x9b, 0xa1, 0xc3, 0x88, 0x2f, 0xe8, 0x04,
0x6a, 0x90, 0x87, 0x4e, 0xef, 0xce, 0x8f, 0x2c, 0xcc, 0x20, 0xe4, 0xf2, 0x74, 0x1f, 0xb0, 0xa3,
0x3a, 0x38, 0x48, 0xae, 0xc9, 0xc9, 0x30, 0x5f, 0xbe, 0xcb, 0xd2, 0xd7, 0x68, 0x19, 0x96, 0x7d,
0x46, 0x71, 0xac, 0xc6, 0x43, 0x1e, 0x40, 0x37, 0x96, 0x8d, 0xb3, 0x78, 0x78, 0xe6, 0x95, 0xc1
};

/* q's CRT exponent dQ: */
const uint8_t dq[] = {
0x95, 0x29, 0x7b, 0x0f, 0x95, 0xa2, 0xfa, 0x67, 0xd0, 0x07, 0x07, 0xd6, 0x09, 0xdf, 0xd4, 0xfc,
0x05, 0xc8, 0x9d, 0xaf, 0xc2, 0xef, 0x6d, 0x6e, 0xa5, 0x5b, 0xec, 0x77, 0x1e, 0xa3, 0x33, 0x73,
0x4d, 0x92, 0x51, 0xe7, 0x90, 0x82, 0xec, 0xda, 0x86, 0x6e, 0xfe, 0xf1, 0x3c, 0x45, 0x9e, 0x1a,
0x63, 0x13, 0x86, 0xb7, 0xe3, 0x54, 0xc8, 0x99, 0xf5, 0xf1, 0x12, 0xca, 0x85, 0xd7, 0x15, 0x83
};

/* CRT coefficient qInv: */
const uint8_t qinv[] = {
0x4f, 0x45, 0x6c, 0x50, 0x24, 0x93, 0xbd, 0xc0, 0xed, 0x2a, 0xb7, 0x56, 0xa3, 0xa6, 0xed, 0x4d,
0x67, 0x35, 0x2a, 0x69, 0x7d, 0x42, 0x16, 0xe9, 0x32, 0x12, 0xb1, 0x27, 0xa6, 0x3d, 0x54, 0x11,
0xce, 0x6f, 0xa9, 0x8d, 0x5d, 0xbe, 0xfd, 0x73, 0x26, 0x3e, 0x37, 0x28, 0x14, 0x27, 0x43, 0x81,
0x81, 0x66, 0xed, 0x7d, 0xd6, 0x36, 0x87, 0xdd, 0x2a, 0x8c, 0xa1, 0xd2, 0xf4, 0xfb, 0xd8, 0xe1
};

/* ---------------------------------
 * RSAES-OAEP Encryption Example 1.1
 * --------------------------------- */

/* Message to be, encrypted: */
const uint8_t message[] = {
0x66, 0x28, 0x19, 0x4e, 0x12, 0x07, 0x3d, 0xb0, 0x3b, 0xa9, 0x4c, 0xda, 0x9e, 0xf9, 0x53, 0x23,
0x97, 0xd5, 0x0d, 0xba, 0x79, 0xb9, 0x87, 0x00, 0x4a, 0xfe, 0xfe, 0x34
};

/* Seed: */
const uint8_t seed[] = {
0x18, 0xb7, 0x76, 0xea, 0x21, 0x06, 0x9d, 0x69, 0x77, 0x6a, 0x33, 0xe9, 0x6b, 0xad, 0x48, 0xe1,
0xdd, 0xa0, 0xa5, 0xef
};

/* Encryption: */
const uint8_t encrypted[] = {
0x35, 0x4f, 0xe6, 0x7b, 0x4a, 0x12, 0x6d, 0x5d, 0x35, 0xfe, 0x36, 0xc7, 0x77, 0x79, 0x1a, 0x3f,
0x7b, 0xa1, 0x3d, 0xef, 0x48, 0x4e, 0x2d, 0x39, 0x08, 0xaf, 0xf7, 0x22, 0xfa, 0xd4, 0x68, 0xfb,
0x21, 0x69, 0x6d, 0xe9, 0x5d, 0x0b, 0xe9, 0x11, 0xc2, 0xd3, 0x17, 0x4f, 0x8a, 0xfc, 0xc2, 0x01,
0x03, 0x5f, 0x7b, 0x6d, 0x8e, 0x69, 0x40, 0x2d, 0xe5, 0x45, 0x16, 0x18, 0xc2, 0x1a, 0x53, 0x5f,
0xa9, 0xd7, 0xbf, 0xc5, 0xb8, 0xdd, 0x9f, 0xc2, 0x43, 0xf8, 0xcf, 0x92, 0x7d, 0xb3, 0x13, 0x22,
0xd6, 0xe8, 0x81, 0xea, 0xa9, 0x1a, 0x99, 0x61, 0x70, 0xe6, 0x57, 0xa0, 0x5a, 0x26, 0x64, 0x26,
0xd9, 0x8c, 0x88, 0x00, 0x3f, 0x84, 0x77, 0xc1, 0x22, 0x70, 0x94, 0xa0, 0xd9, 0xfa, 0x1e, 0x8c,
0x40, 0x24, 0x30, 0x9c, 0xe1, 0xec, 0xcc, 0xb5, 0x21, 0x00, 0x35, 0xd4, 0x7a, 0xc7, 0x2e, 0x8a
};

/* Message to be encrypted: */
const uint8_t message2[] = {
0x75, 0x0c, 0x40, 0x47, 0xf5, 0x47, 0xe8, 0xe4, 0x14, 0x11, 0x85, 0x65, 0x23, 0x29, 0x8a, 0xc9,
0xba, 0xe2, 0x45, 0xef, 0xaf, 0x13, 0x97, 0xfb, 0xe5, 0x6f, 0x9d, 0xd5
};

/* Seed: */
const uint8_t seed2[] = {
0x0c, 0xc7, 0x42, 0xce, 0x4a, 0x9b, 0x7f, 0x32, 0xf9, 0x51, 0xbc, 0xb2, 0x51, 0xef, 0xd9, 0x25,
0xfe, 0x4f, 0xe3, 0x5f
};

/* Encryption: */
const uint8_t encrypted2[] = {
0x64, 0x0d, 0xb1, 0xac, 0xc5, 0x8e, 0x05, 0x68, 0xfe, 0x54, 0x07, 0xe5, 0xf9, 0xb7, 0x01, 0xdf,
0xf8, 0xc3, 0xc9, 0x1e, 0x71, 0x6c, 0x53, 0x6f, 0xc7, 0xfc, 0xec, 0x6c, 0xb5, 0xb7, 0x1c, 0x11,
0x65, 0x98, 0x8d, 0x4a, 0x27, 0x9e, 0x15, 0x77, 0xd7, 0x30, 0xfc, 0x7a, 0x29, 0x93, 0x2e, 0x3f,
0x00, 0xc8, 0x15, 0x15, 0x23, 0x6d, 0x8d, 0x8e, 0x31, 0x01, 0x7a, 0x7a, 0x09, 0xdf, 0x43, 0x52,
0xd9, 0x04, 0xcd, 0xeb, 0x79, 0xaa, 0x58, 0x3a, 0xdc, 0xc3, 0x1e, 0xa6, 0x98, 0xa4, 0xc0, 0x52,
0x83, 0xda, 0xba, 0x90, 0x89, 0xbe, 0x54, 0x91, 0xf6, 0x7c, 0x1a, 0x4e, 0xe4, 0x8d, 0xc7, 0x4b,
0xbb, 0xe6, 0x64, 0x3a, 0xef, 0x84, 0x66, 0x79, 0xb4, 0xcb, 0x39, 0x5a, 0x35, 0x2d, 0x5e, 0xd1,
0x15, 0x91, 0x2d, 0xf6, 0x96, 0xff, 0xe0, 0x70, 0x29, 0x32, 0x94, 0x6d, 0x71, 0x49, 0x2b, 0x44
};

/**********************************************************************************************/
/* ---------------------------------
 * RSAES-OAEP Encryption Example 2.1
 * --------------------------------- */

/* Message to be encrypted: */
const uint8_t message3[] = {
0x8f, 0xf0, 0x0c, 0xaa, 0x60, 0x5c, 0x70, 0x28, 0x30, 0x63, 0x4d, 0x9a, 0x6c, 0x3d, 0x42, 0xc6,
0x52, 0xb5, 0x8c, 0xf1, 0xd9, 0x2f, 0xec, 0x57, 0x0b, 0xee, 0xe7
};

/* Seed: */
const uint8_t seed3[] = {
0x8c, 0x40, 0x7b, 0x5e, 0xc2, 0x89, 0x9e, 0x50, 0x99, 0xc5, 0x3e, 0x8c, 0xe7, 0x93, 0xbf, 0x94,
0xe7, 0x1b, 0x17, 0x82
};

/* Encryption: */
const uint8_t encrypted3[] = {
0x01, 0x81, 0xaf, 0x89, 0x22, 0xb9, 0xfc, 0xb4, 0xd7, 0x9d, 0x92, 0xeb, 0xe1, 0x98, 0x15, 0x99,
0x2f, 0xc0, 0xc1, 0x43, 0x9d, 0x8b, 0xcd, 0x49, 0x13, 0x98, 0xa0, 0xf4, 0xad, 0x3a, 0x32, 0x9a,
0x5b, 0xd9, 0x38, 0x55, 0x60, 0xdb, 0x53, 0x26, 0x83, 0xc8, 0xb7, 0xda, 0x04, 0xe4, 0xb1, 0x2a,
0xed, 0x6a, 0xac, 0xdf, 0x47, 0x1c, 0x34, 0xc9, 0xcd, 0xa8, 0x91, 0xad, 0xdc, 0xc2, 0xdf, 0x34,
0x56, 0x65, 0x3a, 0xa6, 0x38, 0x2e, 0x9a, 0xe5, 0x9b, 0x54, 0x45, 0x52, 0x57, 0xeb, 0x09, 0x9d,
0x56, 0x2b, 0xbe, 0x10, 0x45, 0x3f, 0x2b, 0x6d, 0x13, 0xc5, 0x9c, 0x02, 0xe1, 0x0f, 0x1f, 0x8a,
0xbb, 0x5d, 0xa0, 0xd0, 0x57, 0x09, 0x32, 0xda, 0xcf, 0x2d, 0x09, 0x01, 0xdb, 0x72, 0x9d, 0x0f,
0xef, 0xcc, 0x05, 0x4e, 0x70, 0x96, 0x8e, 0xa5, 0x40, 0xc8, 0x1b, 0x04, 0xbc, 0xae, 0xfe, 0x72,
0x0e
};
#endif
/**********************************************************************************************/

/* ---------------------------------
 * RSAES-OAEP Encryption Example 2.4
 * --------------------------------- */

/* Message to be encrypted: */
const uint8_t message4[]  = {
0xa7, 0xeb, 0x2a, 0x50, 0x36, 0x93, 0x1d, 0x27, 0xd4, 0xe8, 0x91, 0x32, 0x6d, 0x99, 0x69, 0x2f,
0xfa, 0xdd, 0xa9, 0xbf, 0x7e, 0xfd, 0x3e, 0x34, 0xe6, 0x22, 0xc4, 0xad, 0xc0, 0x85, 0xf7, 0x21,
0xdf, 0xe8, 0x85, 0x07, 0x2c, 0x78, 0xa2, 0x03, 0xb1, 0x51, 0x73, 0x9b, 0xe5, 0x40, 0xfa, 0x8c,
0x15, 0x3a, 0x10, 0xf0, 0x0a
};

/* Seed: */
const uint8_t seed4[]  = {
0x9a, 0x7b, 0x3b, 0x0e, 0x70, 0x8b, 0xd9, 0x6f, 0x81, 0x90, 0xec, 0xab, 0x4f, 0xb9, 0xb2, 0xb3,
0x80, 0x5a, 0x81, 0x56
};

/* Encryption: */
const uint8_t encrypted4[]  = {
/* 0x00,*/ 0xa4, 0x57, 0x8c, 0xbc, 0x17, 0x63, 0x18, 0xa6, 0x38, 0xfb, 0xa7, 0xd0, 0x1d, 0xf1, 0x57,
0x46, 0xaf, 0x44, 0xd4, 0xf6, 0xcd, 0x96, 0xd7, 0xe7, 0xc4, 0x95, 0xcb, 0xf4, 0x25, 0xb0, 0x9c,
0x64, 0x9d, 0x32, 0xbf, 0x88, 0x6d, 0xa4, 0x8f, 0xba, 0xf9, 0x89, 0xa2, 0x11, 0x71, 0x87, 0xca,
0xfb, 0x1f, 0xb5, 0x80, 0x31, 0x76, 0x90, 0xe3, 0xcc, 0xd4, 0x46, 0x92, 0x0b, 0x7a, 0xf8, 0x2b,
0x31, 0xdb, 0x58, 0x04, 0xd8, 0x7d, 0x01, 0x51, 0x4a, 0xcb, 0xfa, 0x91, 0x56, 0xe7, 0x82, 0xf8,
0x67, 0xf6, 0xbe, 0xd9, 0x44, 0x9e, 0x0e, 0x9a, 0x2c, 0x09, 0xbc, 0xec, 0xc6, 0xaa, 0x08, 0x76,
0x36, 0x96, 0x5e, 0x34, 0xb3, 0xec, 0x76, 0x6f, 0x2f, 0xe2, 0xe4, 0x30, 0x18, 0xa2, 0xfd, 0xde,
0xb1, 0x40, 0x61, 0x6a, 0x0e, 0x9d, 0x82, 0xe5, 0x33, 0x10, 0x24, 0xee, 0x06, 0x52, 0xfc, 0x76,
0x41
};

/**********************************************************************************************/
#if 1
/* RSA modulus n: */
const uint8_t modulus2[]  = {
0x01, 0x94, 0x7c, 0x7f, 0xce, 0x90, 0x42, 0x5f, 0x47, 0x27, 0x9e, 0x70, 0x85, 0x1f, 0x25, 0xd5,
0xe6, 0x23, 0x16, 0xfe, 0x8a, 0x1d, 0xf1, 0x93, 0x71, 0xe3, 0xe6, 0x28, 0xe2, 0x60, 0x54, 0x3e,
0x49, 0x01, 0xef, 0x60, 0x81, 0xf6, 0x8c, 0x0b, 0x81, 0x41, 0x19, 0x0d, 0x2a, 0xe8, 0xda, 0xba,
0x7d, 0x12, 0x50, 0xec, 0x6d, 0xb6, 0x36, 0xe9, 0x44, 0xec, 0x37, 0x22, 0x87, 0x7c, 0x7c, 0x1d,
0x0a, 0x67, 0xf1, 0x4b, 0x16, 0x94, 0xc5, 0xf0, 0x37, 0x94, 0x51, 0xa4, 0x3e, 0x49, 0xa3, 0x2d,
0xde, 0x83, 0x67, 0x0b, 0x73, 0xda, 0x91, 0xa1, 0xc9, 0x9b, 0xc2, 0x3b, 0x43, 0x6a, 0x60, 0x05,
0x5c, 0x61, 0x0f, 0x0b, 0xaf, 0x99, 0xc1, 0xa0, 0x79, 0x56, 0x5b, 0x95, 0xa3, 0xf1, 0x52, 0x66,
0x32, 0xd1, 0xd4, 0xda, 0x60, 0xf2, 0x0e, 0xda, 0x25, 0xe6, 0x53, 0xc4, 0xf0, 0x02, 0x76, 0x6f,
0x45
};

/* RSA public exponent e: */
const uint8_t public_exponent2[]  = {
0x01, 0x00, 0x01
};

/* RSA private exponent d: */
const uint8_t private_exponent2[]  = {
0x08, 0x23, 0xf2, 0x0f, 0xad, 0xb5, 0xda, 0x89, 0x08, 0x8a, 0x9d, 0x00, 0x89, 0x3e, 0x21, 0xfa,
0x4a, 0x1b, 0x11, 0xfb, 0xc9, 0x3c, 0x64, 0xa3, 0xbe, 0x0b, 0xaa, 0xea, 0x97, 0xfb, 0x3b, 0x93,
0xc3, 0xff, 0x71, 0x37, 0x04, 0xc1, 0x9c, 0x96, 0x3c, 0x1d, 0x10, 0x7a, 0xae, 0x99, 0x05, 0x47,
0x39, 0xf7, 0x9e, 0x02, 0xe1, 0x86, 0xde, 0x86, 0xf8, 0x7a, 0x6d, 0xde, 0xfe, 0xa6, 0xd8, 0xcc,
0xd1, 0xd3, 0xc8, 0x1a, 0x47, 0xbf, 0xa7, 0x25, 0x5b, 0xe2, 0x06, 0x01, 0xa4, 0xa4, 0xb2, 0xf0,
0x8a, 0x16, 0x7b, 0x5e, 0x27, 0x9d, 0x71, 0x5b, 0x1b, 0x45, 0x5b, 0xdd, 0x7e, 0xab, 0x24, 0x59,
0x41, 0xd9, 0x76, 0x8b, 0x9a, 0xce, 0xfb, 0x3c, 0xcd, 0xa5, 0x95, 0x2d, 0xa3, 0xce, 0xe7, 0x25,
0x25, 0xb4, 0x50, 0x16, 0x63, 0xa8, 0xee, 0x15, 0xc9, 0xe9, 0x92, 0xd9, 0x24, 0x62, 0xfe, 0x39
};

/* Prime p: */
const uint8_t p2[]  = {
0x01, 0x59, 0xdb, 0xde, 0x04, 0xa3, 0x3e, 0xf0, 0x6f, 0xb6, 0x08, 0xb8, 0x0b, 0x19, 0x0f, 0x4d,
0x3e, 0x22, 0xbc, 0xc1, 0x3a, 0xc8, 0xe4, 0xa0, 0x81, 0x03, 0x3a, 0xbf, 0xa4, 0x16, 0xed, 0xb0,
0xb3, 0x38, 0xaa, 0x08, 0xb5, 0x73, 0x09, 0xea, 0x5a, 0x52, 0x40, 0xe7, 0xdc, 0x6e, 0x54, 0x37,
0x8c, 0x69, 0x41, 0x4c, 0x31, 0xd9, 0x7d, 0xdb, 0x1f, 0x40, 0x6d, 0xb3, 0x76, 0x9c, 0xc4, 0x1a,
0x43
};

/* Prime q: */
const uint8_t q2[]  = {
0x01, 0x2b, 0x65, 0x2f, 0x30, 0x40, 0x3b, 0x38, 0xb4, 0x09, 0x95, 0xfd, 0x6f, 0xf4, 0x1a, 0x1a,
0xcc, 0x8a, 0xda, 0x70, 0x37, 0x32, 0x36, 0xb7, 0x20, 0x2d, 0x39, 0xb2, 0xee, 0x30, 0xcf, 0xb4,
0x6d, 0xb0, 0x95, 0x11, 0xf6, 0xf3, 0x07, 0xcc, 0x61, 0xcc, 0x21, 0x60, 0x6c, 0x18, 0xa7, 0x5b,
0x8a, 0x62, 0xf8, 0x22, 0xdf, 0x03, 0x1b, 0xa0, 0xdf, 0x0d, 0xaf, 0xd5, 0x50, 0x6f, 0x56, 0x8b,
0xd7
};

/* p's CRT exponent dP: */
const uint8_t dp2[]  = {
0x43, 0x6e, 0xf5, 0x08, 0xde, 0x73, 0x65, 0x19, 0xc2, 0xda, 0x4c, 0x58, 0x0d, 0x98, 0xc8, 0x2c,
0xb7, 0x45, 0x2a, 0x3f, 0xb5, 0xef, 0xad, 0xc3, 0xb9, 0xc7, 0x78, 0x9a, 0x1b, 0xc6, 0x58, 0x4f,
0x79, 0x5a, 0xdd, 0xbb, 0xd3, 0x24, 0x39, 0xc7, 0x46, 0x86, 0x55, 0x2e, 0xcb, 0x6c, 0x2c, 0x30,
0x7a, 0x4d, 0x3a, 0xf7, 0xf5, 0x39, 0xee, 0xc1, 0x57, 0x24, 0x8c, 0x7b, 0x31, 0xf1, 0xa2, 0x55
};

/* q's CRT exponent dQ: */
const uint8_t dq2[]  = {
0x01, 0x2b, 0x15, 0xa8, 0x9f, 0x3d, 0xfb, 0x2b, 0x39, 0x07, 0x3e, 0x73, 0xf0, 0x2b, 0xdd, 0x0c,
0x1a, 0x7b, 0x37, 0x9d, 0xd4, 0x35, 0xf0, 0x5c, 0xdd, 0xe2, 0xef, 0xf9, 0xe4, 0x62, 0x94, 0x8b,
0x7c, 0xec, 0x62, 0xee, 0x90, 0x50, 0xd5, 0xe0, 0x81, 0x6e, 0x07, 0x85, 0xa8, 0x56, 0xb4, 0x91,
0x08, 0xdc, 0xb7, 0x5f, 0x36, 0x83, 0x87, 0x4d, 0x1c, 0xa6, 0x32, 0x9a, 0x19, 0x01, 0x30, 0x66,
0xff
};

/* CRT coefficient qInv: */
const uint8_t qinv2[]  = {
0x02, 0x70, 0xdb, 0x17, 0xd5, 0x91, 0x4b, 0x01, 0x8d, 0x76, 0x11, 0x8b, 0x24, 0x38, 0x9a, 0x73,
0x50, 0xec, 0x83, 0x6b, 0x00, 0x63, 0xa2, 0x17, 0x21, 0x23, 0x6f, 0xd8, 0xed, 0xb6, 0xd8, 0x9b,
0x51, 0xe7, 0xee, 0xb8, 0x7b, 0x61, 0x1b, 0x71, 0x32, 0xcb, 0x7e, 0xa7, 0x35, 0x6c, 0x23, 0x15,
0x1c, 0x1e, 0x77, 0x51, 0x50, 0x7c, 0x78, 0x6d, 0x9e, 0xe1, 0x79, 0x41, 0x70, 0xa8, 0xc8, 0xe8
};

#endif
/**********************************************************************************************/


uint8_t keys_allocated = 0;
rsa_publickey_t pub_key;
rsa_privatekey_t priv_key;

#if 0
  #define MSG       message
  #define SEED      seed
  #define ENCRYPTED encrypted
  #define MODULUS modulus
  #define PUB_EXPONENT public_exponent
  #define PRIV_EXPONENT private_exponent
  #define P p
  #define Q q
  #define DP dp
  #define DQ dq
  #define QINV qinv
#else
	#define MSG       message4
	#define SEED      seed4
	#define ENCRYPTED encrypted4
	#define MODULUS modulus2
	#define PUB_EXPONENT public_exponent2
	#define PRIV_EXPONENT private_exponent2
	#define P p2
	#define Q q2
	#define DP dp2
	#define DQ dq2
	#define QINV qinv2
#endif


uint8_t convert_nibble(uint8_t c){
	if(c>='0' && c<='9'){
		return c - '0';
	}
	c |= 'A' ^ 'a';
	if(c>='a' && c<='f'){
		return c - 'a' + 10;
	}
	return 0xff;
}

const char *block_ignore_string=" \t\r\n,;";
#define BUFFER_LIMIT 120
uint16_t read_os(void* dst, uint16_t length, const char* ignore_string){
	uint16_t counter = 0;
	uint16_t c;
	uint8_t v, tmp = 0, idx = 0;
	if(!ignore_string){
		ignore_string = block_ignore_string;
	}
	while(counter < length){
		c = cli_getc();
		if(c > 0xff){
			return counter;
		}
		if(strchr(ignore_string, c)){
			continue;
		}
		v = convert_nibble(c);
		if(v > 0x0f){
			return counter;
		}
		if(idx){
			((uint8_t*)dst)[counter++] = (tmp << 4) | v;
			idx = 0;
			if(counter % (BUFFER_LIMIT/2) == 0){
				cli_putc('.');
			}
		}else{
			tmp = v;
			idx = 1;
		}
	}
	return counter;
}

uint16_t own_atou(const char* str){
	uint16_t r=0;
	while(*str && *str >= '0' && *str <= '9'){
		r *= 10;
		r += *str++ - '0';
	}
	return r;
}

char* own_utoa(unsigned value, char* str, uint8_t radix){
	char *p = str, *b = str;
	char t;
	div_t d;
	if(radix>36){
		return NULL;
	}
	if(value == 0){
		*p++ = '0';
		*p = '\0';
		return str;
	}
	while(value){
		d = div(value, radix);
		value = d.quot;
		if(d.rem < 10){
			*p++ = '0' + d.rem;
 		}else{
 			*p++ = 'a' + d.rem - 10;
 		}
	}
	*p = '\0';
	while(str<p){
		t = *str;
		*str++ = *--p;
		*p = t;
	}
	return b;
}

uint8_t read_bigint(bigint_t* a, char* prompt){
	uint16_t read_length, actual_length;
	uint8_t off;
	uint8_t *buffer;
	char read_int_str[18];
	cli_putstr(prompt);
	cli_putstr("\r\n  length: ");
	cli_getsn(read_int_str, 16);
	read_length = own_atou(read_int_str);
	off = (sizeof(bigint_word_t) - (read_length % sizeof(bigint_word_t))) % sizeof(bigint_word_t);
	buffer = malloc(((read_length + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t)) * sizeof(bigint_word_t));
	if(!buffer){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	cli_putstr("\r\n  data: ");
	memset(buffer, 0, sizeof(bigint_word_t));
	actual_length = read_os(buffer + off, read_length, NULL);
	if(actual_length != read_length){
		cli_putstr("\r\nERROR: unexpected end of data!");
		free(buffer);
		return 1;
	}
	a->wordv = (bigint_word_t*)buffer;
	a->length_B = (read_length + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	bigint_changeendianess(a);
	bigint_adjust(a);
	return 0;
}

uint8_t pre_alloc_key_crt(void){
	uint8_t c;
	pub_key.modulus = malloc(sizeof(bigint_t));
	if(!pub_key.modulus){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	priv_key.modulus = pub_key.modulus;
	priv_key.n = 5;
	priv_key.components = malloc(5 * sizeof(bigint_t*));
	if(!priv_key.components){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	pub_key.exponent = malloc(sizeof(bigint_t));
	if(!pub_key.exponent){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	for(c=0; c<5; ++c){
		priv_key.components[c] = malloc(sizeof(bigint_t));
		if(!priv_key.components[c]){
			cli_putstr("\r\nERROR: OOM!");
			return 2;
		}
	}
	return 0;
}

void free_key(void){
	uint8_t c;
	free(pub_key.modulus->wordv);
	free(pub_key.exponent->wordv);
	free(pub_key.modulus);
	pub_key.modulus = priv_key.modulus = NULL;
	free(pub_key.exponent);
	pub_key.exponent = NULL;
	for(c = 0; c < priv_key.n; ++c){
		free(priv_key.components[c]->wordv);
		free(priv_key.components[c]);
	}
	free(priv_key.components);
	priv_key.components = NULL;
}

uint8_t read_key_crt(void){
	uint8_t r;
	cli_putstr("\r\n== reading key (crt) ==");
	r = pre_alloc_key_crt();
	if(r) return r;
	r = read_bigint(pub_key.modulus,"\r\n = module =");
	if(r) return r;
	r = read_bigint(pub_key.exponent,"\r\n = public exponent =");
	if(r) return r;
	r = read_bigint(priv_key.components[0],"\r\n = p (first prime) =");
	if(r) return r;
	r = read_bigint(priv_key.components[1],"\r\n = q (second prime) =");
	if(r) return r;
	r = read_bigint(priv_key.components[2],"\r\n = dp (p's exponent) =");
	if(r) return r;
	r = read_bigint(priv_key.components[3],"\r\n = dq (q's exponent) =");
	if(r) return r;
	r = read_bigint(priv_key.components[4],"\r\n = qInv (q' coefficient) =");
	return r;
}

uint8_t read_key_conv(void){
	uint8_t r;
	cli_putstr("\r\n== reading key (crt) ==");
	pub_key.modulus = malloc(sizeof(bigint_t));
	if(!pub_key.modulus){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	r = read_bigint(pub_key.modulus,"\r\n = module =");
	if(r) return r;
	priv_key.modulus = pub_key.modulus;
	priv_key.n = 1;
	pub_key.exponent = malloc(sizeof(bigint_t));
	if(!pub_key.exponent){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	priv_key.components = malloc(sizeof(bigint_t*));
	if(!priv_key.components){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	priv_key.components[0] = malloc(sizeof(bigint_t));
	if(!priv_key.components[0]){
		cli_putstr("\r\nERROR: OOM!");
		return 2;
	}
	r = read_bigint(pub_key.exponent,"\r\n = public exponent =");
	if(r) return r;
	r = read_bigint(priv_key.components[0],"\r\n = private exponent =");
	return r;
}

void load_priv_conventional(void){
	bigint_t *epriv;
	epriv = malloc(sizeof(bigint_t));
	if(!epriv){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	epriv->length_B = (sizeof(PRIV_EXPONENT) + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	epriv->wordv =  malloc(epriv->length_B * sizeof(bigint_word_t));
	if(!epriv->wordv){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	memcpy(epriv->wordv, PRIV_EXPONENT, sizeof(PRIV_EXPONENT));
	priv_key.components = malloc(sizeof(bigint_t*));
	priv_key.components[0] = epriv;
	priv_key.n = 1;
	bigint_changeendianess(epriv);
	bigint_adjust(epriv);
}


void load_priv_crt_mono(void){
	bigint_t **v;
	const uint8_t *bv[5] = {P,Q,DP,DQ,QINV};
	uint16_t sv[5] = {sizeof(P), sizeof(Q), sizeof(DP), sizeof(DQ), sizeof(QINV)};
	uint8_t i;
	v = malloc(5 * sizeof(bigint_t));
	if(!v){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	priv_key.components = malloc(5*sizeof(bigint_t*));
	if(!priv_key.components){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	priv_key.n = 5;
	for(i=0; i<5; ++i){
		v[i] = malloc(sizeof(bigint_t));
		v[i]->info = 0;
		v[i]->length_B = (sv[i] + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
		v[i]->wordv = calloc(v[i]->length_B , sizeof(bigint_word_t));
		if(!v[i]->wordv){
			cli_putstr("\r\nERROR: OOM!");
			return;
		}
		memcpy(v[i]->wordv, bv[i], sv[i]);
		bigint_changeendianess(v[i]);
		bigint_adjust(v[i]);
		priv_key.components[i] = v[i];
	}
}

uint8_t load_bigint_from_os(bigint_t* a, const void* os, uint16_t length_B){
	a->length_B = BIGINT_CEIL(length_B) / sizeof(bigint_word_t);
	a->wordv = malloc(BIGINT_CEIL(length_B));
	if(!a->wordv){
		cli_putstr("\r\nOOM!\r\n");
		return 1;
	}
	memset(a->wordv, 0, sizeof(bigint_word_t));
	memcpy((uint8_t*)a->wordv + BIGINT_OFF(length_B), os, length_B);
	a->info = 0;
	bigint_changeendianess(a);
	bigint_adjust(a);
	return 0;
}

void load_fix_rsa(void){
	if(keys_allocated){
		free_key();
	}
	keys_allocated = 1;

	if(pre_alloc_key_crt()){
		cli_putstr("\r\nOOM!\r\n");
		return;
	}

	load_bigint_from_os(pub_key.modulus, MODULUS, sizeof(MODULUS));
	load_bigint_from_os(pub_key.exponent, PUB_EXPONENT, sizeof(PUB_EXPONENT));
	priv_key.n = 5;
	load_bigint_from_os(priv_key.components[0], P, sizeof(P));
	load_bigint_from_os(priv_key.components[1], Q, sizeof(Q));
	load_bigint_from_os(priv_key.components[2], DP, sizeof(DP));
	load_bigint_from_os(priv_key.components[3], DQ, sizeof(DQ));
	load_bigint_from_os(priv_key.components[4], QINV, sizeof(QINV));

//	load_priv_conventional();
//	load_priv_crt_mono();
}

void quick_test(void){
	uint8_t *ciphertext, *plaintext, rc;
	uint8_t seed[sizeof(SEED)];
	uint16_t clen, plen;
	ciphertext = malloc(clen = pub_key.modulus->length_B * sizeof(bigint_word_t));
	plaintext = malloc(pub_key.modulus->length_B * sizeof(bigint_word_t));
	memcpy(plaintext, MSG, sizeof(MSG));
	memcpy(seed, SEED, sizeof(SEED));
	cli_putstr("\r\nplaintext:");
	cli_hexdump_block(plaintext, sizeof(MSG), 4, 8);
	cli_putstr("\r\nencrypting: ...");
	rc = rsa_encrypt_oaep(ciphertext, &clen, plaintext, sizeof(MSG), &pub_key, NULL, NULL, seed);
	if(rc){
		cli_putstr("\r\nERROR: rsa_encrypt_oaep returned: ");
		cli_hexdump_byte(rc);
		return;

	}

	cli_putstr("\r\n\r\nciphertext:");
	cli_hexdump_block(ciphertext, clen, 4, 8);
	if(clen!=sizeof(ENCRYPTED)){
			cli_putstr("\r\n>>FAIL (no size match)<<");
	}else{
		if(memcmp(ciphertext, ENCRYPTED, clen)){
			cli_putstr("\r\n>>FAIL (no content match)<<");
		}else{
			cli_putstr("\r\n>>OK<<");
		}
	}

	cli_putstr("\r\ndecrypting: ...");
	rc = rsa_decrypt_oaep(plaintext, &plen, ciphertext, clen, &priv_key, NULL, NULL, NULL);
	if(rc){
		cli_putstr("\r\nERROR: rsa_decrypt_oaep returned: ");
		cli_hexdump_byte(rc);
		return;
	}
	cli_putstr("\r\n\r\nplaintext:");
	cli_hexdump_block(plaintext, plen, 4, 8);

	free(ciphertext);
	free(plaintext);
}

void run_seed_test(void){
	uint8_t *msg, *ciph, *msg_;
	uint16_t msg_len, ciph_len, msg_len_;
	uint8_t seed[20], seed_out[20];
	char read_int_str[18];
	cli_putstr("\r\n== test with given seed ==");
	cli_putstr("\r\n = message =");
	cli_putstr("\r\n  length: ");
	cli_getsn(read_int_str, 16);
	msg_len = own_atou(read_int_str);
	msg = malloc(msg_len);
	if(!msg){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	ciph = malloc(bigint_length_B(pub_key.modulus));
	if(!ciph){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	msg_ = malloc(bigint_length_B(pub_key.modulus));
	if(!msg_){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	cli_putstr("\r\n  data: ");
	read_os(msg, msg_len, NULL);
	cli_putstr("\r\n  seed (20 bytes): ");
	read_os(seed, 20, NULL);

	cli_putstr("\r\n  encrypting ...");
	rsa_encrypt_oaep(ciph, &ciph_len, msg, msg_len, &pub_key, NULL, NULL, seed);
	cli_putstr("\r\n  ciphertext:");
	cli_hexdump_block(ciph, ciph_len, 4, 16);
	cli_putstr("\r\n  decrypting ... ");
	rsa_decrypt_oaep(msg_, &msg_len_, ciph, ciph_len, &priv_key, NULL, NULL, seed_out);
	cli_putstr("[done]");
	if(msg_len != msg_len_){
		char tstr[16];
		cli_putstr("\r\nERROR: wrong decrypted message length (");

		own_utoa(msg_len_, tstr, 10);
		cli_putstr(tstr);
		cli_putstr(" instead of ");
		own_utoa(msg_len, tstr, 10);
		cli_putstr(tstr);
		cli_putc(')');
		goto end;
	}
	if(memcmp(msg, msg_, msg_len)){
		cli_putstr("\r\nERROR: wrong decrypted message:");
		cli_hexdump_block(msg_, msg_len_, 4, 16);
		cli_putstr("\r\nreference:");
		cli_hexdump_block(msg, msg_len, 4, 16);
		goto end;
	}

	if(memcmp(seed, seed_out, 20)){
		cli_putstr("\r\nERROR: wrong decrypted seed:");
		cli_hexdump_block(seed_out, 20, 4, 16);
		cli_putstr("\r\nreference:");
		cli_hexdump_block(seed, 20, 4, 16);
		goto end;
	}
	cli_putstr("\r\n  >>OK<<");
end:
	free(msg);
	free(msg_);
	free(ciph);
}

void reset_prng(void){
	uint8_t buf[16];
	memset(buf, 0, 16);
	random_seed(buf);
	cli_putstr("\r\nPRNG reset");
}

void rsa_init(void){
	prng_get_byte = random8;
}

void load_key(void){
	if(keys_allocated){
		free_key();
	}
	keys_allocated = 1;
	read_key_crt();
}

void test_dump(void){
	char lstr[16];
	int len;
	cli_putstr("\r\nenter dump length: ");
	cli_getsn(lstr, 15);
	len = own_atou(lstr);
	cli_putstr("\r\ndumping 0x");
	cli_hexdump_rev(&len, 2);
	cli_putstr(" byte:");
	cli_hexdump_block(pub_key.modulus->wordv, len, 4, 8);
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char echo_test_str[]     = "echo-test";
const char reset_prng_str[]    = "reset-prng";
const char load_key_str[]      = "load-key";
const char load_fix_key_str[]  = "load-fix-key";
const char quick_test_str[]    = "quick-test";
const char seed_test_str[]     = "seed-test";
const char dump_test_str[]     = "dump-test";
const char performance_str[]   = "performance";
const char echo_str[]          = "echo";

const cmdlist_entry_t cmdlist[]  = {
	{ reset_prng_str,       NULL, reset_prng                    },
	{ load_key_str,         NULL, load_key                      },
	{ load_fix_key_str,     NULL, load_fix_rsa                  },
	{ quick_test_str,       NULL, quick_test                    },
	{ seed_test_str,        NULL, run_seed_test                 },
	{ dump_test_str,        NULL, test_dump                     },
//	{ performance_str,      NULL, testrun_performance_bigint    },
	{ echo_str,         (void*)1, (void_fpt)echo_ctrl           },
	{ NULL,                 NULL, NULL                          }
};

void dump_sp(void){
	uint8_t x;
	uint8_t *xa = &x;
	cli_putstr("\r\nstack pointer: ~");
	cli_hexdump_rev(&xa, 4);
}

int main (void){
	main_setup();

	for(;;){
		welcome_msg(algo_name);
		rsa_init();
		cmd_interface(cmdlist);
	}
}
