#pragma once
#include <stdlib.h>
#include <vector>
#include <stdint.h>

class HashParameters;

const uint32_t SALTLEN = 16;
const uint32_t HASHLEN = 32;
const uint32_t T_COST = 2; 
const uint32_t M_COST = (1 << 16);
const uint32_t PARALLELISM = 16;

void PrintHash(const std::vector<uint8_t> &hash);

struct hash_params{
    uint8_t* PSW_Input;
    std::vector<uint8_t> salt_in;
};

inline hash_params hashStruct;

namespace hashManager
{
    class HashParameters {
        public:
        HashParameters() 
        {
            Salt_mem = std::vector<uint8_t>(SALTLEN, '\0');
            password_mem = std::vector<uint8_t>();
        }

        std::vector<uint8_t> password_mem;
        std::vector<uint8_t> Salt_mem;
        std::vector<uint8_t> Hash_mem;

        std::vector<uint8_t> Argon2ID_Hash();

    };
};
