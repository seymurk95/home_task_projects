#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;


cpp_int BlockToInt(const std::string& block) {
    cpp_int result = 0;
    for (char ch : block) {
        result = result * 256 + static_cast<unsigned char>(ch);  
    }
    return result;
}


std::string IntToBlock(cpp_int nums) {
    std::string result;
    while (nums > 0) {
        char ch = static_cast<char>(nums % 256);
        result += ch;
        nums /= 256;
    }
    std::reverse(result.begin(), result.end());
    return result;
}


std::vector<cpp_int> encryptMessage(const std::string& message, const cpp_int& e, const cpp_int& n) {
    std::vector<cpp_int> encryptedBlocks;
    int blockSize = 2;  

    for (size_t i = 0; i < message.size(); i += blockSize) {
        std::string block = message.substr(i, blockSize);
        cpp_int blockNum = BlockToInt(block);
        encryptedBlocks.push_back(powm(blockNum, e, n)); 
    }
    
    return encryptedBlocks;
}


std::string decryptMessage(const std::vector<cpp_int>& encryptedBlocks, const cpp_int& d, const cpp_int& n) {
    std::string decryptedMessage;
    
    for (const auto& encryptedBlock : encryptedBlocks) {
        cpp_int decryptedBlock = powm(encryptedBlock, d, n);
        decryptedMessage += IntToBlock(decryptedBlock);  
    }
    
    return decryptedMessage;
}

int main() {
    cpp_int p = 2579;
    cpp_int q = 3557;
    cpp_int e = 3;
    cpp_int n = p * q;
    cpp_int phi = (p - 1) * (q - 1);
    cpp_int k = 1;
    
    while (((k * phi + 1) % e) != 0) {
        k++;
    }
    cpp_int d = (k * phi + 1) / e;

    std::string message = "Hello123";
    std::cout << "Исходное сообщение: " << message << std::endl;

    
    std::vector<cpp_int> encryptedMessage = encryptMessage(message, e, n);

    std::cout << "Зашифрованное сообщение: ";
    for (const auto& block : encryptedMessage) {
        std::cout << block << " ";
    }
    std::cout << std::endl;

    
    std::string decryptedMessage = decryptMessage(encryptedMessage, d, n);
    std::cout << "Расшифрованное сообщение: " << decryptedMessage << std::endl;

    return 0;
}
