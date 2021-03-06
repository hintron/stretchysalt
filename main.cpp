#include <iostream>
#include <string>
#include "Stretchysalt.h"


// Example usage
int main(int argc, char const *argv[]) {

    Stretchysalt stchy;
    // Stretchysalt stchy(15);

    std::cout << "Password: ";
    std::string password;

    // TODO: hide the password input
    // See http://stackoverflow.com/questions/1413445/reading-a-password-from-stdcin

    std::cin >> password;
    std::cout << "You entered " << password << std::endl;


    // 512-bit key -> 64 bytes -> 2 chars per byte for hex -> 128 + 1 null = 129 bytes
    std::string salt = Stretchysalt::generate_salt();

    std::cout << "Generated salt: " << salt << std::endl;
    // std::cout << "Generated salt length: " << salt.length() << std::endl;


    std::string key = stchy.generate_key(password, salt);

    std::cout << "Generated key: " << key << std::endl;
    // std::cout << "Generated key length: " << key.length() << std::endl;

    std::string key_single2 = stchy.concatenate_key_single_value(salt, key);
    std::cout << "Concatenated key as single value: " << key_single2 << std::endl;
    // TODO: Assert the length to be 274
    std::cout << key_single2.length() << std::endl;



    std::string key_single = stchy.generate_key_single_value(password, salt);
    std::cout << "generate_key_single_value: " << key_single << std::endl;
    // TODO: Assert the length to be 274
    std::cout << key_single.length() << std::endl;


    std::cout << "Get itr, salt, key from a key single value: "
    << Stretchysalt::get_iterations_from_key_single_value(key_single)
    << "----"
    << Stretchysalt::get_salt_from_key_single_value(key_single)
    << "----"
    << Stretchysalt::get_key_from_key_single_value(key_single)
    << std::endl;




    // Generate a key from a given password and salt
    std::cout << "=====================================================" << std::endl;
    std::cout << "Password: ";
    std::string password2;
    std::cin >> password2;
    std::cout << "You entered " << password2 << std::endl;

    std::cout << "Salt: ";
    std::string salt2;
    std::cin >> salt2;
    std::cout << "You entered " << salt2 << std::endl;

    std::string key2 = stchy.generate_key(password2, salt2);

    std::cout << "Generated key: " << key2 << std::endl;
    // std::cout << "Generated key length: " << key2.length() << std::endl;


    if(key == key2){
        std::cout << "The keys were the same! " << std::endl;
    }
    else {
        std::cout << "The keys were different... either password or salt was different" << std::endl;
    }





    return 0;
}