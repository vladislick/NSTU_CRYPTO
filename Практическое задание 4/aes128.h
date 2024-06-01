#ifndef AES128_H
#define AES128_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    uint32_t eK[44], dK[44];    // encKey, decKey
    int Nr; // 10 rounds
} AesKey;

#define BLOCKSIZE 16  //AES-128 packet length is 16 bytes

// uint8_t y[4] -> uint32_t x
#define LOAD32H(x, y) \
  do { (x) = ((uint32_t)((y)[0] & 0xff)<<24) | ((uint32_t)((y)[1] & 0xff)<<16) | \
             ((uint32_t)((y)[2] & 0xff)<<8)  | ((uint32_t)((y)[3] & 0xff));} while(0)

// uint32_t x -> uint8_t y[4]
#define STORE32H(x, y) \
  do { (y)[0] = (uint8_t)(((x)>>24) & 0xff); (y)[1] = (uint8_t)(((x)>>16) & 0xff);   \
       (y)[2] = (uint8_t)(((x)>>8) & 0xff); (y)[3] = (uint8_t)((x) & 0xff); } while(0)

// Extract the nth byte from the lower bit from uint32_t x
#define BYTE(x, n) (((x) >> (8 * (n))) & 0xff)

/* used for keyExpansion */
// Byte replacement and rotate left by 1 bit
#define MIX(x) (((S[BYTE(x, 2)] << 24) & 0xff000000) ^ ((S[BYTE(x, 1)] << 16) & 0xff0000) ^ \
                ((S[BYTE(x, 0)] << 8) & 0xff00) ^ (S[BYTE(x, 3)] & 0xff))

// uint32_t x rotate left by n bits
#define ROF32(x, n)  (((x) << (n)) | ((x) >> (32-(n))))
// uint32_t x rotate right by n bits
#define ROR32(x, n)  (((x) >> (n)) | ((x) << (32-(n))))

/* for 128-bit blocks, Rijndael never uses more than 10 rcon values */
// AES-128 round constant
extern const uint32_t rcon[10];
// S box
extern unsigned char S[256];

//Inverse S box
extern unsigned char inv_S[256];

/* copy in[16] to state[4][4] */
int loadStateArray(uint8_t (*state)[4], const uint8_t *in);

/* copy state[4][4] to out[16] */
int storeStateArray(uint8_t (*state)[4], uint8_t *out);
//Key extension
int keyExpansion(const uint8_t *key, uint32_t keyLen, AesKey *aesKey);

// Round key addition
int addRoundKey(uint8_t (*state)[4], const uint32_t *key);

//Byte replacement
int subBytes(uint8_t (*state)[4]);

//Reverse byte replacement
int invSubBytes(uint8_t (*state)[4]);

//Line shift
int shiftRows(uint8_t (*state)[4]);

//Reverse shift
int invShiftRows(uint8_t (*state)[4]);

/* Galois Field (256) Multiplication of two Bytes */
// Two byte Galois field multiplication
uint8_t GMul(uint8_t u, uint8_t v);

// Column mix
int mixColumns(uint8_t (*state)[4]);

// Inverse mixing
int invMixColumns(uint8_t (*state)[4]);

// AES-128 encryption interface, the input key should be 16 bytes long, the input length should be a multiple of 16 bytes,
// In this way, the output length is the same as the input length, and the function call allocates memory for output data
int aesEncrypt(const uint8_t *key, uint32_t keyLen, const uint8_t *pt, uint8_t *ct, uint32_t len);

// AES128 decryption, the parameters require the same encryption
int aesDecrypt(const uint8_t *key, uint32_t keyLen, const uint8_t *ct, uint8_t *pt, uint32_t len);

#endif