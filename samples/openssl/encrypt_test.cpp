#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>

// AES-256-CBC加密
int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char **ciphertext, int *ciphertext_len) {
    // 计算加密后数据长度（AES块大小的整数倍）
    *ciphertext_len = ((plaintext_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    *ciphertext = (unsigned char *)malloc(*ciphertext_len);
    if (!*ciphertext) return 0;

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) < 0) {
        free(*ciphertext);
        return 0;
    }

    unsigned char *iv_copy = (unsigned char *)malloc(AES_BLOCK_SIZE);
    memcpy(iv_copy, iv, AES_BLOCK_SIZE);

    AES_cbc_encrypt(plaintext, *ciphertext, plaintext_len, &aes_key, iv_copy, AES_ENCRYPT);
    free(iv_copy);
    return 1;
}

// AES-256-CBC解密
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char **plaintext, int *plaintext_len) {
    *plaintext = (unsigned char *)malloc(ciphertext_len);
    if (!*plaintext) return 0;

    AES_KEY aes_key;
    if (AES_set_decrypt_key(key, 256, &aes_key) < 0) {
        free(*plaintext);
        return 0;
    }

    unsigned char *iv_copy = (unsigned char *)malloc(AES_BLOCK_SIZE);
    memcpy(iv_copy, iv, AES_BLOCK_SIZE);

    AES_cbc_encrypt(ciphertext, *plaintext, ciphertext_len, &aes_key, iv_copy, AES_DECRYPT);
    free(iv_copy);
    *plaintext_len = ciphertext_len;  // 注意：实际应用中可能需要去除填充
    return 1;
}

int main() {
    // 初始化OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // 生成随机密钥和IV
    unsigned char key[32];  // 256位密钥
    unsigned char iv[16];   // 128位IV
    if (RAND_bytes(key, 32) != 1 || RAND_bytes(iv, 16) != 1) {
        fprintf(stderr, "随机数生成失败\n");
        return 1;
    }

    const unsigned char *plaintext = (const unsigned char *)"Hello, AES encryption!";
    int plaintext_len = strlen((const char *)plaintext);

    // 加密
    unsigned char *ciphertext = NULL;
    int ciphertext_len = 0;
    if (!aes_encrypt(plaintext, plaintext_len, key, iv, &ciphertext, &ciphertext_len)) {
        fprintf(stderr, "加密失败\n");
        return 1;
    }

    // 解密
    unsigned char *decrypted_text = NULL;
    int decrypted_len = 0;
    if (!aes_decrypt(ciphertext, ciphertext_len, key, iv, &decrypted_text, &decrypted_len)) {
        fprintf(stderr, "解密失败\n");
        free(ciphertext);
        return 1;
    }

    // 输出结果
    printf("原文: %s\n", plaintext);
    printf("密文长度: %d\n", ciphertext_len);
    printf("解密后: %.*s\n", decrypted_len, decrypted_text);

    // 清理
    free(ciphertext);
    free(decrypted_text);
    RAND_cleanup();
    ERR_free_strings();
    return 0;
}