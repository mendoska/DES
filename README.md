# Cryptography Assignment #2
# Implementation of DES with Output Feedback Mode of Operation



This program was created in C++ and uses DES as the encryption and output feedback mode (OFM) as the mode of operation.


I tried to make the flow and implementation of this program simple so I can learn and show the process of DES and the OFM mode of operation.

The program:

- It takes input from the user and receives it as a string.

- It then asks for 8 characters as a master key. I chose this because I can turn 8 characters into a 64 bit block which I use as the master in the DES operation.

- After taking input, I convert both into their binary representation respectively.

- For the plain text, I store it into a vector so I can use each individual block of 64-bits into the OFM.

- Then I call the OFB function which takes the master key, the plain text vector with blocks of 64bits, and the size of the vector all as parameters.

- The program then runs the OFB operation by calling the DES operation. 

- The way I made OFB was to prove that the operation works:

I output the encrypted text and then output the decrypted text after running through the operation and it results in the same text that was originally taken as input.

