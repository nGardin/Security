from Crypto.PublicKey import RSA
import hashlib
import os, hashlib, struct
import ast

from Crypto.Cipher import AES
from Crypto import Random


class AES_Handler():

    def __init__(self, key):
        self.key = hashlib.sha256(key.encode("utf-8")).digest()
        self.chunksize = 16

    def encrypt_file(self, input_file):
        iv = Random.new().read(AES.block_size)
        encryptor = AES.new(self.key, AES.MODE_CBC, iv)

        output_file = input_file + ".enc"

        filesize = os.path.getsize(input_file)

        with open(input_file, "rb") as infile:
            with open(output_file, "wb") as outfile:
                outfile.write(struct.pack('<Q', filesize))
                outfile.write(iv)

                while True:
                    chunk = infile.read(self.chunksize)
                    if len(chunk) == 0:
                        break

                    elif len(chunk) % 16 != 0:
                        chunk = self._pad(chunk)

                    outfile.write(encryptor.encrypt(chunk))

    def decrypt_file(self, input_file):
        output_file = input_file + ".txt"

        with open(input_file, 'rb') as infile:
            origsize = struct.unpack('<Q', infile.read(struct.calcsize('Q')))[0]
            iv = infile.read(16)
            decryptor = AES.new(self.key, AES.MODE_CBC, iv)

            with open(output_file, 'wb') as outfile:
                while True:
                    chunk = infile.read(self.chunksize)
                    if len(chunk) == 0:
                        break
                    outfile.write((decryptor.decrypt(chunk)))

                outfile.truncate(origsize)

    def _pad(self, txt):
        return txt + (16 - len(txt)) * b'\x00'


#if __name__ == "__main__":
symetrical_key = "8393444412345678"
file_encrypter_Alice = AES_Handler(symetrical_key)
file_encrypter_Alice.encrypt_file("test.txt")


""" Taking the encrypted file and hashing it using the Hash256 algorithm, and converting the outputed hexidecimal hash 
into it's integer equivilant"""
with open("test.txt", "r") as encF:
    fileEnc = encF.read()


byteString = str.encode(fileEnc)
hashObj = hashlib.sha256(byteString)
hex_val = hashObj.hexdigest()
int_hash_value = int(hex_val, 16)

symetrical_key_for_enc = ast.literal_eval(symetrical_key)
print(symetrical_key)

"""Creating a RSA key for Alice and Bob as well as their corresponding public key"""
alice_key = RSA.generate(2048)
alice_public_key = alice_key.publickey()

bob_key = RSA.generate(2048)
bob_public_key = bob_key.publickey()

"""Alice is encrypting the symmetrical cipher key used to encrypt the text file using Bob's public key. She also sign 
the file using its hash value and her private RSA key."""
cipher_text_of_sym_key = bob_public_key.encrypt(symetrical_key_for_enc, 1)
alice_signature = alice_key.sign(int_hash_value, 1)

"""Bob is decrypting the encrypted symmetrical cipher key from Alice using his own RSA private key. And he verifies 
Alice's signature using her public RSA key."""

plain_text_of_sym_key = bob_key.decrypt(cipher_text_of_sym_key)
verification_of_alice_signature = alice_public_key.verify(int_hash_value, alice_signature)
string_plain_text_of_sym_key = str(plain_text_of_sym_key)

file_decrypter_Bob = AES_Handler(str(plain_text_of_sym_key))
file_decrypter_Bob.decrypt_file("test.txt.enc")

print("The symetrical key useed is: " + str(symetrical_key_for_enc))
print("The cipher text generated is: " + str(cipher_text_of_sym_key[0]))
print("The decrypted plain text is: " + str(plain_text_of_sym_key), '\n')

print("The hashed value for the encrypted file is:" + hex_val)
print("The interger value of the has is: " + str(int_hash_value))
print("The value of Alice's signature is: " + str(alice_signature[0]))
print("Alice's signature is verified as: " + str(verification_of_alice_signature))
