#include <iostream>
#include "cryptowrapper/Argon2ID_HashFunc.h"
#include <string>
#include "cryptowrapper/password.h"
#include <iomanip>

using namespace prototype_functions;
using namespace hashManager;

int main()
{   
    hashManager::HashParameters classobj;

	std::cout << "A Mind is Born\n";

    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;

    classobj.Salt_mem = randomByteGen(SALTLEN);
    classobj.password_mem = std::vector<uint8_t>(password.begin(), password.end());

    hashStruct = {
        classobj.password_mem.data(),
        classobj.Salt_mem
    };
    
    hashStruct.PSW_Input = classobj.password_mem.data();
    hashStruct.salt_in = classobj.Salt_mem;

    classobj.Hash_mem = classobj.Argon2ID_Hash();
    PrintHash(classobj.Hash_mem);

    
    std::cout << "\nclass object salt ";
    PrintHash(classobj.Salt_mem);
    std::cout << '\n';

    std::cout << "class object password ";
    for(uint32_t i{}; i < classobj.password_mem.size(); i++) {
        std::cout << classobj.password_mem[i];
    }
    
    std::cout << '\n';

    std::cout << "class object hash ";
    PrintHash(classobj.Hash_mem);
    std::cout << "\n";
    
    std::cout << "hashStruct PSW_Input ";
    for(uint32_t i{}; i < classobj.password_mem.size(); i++) {
        std::cout << hashStruct.PSW_Input[i];
    }
    std::cout << "\n";

    std::cout << "hashStruct salt_in ";
    PrintHash(hashStruct.salt_in);
    std::cout << "\n";
    
    std::cout << "class hash ";
    PrintHash(classobj.Hash_mem);
    std::cout << "\n";
	return 0;
}
