#include <iostream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// 错误处理函数
void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// 1. 生成RSA密钥对（公钥+私钥）
bool generateRSAKey(RSA*& rsa, std::string& pubKey, std::string& privKey) {
    // 创建RSA对象
    rsa = RSA_new();
    if (rsa == nullptr) {
        handleErrors();
        return false;
    }

    // 生成公钥指数（通常为65537，即RSA_F4）
    BIGNUM* e = BN_new();
    if (BN_set_word(e, RSA_F4) != 1) { // RSA_F4 = 0x10001 = 65537
        handleErrors();
        BN_free(e);
        RSA_free(rsa);
        return false;
    }

    // 生成2048位密钥对
    if (RSA_generate_key_ex(rsa, 2048, e, nullptr) != 1) {
        handleErrors();
        BN_free(e);
        RSA_free(rsa);
        return false;
    }
    BN_free(e); // 释放公钥指数

    // 保存公钥到字符串（PEM格式）
    BIO* pubBio = BIO_new(BIO_s_mem());
    if (PEM_write_bio_RSAPublicKey(pubBio, rsa) != 1) {
        handleErrors();
        BIO_free(pubBio);
        RSA_free(rsa);
        return false;
    }
    char* pubPtr;
    long pubLen = BIO_get_mem_data(pubBio, &pubPtr);
    pubKey = std::string(pubPtr, pubLen);
    BIO_free(pubBio);

    // 保存私钥到字符串（PEM格式）
    BIO* privBio = BIO_new(BIO_s_mem());
    if (PEM_write_bio_RSAPrivateKey(privBio, rsa, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        handleErrors();
        BIO_free(privBio);
        RSA_free(rsa);
        return false;
    }
    char* privPtr;
    long privLen = BIO_get_mem_data(privBio, &privPtr);
    privKey = std::string(privPtr, privLen);
    BIO_free(privBio);

    return true;
}

// 2. 公钥加密（明文->密文）
std::vector<unsigned char> rsaEncrypt(const std::string& pubKey, const std::string& plaintext) {
    // 从PEM格式字符串加载公钥
    BIO* pubBio = BIO_new_mem_buf(pubKey.data(), pubKey.size());
    RSA* rsa = PEM_read_bio_RSAPublicKey(pubBio, nullptr, nullptr, nullptr);
    if (rsa == nullptr) {
        handleErrors();
        BIO_free(pubBio);
        return {};
    }
    BIO_free(pubBio);

    // 计算加密缓冲区大小（RSA_size返回密钥长度，单位字节）
    int encryptLen = RSA_size(rsa);
    std::vector<unsigned char> ciphertext(encryptLen);

    // 执行公钥加密（RSA_PKCS1_OAEP_PADDING为推荐的安全填充模式）
    int result = RSA_public_encrypt(
        plaintext.size(),                  // 明文长度
        (const unsigned char*)plaintext.data(), // 明文数据
        ciphertext.data(),                 // 密文输出缓冲区
        rsa,                               // RSA对象
        RSA_PKCS1_OAEP_PADDING             // 填充模式（安全推荐）
    );
    RSA_free(rsa);

    if (result == -1) {
        handleErrors();
        return {};
    }

    // 截取实际密文长度（result为加密后的有效字节数）
    ciphertext.resize(result);
    return ciphertext;
}

// 3. 私钥解密（密文->明文）
std::string rsaDecrypt(const std::string& privKey, const std::vector<unsigned char>& ciphertext) {
    // 从PEM格式字符串加载私钥
    BIO* privBio = BIO_new_mem_buf(privKey.data(), privKey.size());
    RSA* rsa = PEM_read_bio_RSAPrivateKey(privBio, nullptr, nullptr, nullptr);
    if (rsa == nullptr) {
        handleErrors();
        BIO_free(privBio);
        return "";
    }
    BIO_free(privBio);

    // 计算解密缓冲区大小
    int decryptLen = RSA_size(rsa);
    std::vector<unsigned char> plaintext(decryptLen);

    // 执行私钥解密（填充模式需与加密时一致）
    int result = RSA_private_decrypt(
        ciphertext.size(),                 // 密文长度
        ciphertext.data(),                 // 密文数据
        plaintext.data(),                  // 明文输出缓冲区
        rsa,                               // RSA对象
        RSA_PKCS1_OAEP_PADDING             // 与加密填充模式一致
    );
    RSA_free(rsa);

    if (result == -1) {
        handleErrors();
        return "";
    }

    // 截取实际明文长度（result为解密后的有效字节数）
    return std::string((char*)plaintext.data(), result);
}

int main() {
    // 初始化OpenSSL错误处理
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // 1. 生成密钥对
    RSA* rsa = nullptr;
    std::string pubKey, privKey;
    if (!generateRSAKey(rsa, pubKey, privKey)) {
        std::cerr << "密钥对生成失败" << std::endl;
        return 1;
    }
    RSA_free(rsa); // 此处已保存密钥字符串，释放RSA对象
    std::cout << "公钥:\n" << pubKey << std::endl;
    std::cout << "私钥:\n" << privKey << std::endl;

    // 2. 待加密的明文（注意：RSA加密长度有限制，2048位密钥约可加密245字节）
    std::string plaintext = "这是一段需要非对称加密的敏感信息";
    std::cout << "\n原始明文: " << plaintext << std::endl;

    // 3. 公钥加密
    std::vector<unsigned char> ciphertext = rsaEncrypt(pubKey, plaintext);
    if (ciphertext.empty()) {
        std::cerr << "加密失败" << std::endl;
        return 1;
    }
    std::cout << "\n加密后密文（十六进制）: ";
    for (unsigned char c : ciphertext) {
        printf("%02X", c); // 以十六进制打印密文
    }
    std::cout << std::endl;

    // 4. 私钥解密
    std::string decryptedText = rsaDecrypt(privKey, ciphertext);
    if (decryptedText.empty()) {
        std::cerr << "解密失败" << std::endl;
        return 1;
    }
    std::cout << "\n解密后明文: " << decryptedText << std::endl;

    // 清理OpenSSL资源
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}