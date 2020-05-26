# DLogResolver
Discrete logarithm resolver using *Index Calculus Algorithm*. 

TEN (aka "Teoria Elementare dei Numeri") project AA 2019/2020 (*Tor Vergata University*).

### How to use

#### (For Ubuntu Users) Note that `cmake` and `libgmp3-dev` packages are required!
#### If you are using another Linux-based OS, please install `gmp` (see [official GMP website](https://gmplib.org/)) and `cmake`

1. Inside your `$HOME` directory, clone this repository using following command:

    `git clone https://github.com/AndreaG93/TEN-Project`

1. Change your current directory into `$HOME/TEN-Project`:

    `cd $HOME/TEN-Project`

1. Build this project using following commands:

    `cmake -Bbuild -H.`
    
    `cmake --build build --target all`
  
1. Run `DLogResolver` binary file:

    `/build/DLogResolver [dLogBase] [dLogArgument] [multiplicativeGroup] [ (OPTIONAL) smoothnessBound ]`

