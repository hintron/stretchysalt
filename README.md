# Stretchysalt
A password salting and stretching project (same idea as bcrypt).

## How to use
Run on Linux (Tested on Debian 8 - Jessse).
See main.cpp.

## Dependencies
* OpenSSL 1.0.2h and the libssl-dev package.
* g++ (sudo apt-get build-essential)

## Installation instructions
make
bin/test.out

## Attribution

Stretchysalt is simply an implementation of a salting and stretching scheme outlined in Practical Cryptography. The authors of the book said that out of all cryptography-related things that one could implement, this was the easiest and most straight-forward. So I did.

Originally, this code was used in the Oldentide server code. However, it was eventually removed in favor of a nodejs webserver and bcrypt for account management. So I decided to break out the code, massage it a bit, and put it here.  

## TODO:

* Make it object oriented, so it is easy to set the iteration strength

* Look into two types of modes: all-in-one mode, where the salt is stored with the key as a single value, and separate mode, where the salt and key are stored as separate and distinct values.

* Look into storing the # of iterations used, and reading that back in.

* Add in warnings and checks
