/**
 * \file aes.h
 */
#ifndef __NT_AES_H__
#define __NT_AES_H__


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief          AES context structure
 */
typedef struct
{
    unsigned long erk[64] ;     /*!< encryption round keys */
    unsigned long drk[64] ;     /*!< decryption round keys */
    int nr;                    /*!< number of rounds      */
}
aes_context;

/** ����1
 * \brief          AES key schedule
 *
 * \param ctx      AES context to be initialized
 * \param key      the secret key
 * \param keysize  must be 128, 192 or 256
 */
void aes_set_key(aes_context *ctx, const unsigned char *key, int keysize) ;

// ����2
// ����У�ļ���
int aes_my_encrypt(aes_context *ctx,
               const unsigned char *input,
               int ilen,
               unsigned char *output,
               int osize) ;

// ����3
// ����У�Ľ���
int aes_my_decrypt(aes_context *ctx,
               const unsigned char *input,
               int ilen,
               unsigned char *output) ;

// cbc+cfb��ϼ���
void aes_mix_encrypt(aes_context *ctx,
                   unsigned char iv[16],
                   const unsigned char *input,
                   unsigned char *output,
                   int len) ;

// cbc+cfb��Ͻ���
void aes_mix_decrypt(aes_context *ctx,
                   unsigned char iv[16],
                   const unsigned char *input,
                   unsigned char *output,
                   int len) ;


/**
 * \brief          AES block encryption (ECB mode)
 *
 * \param ctx      AES context
 * \param input    plaintext  block
 * \param output   ciphertext block
 */
void aes_encrypt( aes_context *ctx,
                  const unsigned char input[16],
                  unsigned char output[16] ) ;

/**
 * \brief          AES block decryption (ECB mode)
 *
 * \param ctx      AES context
 * \param input    ciphertext block
 * \param output   plaintext  block
 */
void aes_decrypt( aes_context *ctx,
                  const unsigned char input[16],
                  unsigned char output[16] ) ;

/**
 * \brief          AES-CBC buffer encryption
 *
 * \param ctx      AES context
 * \param iv       initialization vector (modified after use)
 * \param input    buffer holding the plaintext
 * \param output   buffer holding the ciphertext
 * \param len      length of the data to be encrypted
 */
void aes_cbc_encrypt( aes_context *ctx,
                      unsigned char iv[16],
                      const unsigned char *input,
                      unsigned char *output,
                      int len ) ;

/**
 * \brief          AES-CBC buffer decryption
 *
 * \param ctx      AES context
 * \param iv       initialization vector (modified after use)
 * \param input    buffer holding the ciphertext
 * \param output   buffer holding the plaintext
 * \param len      length of the data to be decrypted
 */
void aes_cbc_decrypt( aes_context *ctx,
                      unsigned char iv[16],
                      const unsigned char *input,
                      unsigned char *output,
                      int len ) ;

void aes_cfb_encrypt( aes_context *ctx,
                      unsigned char iv[16],
                      unsigned char bytes_per_round,
                      const unsigned char *input,
                      unsigned char *output,
                      int len ) ;

void aes_cfb_decrypt( aes_context *ctx,
                      unsigned char iv[16],
                      unsigned char bytes_per_round,
                      const unsigned char *input,
                      unsigned char *output,
                      int len ) ;

int aes_my_encrypt2(aes_context *ctx,
               const unsigned char *input,
               int ilen,
               unsigned char *output,
               int osize) ;


int aes_my_decrypt2(aes_context *ctx,
               const unsigned char *input,
               int ilen,
               unsigned char *output) ;

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int aes_self_test( int verbose ) ;


//Aes����
int AesEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);

//Aes����
int AesDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


#ifdef __cplusplus
}
#endif


#endif // __NT_AES_H__

// end of file
