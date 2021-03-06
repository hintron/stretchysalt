// OpenSSL 1.0.2h needs to be installed on the system! It is the LTS solution and will be supported until Dec 2019

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include "Stretchysalt.h"





// Constructor
// Iterations should be calibrated so the whole process takes 200-1000 ms
// If it is any quicker, it defeats the point of salting and stretching!
// 2^20 iterations will effectively add 20 bits to the password
// NOTE: The generated key depends on the number of iterations.
// For default params - see https://en.wikipedia.org/wiki/C%2B%2B_classes#Constructors
// Doing it this way to initialize a const
Stretchysalt::Stretchysalt(uint64_t itr) : ITERATIONS(itr) {}

// Destructor
Stretchysalt::~Stretchysalt() {}

// CLIENT. Takes a password of any length and a 512-bit salt and calculates a 512-bit key.
// Uses the OpenSSL implementation of sha512.
// @param password : IN. A c string containing the password of the user.
// @param salt : IN. The salt to use to generate the key.
// @param generated_ke_sring_hex : OUT. A pointer to where the key hex string should be copied to.
std::string Stretchysalt::generate_key(std::string password_in, std::string salt_string_hex) {
    EVP_MD_CTX *md_context;
    const EVP_MD *md_function;
    unsigned int md_len, i;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    // NOTE: EVP_MAX_MD_SIZE is 64 (512 bits)

    const char *password = password_in.c_str();

    // printf("Salting and stretching the password...\n");
    // printf("Hash Function: SHA-512\n");
    md_function = EVP_sha512();

    // printf("Password: \"%s\"\n", password);

    // Initialize salt and generated key BIGNUMs
    BIGNUM *salt = BN_new();
    BIGNUM *generated_key = BN_new();

    BN_hex2bn(&salt, salt_string_hex.c_str());

    int salt_bytes = BN_num_bytes(salt);
    int salt_bits = BN_num_bits(salt);
    unsigned char salt_string_bin[salt_bytes];
    int salt_length = BN_bn2bin(salt, salt_string_bin);

    // printf("Salt length : bytes : bits\n%d : %d : %d\n", salt_length, salt_bytes, salt_bits);
    // printf("Salt (hex):\n");
    // for(int i = 0; i < salt_length; i++) {
    //    printf("%02x", salt_string_bin[i]);
    // }
    // printf("\n");

    // md_value needs to be initialized to all zeros, since it's on the stack
    for(int i = 0; i < EVP_MAX_MD_SIZE; ++i) {
        md_value[i] = 0;
    }

    //printf("Iterations: %llu\n", ITERATIONS);
    for (uint64_t i = 0; i < ITERATIONS; ++i) {
        md_context = EVP_MD_CTX_create();
        EVP_DigestInit(md_context, md_function);
        // Append the previous hash, password, and salt together in this order:
        //x = hash512(x || password || salt);
        EVP_DigestUpdate(md_context, md_value, EVP_MAX_MD_SIZE);
        EVP_DigestUpdate(md_context, password, strlen(password));
        EVP_DigestUpdate(md_context, salt_string_bin, salt_bytes);
        // Execute the hash and store it in md_value
        EVP_DigestFinal_ex(md_context, md_value, &md_len);
        // clean up md_context
        EVP_MD_CTX_destroy(md_context);
        // NOTE: EVP_DigestFinal() should == EVP_DigestFinal_ex() + EVP_MD_CTX_destroy()
    }
    // End loop

    // printf("Final salted and stretched password/key is: \n");
    // for(int i = 0; i < md_len; i++) {
    //    printf("%02x", md_value[i]);
    // }
    // printf("\n");

    // Convert from binary big-endian md_value to BIGNUM
    BN_bin2bn(md_value, EVP_MAX_MD_SIZE, generated_key);
    // Convert from BIGNUM to hex string and output it to user
    // Caller will need to deallocate this!
    char *generated_key_string_hex_temp;
    generated_key_string_hex_temp = BN_bn2hex(generated_key);

    // Copy the contents to a c++ string
    std::string key_string(generated_key_string_hex_temp);

    //printf("Final key in hex:\n%s\n", generated_key_string_hex);
    // Free the malloced temp key string
    OPENSSL_free(generated_key_string_hex_temp);

    // Clear the allocated BIGNUMs
    BN_clear_free(generated_key);
    BN_clear_free(salt);

    return key_string;
}



std::string Stretchysalt::generate_key_single_value(std::string password_in, std::string salt_string_hex){
    // Generate the key from the salt and password
    std::string key_hex = generate_key(password_in, salt_string_hex);
    // Concatenate the iterations, salt, and password, and return as a single value
    return Stretchysalt::concatenate_key_single_value(salt_string_hex, key_hex);
}


std::string Stretchysalt::concatenate_key_single_value(std::string salt_hex, std::string key_hex){
    // Convert ITERATIONS into a constant-width hex string
    // See http://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
    std::stringstream single_value_ss;
    single_value_ss << std::setfill('0') << std::setw(sizeof(ITERATIONS)*2) << std::hex << ITERATIONS;

    // Add in the salt and key
    // Use | delimiter, since I want the single-value key to be somewhat human-readable
    single_value_ss << "|" << salt_hex << "|" << key_hex;


    // Create a single string
    std::string single_value(single_value_ss.str());

    return single_value;
}




// NOTE: the function returns by value, so it makes a copy of std::string
std::string Stretchysalt::generate_salt() {
    // Initialize salt and generated key BIGNUMs
    BIGNUM *salt = BN_new();

    // Since argument was not supplied, generate a random salt
    // Create the random number (openssl should auto-seed from /dev/urandom)
    BN_rand(salt, SALT_BIT_SIZE, -1, 0);

    // Convert the salt BIGNUM to a hex string to store as text
    // Store the key as hex, so it is easy to read out
    // Note: This needs to be freed later
    char *salt_string_hex_temp = BN_bn2hex(salt);
    // printf("%s\n", salt_string_hex_temp);

    // Copy the contents to a c++ string
    std::string salt_string(salt_string_hex_temp);

    // Free memory
    OPENSSL_free(salt_string_hex_temp);
    BN_clear_free(salt);

    // Return a copy of the string
    return salt_string;
}


// NOTE: the function returns by value, so it makes a copy of std::string
std::string Stretchysalt::get_key_from_key_single_value(std::string key_single_value) {
    // Check length
    if(key_single_value.length() != KEY_SINGLE_VALUE_LENGTH){
        std::cout << "key_single_value is not 274 characters long! Something is wrong." << std::endl;
        exit(1);
    }

    return key_single_value.substr(17+129, 128);
}

std::string Stretchysalt::get_salt_from_key_single_value(std::string key_single_value) {
    // Check length
    if(key_single_value.length() != KEY_SINGLE_VALUE_LENGTH){
        std::cout << "key_single_value is not 274 characters long! Something is wrong." << std::endl;
        exit(1);
    }

    return key_single_value.substr(17, 128);
}

std::string Stretchysalt::get_iterations_from_key_single_value(std::string key_single_value) {
    // Check length
    if(key_single_value.length() != KEY_SINGLE_VALUE_LENGTH){
        std::cout << "key_single_value is not 274 characters long! Something is wrong." << std::endl;
        exit(1);
    }

    return key_single_value.substr(0, 16);
}





// TODO: Create a format where the key and salt are together in a single value, for ease of use
// std::string Stretchysalt::generatesalt_and_key(std::string password) {
//     return Stretchysalt::generate_key(password, Stretchysalt::GenerateSalt())
// }