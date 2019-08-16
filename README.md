# File XOR
## Description
I got bored in Art as Experience and decided to write this up based on a design idea I had over the summer :P. The basic premise is that a string is xor'd with a key, but the temporary key has a rotation operation performed on it and is then xor'd with the permanant key val.
## Building
Before building the program, feel free to change the roation values `#define`'d at the top of the code. It'll change by how much the characters are rotated in the temp key as the key is iterated through, so they can be any value 0-7 (although they would have to contain the same value in the encryption and decryption in order to work properly). The values are 2 and 3 by default for right rotation and left rotation respectively.

In order to build the program, please use the following format:
```
> make fxor
```
## Usage
### Encryption
For encryption, please use the following format (optional params are in [], required params are in <>):
```
> ./fxor -e <file_in> <key_len> [output_file]
```
So, if you were to input the following example:
```
> ./fxor -e peacemaker.txt 256
```
The output would be a file named `peacemaker.txt.enc` (the encrypted string) and `peacemaker.txt.key` (a file containing the key). You could also include an output file:
```
> ./fxor -e peacemaker.txt 256 encrypted.txt
```
Which would generate a file named `encrypted.txt` with the encrypted contents and a key file called `encrypted.txt.key`.
### Decryption
For decryption, please use the following format:
```
> ./fxor -d <encrypted_file> <key_file> [output_file]
```
So, if we were to run this after the last example, we would run:
```
> ./fxor -d peacemaker.txt.enc peacemaker.txt.key
```
And the decrypted output would be sent to `peacemaker.txt`. You could also give an output file as an optional last option:
```
> ./fxor -d peacemaker.txt.enc peacemaker.txt.key decrypted.txt
```
Which would send the decrypted contents to of `peacemaker.txt.enc` to `decrypted.txt`.
## Credit
Programmed by Ryan Houck (rgrahamh).
