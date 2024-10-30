#include <iostream>
#include <string>
#include <vector>
#include <sstream>    
#include <iomanip>
class BigInt {
public:
    BigInt() = default;
    BigInt(const std::string& str) {
        for (auto it = str.rbegin(); it != str.rend(); ++it) {
            digits.push_back(*it - '0');
        }
    }

    BigInt(const BigInt& other) : digits(other.digits) {}

    BigInt& operator=(const BigInt& other) {
        digits = other.digits;
        return *this;
    }

    BigInt operator+(const BigInt& other) const {
        BigInt result;
        int carry = 0;
        for (size_t i = 0; i < std::max(digits.size(), other.digits.size()) || carry; ++i) {
            if (i == result.digits.size())
                result.digits.push_back(0);
            result.digits[i] = (i < digits.size() ? digits[i] : 0) + (i < other.digits.size() ? other.digits[i] : 0) + carry;
            carry = result.digits[i] >= 10;
            if (carry)
                result.digits[i] -= 10;
        }
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        BigInt result;
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            int sub = digits[i] - (i < other.digits.size() ? other.digits[i] : 0) - carry;
            carry = sub < 0;
            if (carry)
                sub += 10;
            result.digits.push_back(sub);
        }
        while (result.digits.size() > 1 && result.digits.back() == 0)
            result.digits.pop_back();
        return result;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.assign(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                long long cur = result.digits[i + j] + digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                result.digits[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        while (result.digits.size() > 1 && result.digits.back() == 0)
            result.digits.pop_back();
        return result;
    }

    BigInt operator/(const BigInt& other) const {
        BigInt result, current;
        result.digits.assign(digits.size(), 0);
        for (int i = digits.size() - 1; i >= 0; --i) {
            current.digits.insert(current.digits.begin(), digits[i]);
            while (current.digits.size() > 1 && current.digits.back() == 0)
                current.digits.pop_back();
            int x = 0, l = 0, r = 10;
            while (l <= r) {
                int m = (l + r) / 2;
                BigInt t = other * BigInt(std::to_string(m));
                if (t < current || t == current) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            result.digits[i] = x;
            current = current - other * BigInt(std::to_string(x));
        }
        while (result.digits.size() > 1 && result.digits.back() == 0)
            result.digits.pop_back();
        return result;
    }

    BigInt operator%(const BigInt& other) const {
        BigInt result, current;
        result.digits.assign(digits.size(), 0);
        for (int i = digits.size() - 1; i >= 0; --i) {
            current.digits.insert(current.digits.begin(), digits[i]);
            while (current.digits.size() > 1 && current.digits.back() == 0)
                current.digits.pop_back();
            int x = 0, l = 0, r = 10;
            while (l <= r) {
                int m = (l + r) / 2;
                BigInt t = other * BigInt(std::to_string(m));
                if (t < current || t == current) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            result.digits[i] = x;
            current = current - other * BigInt(std::to_string(x));
        }
        return current;
    }

     BigInt& operator%=(const BigInt& other) {
        *this = *this % other; 
        return *this;}

    bool operator<(const BigInt& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return false;
    }
    bool operator>(const BigInt& other) const {
        return other < *this; 
    }

    bool operator==(const BigInt& other) const {
        return digits == other.digits;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    std::string to_string() const {
        std::string str;
        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            str += std::to_string(*it);
        }
        return str;
    }

private:
    std::vector<int> digits;
};

std::string textToNumericString(const std::string& text) {
    std::ostringstream oss;
    for (char c : text) {
        oss << std::setw(3) << std::setfill('0') << int(c); 
    }
    return oss.str();
}


std::string numericStringToText(const std::string& numStr) {
    std::string text;
    for (size_t i = 0; i < numStr.length(); i += 3) {
        int asciiCode = std::stoi(numStr.substr(i, 3));
        text += char(asciiCode);
    }
    return text;
}
   

std::string multiplyStrings(const std::string& num1, const std::string& num2) {
    BigInt n1(num1);
    BigInt n2(num2);
    BigInt result = n1 * n2;
    return result.to_string();
}

std::string subtractConstant(const std::string& num, int constant) {
    BigInt n(num);
    BigInt c(std::to_string(constant));
    BigInt result = n - c;
    return result.to_string();
}

std::string euler(const std::string& a, const std::string& b) {
    return multiplyStrings(subtractConstant(a, 1), subtractConstant(b, 1));
}


 BigInt mod_pow(BigInt a, BigInt n, BigInt m) {
    BigInt result("1");
    a %= m;  
    while (n > BigInt("0")) {
        if (n % BigInt("2") == BigInt("1"))  
            result = (result * a) % m;
        a = (a * a) % m; 
        n = n / BigInt("2");
    }
    return result;
}

std::vector<BigInt> encryptMessage(const std::string& message, const BigInt& e, const BigInt& n) {
    std::string numericMessage = textToNumericString(message);
    std::vector<BigInt> encryptedChunks;

    
    size_t chunkSize = n.to_string().size() - 1; 
    for (size_t i = 0; i < numericMessage.size(); i += chunkSize) {
        std::string chunk = numericMessage.substr(i, chunkSize);
        BigInt chunkBigInt(chunk);
        encryptedChunks.push_back(mod_pow(chunkBigInt, e, n)); 
    }
    return encryptedChunks;
}

std::string decryptMessage(const std::vector<BigInt>& encryptedChunks, const BigInt& d, const BigInt& n) {
    std::string decryptedNumericMessage;

    for (const BigInt& encryptedChunk : encryptedChunks) {
        BigInt decryptedChunk = mod_pow(encryptedChunk, d, n); 
        decryptedNumericMessage += decryptedChunk.to_string();
    }
    return numericStringToText(decryptedNumericMessage); 
}

int main() {
    std::string p = "3557";
    std::string q = "2579";
    BigInt n = multiplyStrings(p, q);
    BigInt phi = euler(p, q);
    BigInt e = BigInt("3");
    BigInt k = BigInt("1");

    
    while (((k * phi + BigInt("1")) % e) != BigInt("0")) 
    {
        k = k + BigInt("1");
    }
    BigInt d = (k * phi + BigInt("1")) / e;

    
    std::string message = "40";
    std::cout << "Original message: " << message << std::endl;

    
    std::vector<BigInt> encryptedMessage = encryptMessage(message, e, n);
    std::cout << "Encrypted message: ";
    for (const BigInt& chunk : encryptedMessage) {
        std::cout << chunk.to_string() << " ";
    }
    std::cout << std::endl;

   
    std::string decryptedMessage = decryptMessage(encryptedMessage, d, n);
    std::cout << "Decrypted message: " << decryptedMessage << std::endl;

    return 0;
}
