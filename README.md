# BroCode
  An encryption / decryption tool which encrpt string compose of a-z only. (numerical support coming soon)

```
usage : python brocode.py [-d or -e] [encryption_key] [content_string] [-v or none]
```

  Tool support both encoding and decoding with the encryption key which is predefined by both party.
  
  To encrypt "what is goin on" text with encryption key "mykey" string. Additional -v command at the end to verbose or none at all to not verbose.
  
```
example : python brocode.py -e mykey "what is goin on" 
```
# Encryption Method

  String : "what is goin on"
  
  Encryption_key : mykey
  
  Any spaces must be replace with '+' The string is split into length of blocks equal to the length of encryption_key
  ```
    what+is+goin+on
    what+ is+go in+on
  ```
  Then convert the blocks into its digital equivalent from the dictionary table. a=01, b=02, ... , z=26, +=00.
    ```'what+'=2308012000, 'is+go'=0919000715   'in+on'=0914001514```
  Proceed to convert the encryption key to its digital equivalent also
    ``` mykey:1325110525 ```
    
  For each of the block each two digit in the block may sum with encrption_key relatively. In case the sum goes beyond 27 (which is total supported character count) sum % 27 = x and take the x. Then the convert the result back into it alphabetical equivalent to get encypted string.
   ```
        2308012000  0919000715  0914001514
        1325110525  1325110525  1325110525
        ----------  ----------  ----------
        0906122525  2217111213  2220111211 ----> "iflyy vqklm vlktl"
   ```
    
  
