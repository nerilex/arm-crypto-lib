# Makefile for hfal-algos (library)
ALGO_NAME := HFAL_ALGOS

$(ALGO_NAME)_DIR      := hfal/
$(ALGO_NAME)_INCDIR   := 
$(ALGO_NAME)_OBJ      := \
hfal_blake_large.o       \
hfal_blake_small.o       \
hfal_bmw_large.o         \
hfal_bmw_small.o         \
hfal_cubehash.o          \
hfal_echo.o              \
hfal_groestl_large.o     \
hfal_groestl_small.o     \
hfal_jh.o                \
hfal_keccak.o            \
hfal_md5.o               \
hfal_sha1.o              \
hfal_sha224.o            \
hfal_sha256.o            \
hfal_sha384.o            \
hfal_sha512.o            \
hfal_shabal.o            \
hfal_skein1024.o         \
hfal_skein256.o          \
hfal_skein512.o          

