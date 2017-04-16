// OpenSSL 1.0.2h needs to be installed on the system! It is the LTS solution and will be supported until Dec 2019

#ifndef STRETCHYSALT_H
#define STRETCHYSALT_H

#include <string>

#define SALT_BIT_SIZE 512

class Stretchysalt {
    public:
        // Constructor - Make ITERATIONS default to 2^20
        Stretchysalt(long long int = 2<<20);
        ~Stretchysalt();
        static std::string generate_salt();
        // static std::string generate_key(std::string, std::string);
        std::string generate_key(std::string, std::string);
        // static std::string generate_salt_and_key(std::string);

        // Store salt, key, and iterations together into a single value
        std::string generate_key_single_value(std::string, std::string);
        std::string concatenate_key_single_value(std::string, std::string);

    private:
        const long long int ITERATIONS;
};

#endif // STRETCHYSALT_H
