#include <iostream>
#include <string>
#include <vector>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

// 错误处理函数
void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// 生成RSA密钥对
RSA* generateRSAKey(int bits) {
    RSA* rsa = RSA_new();
    BIGNUM* bn = BN_new();
    
    if (RAND_poll() != 1) handleErrors();
    if (BN_set_word(bn, RSA_F4) != 1) handleErrors();
    if (RSA_generate_key_ex(rsa, bits, bn, nullptr) != 1) handleErrors();
    
    BN_free(bn);
    return rsa;
}

// 生成X.509证书（CA签名）
X509* generateCertificate(RSA* rsa, int days, X509* ca_cert, RSA* ca_rsa, const std::string& cn) {
    X509* x509 = X509_new();
    if (!x509) handleErrors();
    
    // 设置版本和序列号
    X509_set_version(x509, 2);
    ASN1_INTEGER* serial = ASN1_INTEGER_new();
    ASN1_INTEGER_set(serial, rand());
    X509_set_serialNumber(x509, serial);
    
    // 设置主体信息
    X509_NAME* name = X509_NAME_new();
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char*)"CN", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char*)"MyCompany", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)cn.c_str(), -1, -1, 0);
    
    // 设置颁发者（CA）和主体
    X509_set_issuer_name(x509, X509_get_subject_name(ca_cert));
    X509_set_subject_name(x509, name);
    X509_NAME_free(name);
    
    // 设置公钥
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey, rsa);
    X509_set_pubkey(x509, pkey);
    EVP_PKEY_free(pkey);
    
    // 设置有效期
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), days * 24 * 3600);
    
    // 使用CA私钥签名证书
    EVP_PKEY* ca_pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(ca_pkey, ca_rsa);
    X509_sign(x509, ca_pkey, EVP_sha256());
    EVP_PKEY_free(ca_pkey);
    
    return x509;
}

// 生成自签名根CA证书
X509* generateRootCA(RSA* ca_rsa, int days) {
    X509* x509 = X509_new();
    if (!x509) handleErrors();
    
    // 版本和序列号
    X509_set_version(x509, 2);
    ASN1_INTEGER* serial = ASN1_INTEGER_new();
    ASN1_INTEGER_set(serial, rand());
    X509_set_serialNumber(x509, serial);
    
    // 主体信息（根CA自己为自己签名）
    X509_NAME* name = X509_NAME_new();
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char*)"CN", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char*)"MyRootCA", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"MyRootCA", -1, -1, 0);
    X509_set_issuer_name(x509, name);
    X509_set_subject_name(x509, name);
    X509_NAME_free(name);
    
    // 设置公钥
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey, ca_rsa);
    X509_set_pubkey(x509, pkey);
    EVP_PKEY_free(pkey);
    
    // 有效期和CA扩展
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), days * 24 * 3600);
    
    X509V3_CTX ctx;
    X509V3_set_ctx_nodb(&ctx);
    X509V3_set_ctx(&ctx, x509, x509, nullptr, nullptr, 0);
    
    X509_EXTENSION* basic_constraints = X509V3_EXT_conf_nid(nullptr, &ctx, NID_basic_constraints, "critical,CA:TRUE");
    X509_add_ext(x509, basic_constraints, -1);
    X509_EXTENSION_free(basic_constraints);
    
    X509_EXTENSION* key_usage = X509V3_EXT_conf_nid(nullptr, &ctx, NID_key_usage, "keyCertSign,cRLSign");
    X509_add_ext(x509, key_usage, -1);
    X509_EXTENSION_free(key_usage);
    
    // 自签名
    EVP_PKEY* sign_key = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(sign_key, ca_rsa);
    X509_sign(x509, sign_key, EVP_sha256());
    EVP_PKEY_free(sign_key);
    
    return x509;
}

// 保存/加载函数
void saveRSAKey(RSA* rsa, const std::string& filename, bool is_private) {
    FILE* file = fopen(filename.c_str(), "w");
    if (!file) handleErrors();
    if (is_private) PEM_write_RSAPrivateKey(file, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    else PEM_write_RSAPublicKey(file, rsa);
    fclose(file);
}

void saveCertificate(X509* x509, const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "w");
    if (!file) handleErrors();
    PEM_write_X509(file, x509);
    fclose(file);
}

RSA* loadPrivateKey(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) handleErrors();
    RSA* rsa = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
    fclose(file);
    return rsa ? rsa : (handleErrors(), nullptr);
}

