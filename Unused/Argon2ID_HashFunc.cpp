#include "universal/cryptowrapper/argon2id.h"
#include <string.h>
#include <iomanip>
#include <iostream>
#include <argon2.h>
#include <random>
#include <openssl/rand.h>
#include <stdexcept>
#include "universal/cryptowrapper/secure_mem.h"

using namespace hashManager;

uint8_t Hashedpsw[HASHLEN];

int HashPassword()
{
    if(hashStruct.PSW_Input == NULL) {
    printf("Inputed password variable is a nullptr!\n");
    return 0;
    }
    if(hashStruct.PSW_Input[0] == '\0'){
        printf("Inputed password variable is an empty string!\n");
        return 0;
    }

    uint8_t hash1[HASHLEN];
    
    uint8_t *pwd = (uint8_t *)strdup((char*)hashStruct.PSW_Input);
    uint32_t pwdlen = strlen((char *)pwd);
    uint32_t t_cost = T_COST;            // 2-pass computation
    uint32_t m_cost = M_COST;      // 64 mebibytes memory usage
    uint32_t parallelism = PARALLELISM;       // number of threads and lanes

    int ret = argon2id_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, hashStruct.salt_in.data(), hashStruct.salt_in.size(), hash1, HASHLEN);
    if(ret != ARGON2_OK) {
        std::cerr << "Argon2 hashing failed: " << argon2_error_message(ret) << "\n";
    return 0;
}
    free(pwd);

    for( uint32_t i=0; i<HASHLEN; ++i ){ 
        Hashedpsw[i] = hash1[i];
    }
    printf("\nHashing successful\n");
    return 1;
}

std::vector<uint8_t> hashManager::HashParameters::Argon2ID_Hash()
{
    std::vector<uint8_t> hash(HASHLEN);
    
    bool success = HashPassword();

    if(success == 0){
        std::cout << "ERROR HashPassword(uint8_t* PWD, uint32_t parrallelism_in, const std::vector<uint8_t>& salt, uint32_t Tcost_in, uint32_t Mcost_in) returned 0.\n";
        return std::vector<uint8_t>(HASHLEN, '\0');
    }

    for(uint32_t i{}; i < HASHLEN; i++) {
        hash[i] = Hashedpsw[i];
    }
    return hash;
}

void PrintHash(const std::vector<uint8_t> &hash)
{
    for(uint32_t i{}; i < hash.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]) << " ";
    }
}