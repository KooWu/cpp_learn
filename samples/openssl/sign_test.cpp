#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// 生成RSA密钥对
int generate_rsa_key(const char *private_key_file, const char *public_key_file) {
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!rsa) {
        ERR_print_errors_fp(stderr);
        return 0;
    }

    // 保存私钥到文件
    FILE *fp = fopen(private_key_file, "wb");
    if (!fp) {
        RSA_free(rsa);
        return 0;
    }
    PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(fp);

    // 保存公钥到文件
    fp = fopen(public_key_file, "wb");
    if (!fp) {
        RSA_free(rsa);
        return 0;
    }
    PEM_write_RSAPublicKey(fp, rsa);
    fclose(fp);

    RSA_free(rsa);
    return 1;
}

// 使用私钥签名
int rsa_sign(const unsigned char *data, size_t data_len, unsigned char **sig, uint32_t *sig_len, const char *private_key_file) {
    FILE *fp = fopen(private_key_file, "rb");
    if (!fp) return 0;

    RSA *rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!rsa) {
        ERR_print_errors_fp(stderr);
        return 0;
    }

    *sig_len = RSA_size(rsa);
    *sig = (unsigned char *)malloc(*sig_len);
    if (!*sig) {
        RSA_free(rsa);
        return 0;
    }

    int result = RSA_sign(NID_sha256, data, data_len, *sig, sig_len, rsa);
    RSA_free(rsa);
    return result;
}

// 使用公钥验签
int rsa_verify(const unsigned char *data, size_t data_len, const unsigned char *sig, size_t sig_len, const char *public_key_file) {
    FILE *fp = fopen(public_key_file, "rb");
    if (!fp) return 0;

    RSA *rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!rsa) {
        ERR_print_errors_fp(stderr);
        return 0;
    }

    int result = RSA_verify(NID_sha256, data, data_len, sig, sig_len, rsa);
    RSA_free(rsa);
    return result;
}

int main() {
    const char *private_key_file = "private_key.pem";
    const char *public_key_file = "public_key.pem";
    const unsigned char *message = (const unsigned char *)"Hello, OpenSSL!";
    size_t message_len = strlen((const char *)message);

    // 初始化OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // 生成密钥对
    if (!generate_rsa_key(private_key_file, public_key_file)) {
        fprintf(stderr, "密钥生成失败\n");
        return 1;
    }

    // 签名
    unsigned char *signature = NULL;
    uint32_t signature_len = 0;
    if (!rsa_sign(message, message_len, &signature, &signature_len, private_key_file)) {
        fprintf(stderr, "签名失败\n");
        return 1;
    }

    // 验签
    int verify_result = rsa_verify(message, message_len, signature, signature_len, public_key_file);
    printf("验签结果: %s\n", verify_result ? "成功" : "失败");

    // 清理
    free(signature);
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}