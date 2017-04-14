// OpenSSL 1.0.2h needs to be installed on the system! It is the LTS solution and will be supported until Dec 2019

#ifndef STRETCHYSALT_H
#define STRETCHYSALT_H

#include <string>

#define SALT_BIT_SIZE 512

class Stretchysalt {
    public:
        Stretchysalt();
        ~Stretchysalt();
        static std::string generate_salt();
        static std::string generate_key(std::string, std::string);
        // TODO:
        // static std::string generate_salt_and_key(std::string);
    private:
        static const long long int ITERATIONS;
};

#endif // STRETCHYSALT_H
