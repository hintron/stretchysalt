// OpenSSL 1.0.2h needs to be installed on the system! It is the LTS solution and will be supported until Dec 2019

#ifndef STRETCHYSALT_H
#define STRETCHYSALT_H

#include <string>
#include <cstdint>

#define SALT_BIT_SIZE 512
// iterations|salt|key
// bits = 64 + 512 + 512 = 1088 bits
// Convert to hex string - 1 char for every 4 bits
// chars = 16 + 128 + 128 = 272 chars
// 272 characters + 2 delimiters = 274 characters
#define KEY_SINGLE_VALUE_LENGTH 274

class Stretchysalt {
    public:
        // Constructor - Make ITERATIONS default to 2^20
        Stretchysalt(uint64_t = 2<<20);
        ~Stretchysalt();
        static std::string generate_salt();
        std::string generate_key(std::string, std::string);
        // Store salt, key, and iterations together into a single value
        std::string generate_key_single_value(std::string, std::string);
        std::string concatenate_key_single_value(std::string, std::string);

        static std::string get_key_from_key_single_value(std::string);
        static std::string get_salt_from_key_single_value(std::string);
        static std::string get_iterations_from_key_single_value(std::string);

    private:
        const uint64_t ITERATIONS;
};

#endif // STRETCHYSALT_H