X509* loadCertificate(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) handleErrors();
    X509* x509 = PEM_read_X509(file, nullptr, nullptr, nullptr);
    fclose(file);
    return x509 ? x509 : (handleErrors(), nullptr);
}

// 签名和验证核心函数
std::vector<unsigned char> signWithPrivateKey(RSA* private_key, const std::string& data) {
    std::vector<unsigned char> signature(RSA_size(private_key));
    unsigned int sig_len;
    if (RSA_sign(NID_sha256, (const unsigned char*)data.c_str(), data.size(), 
                signature.data(), &sig_len, private_key) != 1) {
        handleErrors();
    }
    signature.resize(sig_len);
    return signature;
}

// 使用根证书验证用户证书合法性
bool verifyCertificate(X509* user_cert, X509* root_cert) {
    // 创建证书存储并添加根证书
    X509_STORE* store = X509_STORE_new();
    X509_STORE_add_cert(store, root_cert);
    
    // 验证证书链
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, user_cert, nullptr);
    
    bool valid = (X509_verify_cert(ctx) == 1);
    
    // 清理资源
    X509_STORE_CTX_free(ctx);
    X509_STORE_free(store);
    return valid;
}

// 从证书中提取公钥
RSA* getPublicKeyFromCert(X509* cert) {
    EVP_PKEY* pkey = X509_get_pubkey(cert);
    if (!pkey) handleErrors();
    
    RSA* rsa = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);
    return rsa ? rsa : (handleErrors(), nullptr);
}

// 使用公钥验证签名
bool verifySignature(RSA* public_key, const std::string& data, const std::vector<unsigned char>& signature) {
    return RSA_verify(NID_sha256, (const unsigned char*)data.c_str(), data.size(),
                     signature.data(), signature.size(), public_key) == 1;
}

int main() {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    try {
        // --------------------------
        // 1. 生成根CA证书（颁发者）
        // --------------------------
        std::cout << "=== 生成根CA证书 ===" << std::endl;
        RSA* root_rsa = generateRSAKey(2048);
        X509* root_cert = generateRootCA(root_rsa, 3650); // 10年有效期
        saveRSAKey(root_rsa, "root_private.pem", true);
        saveCertificate(root_cert, "root_cert.pem");
        
        // --------------------------
        // 2. 生成用户证书（由根CA签名）
        // --------------------------
        std::cout << "\n=== 生成用户证书 ===" << std::endl;
        RSA* user_rsa = generateRSAKey(2048);
        X509* user_cert = generateCertificate(user_rsa, 365, root_cert, root_rsa, "user@example.com");
        saveRSAKey(user_rsa, "user_private.pem", true);
        saveCertificate(user_cert, "user_cert.pem");
        
        // --------------------------
        // 3. 发送方：用用户私钥签名数据
        // --------------------------
        std::cout << "\n=== 发送方操作 ===" << std::endl;
        std::string data = "这是需要传输的敏感数据";
        std::vector<unsigned char> signature = signWithPrivateKey(user_rsa, data);
        std::cout << "原始数据: " << data << std::endl;
        std::cout << "签名结果: ";
        for (unsigned char c : signature) printf("%02x", c);
        std::cout << std::endl;
        
        // 发送内容：data + signature + user_cert（模拟网络传输）
        
        // --------------------------
        // 4. 接收方：验证流程
        // --------------------------
        std::cout << "\n=== 接收方操作 ===" << std::endl;
        
        // 4.1 用根证书验证用户证书合法性
        bool cert_valid = verifyCertificate(user_cert, root_cert);
        std::cout << "用户证书验证结果: " << (cert_valid ? "合法" : "非法") << std::endl;
        if (!cert_valid) {
            std::cerr << "证书无效，终止验证" << std::endl;
            return 1;
        }
        
        // 4.2 从合法证书中提取公钥
        RSA* user_pubkey = getPublicKeyFromCert(user_cert);
        
        // 4.3 用公钥验证数据签名
        bool sig_valid = verifySignature(user_pubkey, data, signature);
        std::cout << "数据签名验证结果: " << (sig_valid ? "有效" : "无效") << std::endl;
        
        // 4.4 验证篡改后的数据
        std::string tampered_data = "这是被篡改的数据";
        bool tampered_valid = verifySignature(user_pubkey, tampered_data, signature);
        std::cout << "篡改数据验证结果: " << (tampered_valid ? "有效" : "无效") << std::endl;
        
        // 清理资源
        RSA_free(root_rsa);
        RSA_free(user_rsa);
        RSA_free(user_pubkey);
        X509_free(root_cert);
        X509_free(user_cert);
    } catch (...) {
        handleErrors();
    }
    
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}