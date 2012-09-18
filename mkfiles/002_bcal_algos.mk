# Makefile for bcal-algos (library)
ALGO_NAME := BCAL_ALGOS

$(ALGO_NAME)_DIR      := bcal/
$(ALGO_NAME)_INCDIR   := 
$(ALGO_NAME)_OBJ      := \
bcal_aes128.o    \
bcal_aes192.o    \
bcal_aes256.o    \
bcal_cast5.o    \
bcal_cast6.o    \
bcal_cscipher.o    \
bcal_des.o    \
bcal_khazad.o    \
bcal_noekeon.o    \
bcal_present80.o    \
bcal_present128.o \
bcal_rc5.o    \
bcal_rc6.o    \
bcal_seed.o    \
bcal_serpent.o    \
bcal_tdes2.o    \
bcal_tdes.o    \
bcal_xtea.o    


