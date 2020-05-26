# DLogResolver
Discrete logarithm resolver using *Index Calculus Algorithm*. 

TEN (aka "Teoria Elementare dei Numeri") project AA 2019/2020 (*Tor Vergata University*).

### How to use

1. Inside your `$HOME` directory, clone this repository using following command:

    `git clone https://github.com/AndreaG93/TEN-Project`

1. In order to build makefile, run following:

    `cmake $HOME/TEN-Project/CMakeLists.txt`

1. Finally build this project using `make`:

    `make --directory=$HOME/TEN-Project`
    
    Alternatively, change you current directory into `$HOME/TEN-Project` and run `make`

1. Run `DLogResolver` binary file:

    `$HOME/TEN-Project/DLogResolver [dLogBase] [dLogArgument] [multiplicativeGroup] [ (OPTIONAL) smoothnessBound ]`
