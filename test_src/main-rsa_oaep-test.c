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

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

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

rsa_publickey_t pub_key;
rsa_privatekey_t priv_key;

void load_priv_conventional(void){
	bigint_t *epriv;
	epriv = malloc(sizeof(bigint_t));
	if(!epriv){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	epriv->length_B = (sizeof(private_exponent) + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	epriv->wordv =  malloc(epriv->length_B * sizeof(bigint_word_t));
	if(!epriv->wordv){
		cli_putstr("\r\nERROR: OOM!");
		return;
	}
	memcpy(epriv->wordv, private_exponent, sizeof(private_exponent));
	priv_key.components = malloc(sizeof(bigint_t*));
	priv_key.components[0] = epriv;
	priv_key.n = 1;
	bigint_changeendianess(epriv);
	bigint_adjust(epriv);
}


void load_priv_crt_mono(void){
	bigint_t **v;
	const uint8_t *bv[5] = {p,q,dp,dq,qinv};
	uint16_t sv[5] = {sizeof(p), sizeof(q), sizeof(dp), sizeof(dq), sizeof(qinv)};
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


void load_fix_rsa(void){
	bigint_t *m, *epub;
	m = malloc(sizeof(bigint_t));
	epub = malloc(sizeof(bigint_t));
	if(!m || !epub){
		cli_putstr("\r\nOOM!\r\n");
		return;
	}

	m->length_B = (sizeof(modulus) + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	epub->length_B = (sizeof(public_exponent) + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);

	m->wordv = malloc(m->length_B * sizeof(bigint_word_t));
	epub->wordv = malloc(epub->length_B * sizeof(bigint_word_t));

	if(!m->wordv || !epub->wordv){
		cli_putstr("\r\nOOM!\r\n");
		return;
	}

	memcpy(m->wordv, modulus, sizeof(modulus));
	memcpy(epub->wordv, public_exponent, sizeof(public_exponent));

	pub_key.modulus = priv_key.modulus = m;
	pub_key.exponent = epub;

	bigint_changeendianess(m);
	bigint_adjust(m);
	bigint_changeendianess(epub);
	bigint_adjust(epub);

//	load_priv_conventional();
	load_priv_crt_mono();
}


#define MSG       message2
#define SEED      seed2
#define ENCRYPTED encrypted2

void quick_test(void){
	uint8_t *ciphertext, *plaintext, rc;
	uint16_t clen, plen;
	ciphertext = malloc(clen = pub_key.modulus->length_B * sizeof(bigint_word_t));
	plaintext = malloc(pub_key.modulus->length_B * sizeof(bigint_word_t));
//	memcpy(ciphertext, message1, sizeof(message1));
	cli_putstr("\r\nplaintext:");
	cli_hexdump_block(MSG, sizeof(MSG), 4, 8);
	uart_flush(0);
	rc = rsa_encrypt_oaep(ciphertext, &clen, MSG, sizeof(MSG), &pub_key, NULL, NULL, SEED);
	if(rc){
		cli_putstr("\r\nERROR: rsa_encrypt_oaep returned: ");
		cli_hexdump_byte(rc);
		return;

	}
	cli_putstr("\r\n\r\nciphertext:");
	cli_hexdump_block(ciphertext, clen, 4, 8);
	if(clen!=sizeof(ENCRYPTED) || memcmp(ENCRYPTED, ciphertext, clen)){
		cli_putstr("\r\n>>FAIL<<");
	}else{
		cli_putstr("\r\n>>OK<<");
	}
	uart_flush(0);


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

void reset_prng(void){
	uint8_t buf[16];
	memset(buf, 0, 16);
	random_seed(buf);
	cli_putstr("\r\nPRNG reset");
}

void rsa_init(void){
	load_fix_rsa();
	prng_get_byte = random8;
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char echo_test_str[]     = "echo-test";
const char reset_prng_str[]    = "reset-prng";
const char quick_test_str[]    = "quick-test";
const char performance_str[]   = "performance";
const char echo_str[]          = "echo";

cmdlist_entry_t cmdlist[] = {
	{ reset_prng_str,       NULL, reset_prng                    },
	{ quick_test_str,       NULL, quick_test                    },
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
