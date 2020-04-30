# Theory-Of-Algorithms
I was required to write a program in the C programming language that calculates the MD5 hash digest of an input. This algorithm is specified in the 'Request For Comments 1321' document supplied by the the Internet Engineering Task Force [1]. The MD5 algorithm takes in a message input of arbitrary length producing an output of 128-bit message digest. It is considered to be a pseudorandom hashing algorithm meaning it is computationally impossible that the MD5 message digest, of different input, can produce two messages of the exact same message digest [3].

# How to Set-up and Run
The following steps will be a guide to download, compile and run the code.
* Clone the repository using the following:
~~~
git clone https://github.com/niemaattarian/Theory-Of-Algorithms.git
~~~
* Navigate to the correct directory if necessary
* The MD5 file can be found at:
~~~
/MD5/main.c
~~~
* Compile the file using the command:
~~~
make main
~~~
* Run the file using the command:
~~~
./main "insert text"
~~~

The compiler used to help compile the code is the *GNU Compiler Collection (GCC)*. This compiler is an integrated distribution of compilers for many well known programming languages such as C, C++, Objective-C, etc [2]. 

The following steps will be a guide to download and install the compiler:

#### Ubuntu 20.04
* Open up a terminal and enter:
~~~
$ sudo apt install gcc
~~~
* To confirm and/or check your version of installation enter the following:
~~~
$ gcc --version
~~~
* If done correctly, the following information should be displayed:
~~~
gcc (Arch Linux 9.3.0-1) 9.3.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
~~~

# Overview
A detailed overview of this project can be found at:

[Overview.md](https://github.com/niemaattarian/Theory-Of-Algorithms/blob/master/Overview.md)

This docoument details how to run the algorithm, command line arguemnts and tests as specified in the brief, a detailed breakdown of the algorithm including diagrams with relevant research, the complexity of the algorithm including the complexity of ways to breach the algorithm and relevant references.

# References

[1] Rivest, R., 1992. RFC1321: The MD5 message-digest algorithm.

[2] Stallman, R.M., 1988. Using the GNU Compiler Collection. For GCC version, 4(2).

[3] Rivest, R. and Dusse, S., 1992. The MD5 message-digest algorithm.
