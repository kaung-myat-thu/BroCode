# BroCode
  "BroCode" is a tool capable of encrypting, decrypting a-z, 0-9, string with encryption key. The encryption key is of course can be anything but only known by authorized ones. This tools mainly is to improve my python skill as well as my for my own personal uses. Below you can see how "what is goin on 123" string is encrypted with a encryption key "mykey" string. Tip: Standalone programs are in the bin for those in case you're not aware of.

### Encryption Method

  String : "what is goin on 123"
 
  Encryption_key : mykey
  
  Any spaces must be replace with '+' The string is split into length of blocks equal to the length of encryption_key
  ```
    what+is+goin+on+123
    what+ is+go in+on +123+
  ```
  Then convert the blocks into its digital equivalent from the dictionary table. a=01, b=02, ... , z=26, +=00, 1=27,...9=35,0=36
    ```'what+'=2308012000, 'is+go'=0919000715,  'in+on'=0914001514 '+123+'=0027282900```
  Proceed to convert the encryption key to its digital equivalent also
    ``` mykey:1325110525 ```
    
  For each of the block each two digit in the block may sum with encrption_key relatively. In case the sum goes beyond 27 (which is total supported character count) ``` sum % 37 (total supported character count) = x ``` and take the ```x```. Then the convert the result back into it alphabetical equivalent to get encypted string.
   ```
        2308012000  0919000715  0914001514 0027282900
        1325110525  1325110525  1325110525 1325110525
        ----------  ----------  ---------- -----------
        3633122525  2207111203  2202112002 1315023425  ----> "07lyy vgklc vbktb mob8y"
   ```
   The string is finally encrypted as ```07lyyvgklcvbktbmob8y``` by encryption_key ```mykey```
   
#### Encode
   ![image of encode](https://github.com/kaung-myat-thu/BroCode/blob/master/images/encode.png)
   
#### Decode
   ![image of decode](https://github.com/kaung-myat-thu/BroCode/blob/master/images/decode.png)
    
### Requirement & Installation
  Clone this repository to your system in your desire folder.

  #### Python version
  Python must be installed and setup on your system. Run below pip command in terminal
  ```
   pip install art
   ```
  After this script should be ready. 
  
## Usage
  Run the command in the python.py folder
```
usage : python brocode.py [-d or -e] [encryption_key] [content_string] [-v or none]
```

  Tool support both encoding and decoding with the encryption key which is predefined by both party.
  
  To encrypt "what is goin on" text with encryption key "mykey" string. Additional -v command at the end to verbose or none at all to not verbose. 
  
```
example : python brocode.py -e mykey "what is goin on" 
```


 #### Linux version
 
   Move brocode file in /bin/linux/ directory to /bin in your linux system
```
usage : brocode [-d or -e] [encryption_key] [content_string] [-v or none]
```
#### Windows version
```
usage : /path/to/brocode.exe [-d or -e] [encryption_key] [content_string] [-v or none]
example : /path/to/brocode.exe -e mykey "what is goin on" 
```
