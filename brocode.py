#!/bin/python

import sys
from art import * 

#Created By Kaung Myat Thu 
#This encrption tool only support english character 'a,b,...,z' , '1,2,3,...,9,0' and ' ' (spaces)
#Github https://github.com/kaung-myat-thu/BroCode
mode = str()
key = str()
content = str()
verbose = str()
verboseflag = bool()

dictionTable={
    "a": 1,
    "b": 2,
    "c": 3,
    "d": 4,
    "e": 5,
    "f": 6,
    "g": 7,
    "h": 8,
    "i": 9,
    "j": 10,
    "k": 11,
    "l": 12,
    "m": 13,
    "n": 14,
    "o": 15,
    "p": 16,
    "q": 17,
    "r": 18,
    "s": 19,
    "t": 20,
    "u": 21,
    "v": 22,
    "w": 23,
    "x": 24,
    "y": 25,
    "z": 26,
    "1": 27,
    "2": 28,
    "3": 29,
    "4": 30,
    "5": 31,
    "6": 32,
    "7": 33,
    "8": 34,
    "9": 35,
    "0": 36,
    "+": 0
}

class txtcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RESET = "\033[0;0m"



def main(mode): 
    try:
        if mode == '-d':
            Decode(content,key)
        elif mode == '-e':
            Encode(content,key)
        else:
            sys.exit()
    except:
        DisplayUsage()
        sys.exit()



def DisplayUsage():
        print(txtcolors.WARNING + "Usage : python {} [-d or -e] [encryption_key] [content] [-v (optional)]".format(sys.argv[0]))
        print("-d : Decode. \n-e : Encode.\n-v : verbose true , default is false")
        print("Example : {} -e 'castle' 'pls make me secure'".format(sys.argv[0]) + txtcolors.RESET)
        

def Encode(content, key): ####################### Encode Function Defination : This function is for encoding option ################################

    content = content.replace(' ', '+')
    key = key.replace(' ', '+')
    contLen = len(content)
    keyLen = len(key)
    blocks = list()     #to store separated values

    if (keyLen > contLen) or (keyLen == contLen): #when encryption key's length is longer than the actual content
        content = content + ('+' * (keyLen - contLen))
        blocks.append(content[0:keyLen])
    
    counter = 1         #to count how many while loop has been passed as it can be useful later on 
    buffer = keyLen     
    if keyLen < contLen:    #when encrption key's length is shorter than the content
        while contLen > buffer:          
            buffer = counter * keyLen        
            counter = counter + 1             
            #print(str(buffer))          #debuggin purposes

        content = content + '+' * (buffer - contLen)
        del buffer 
        del counter

        blocks = AutoSeperate(content, len(key))

    if verboseflag == True:
        print("content: {0} , content_Length: {1} \nkey: {2} , key_Length: {3} \nChunks : {4}".format(content, len(content), key, keyLen,str(blocks)))

    key = Digitize(key)                 #convert the encryption key to its digital equivalent
    for x in range(len(blocks)):        #convert the content to its digital equivalent 
        blocks[x] = Digitize(blocks[x])
    
    if verboseflag == True:
        print('key: {}, content: {}'.format(key,str(blocks)))

    keyList = AutoSeperate(key,2) #seperate the numbers so that it can perform maths ops
    if verboseflag == True:
        print(txtcolors.OKBLUE + "key        : {}".format(keyList) + txtcolors.RESET)

    buffer = list()
    sum = int()
    bufferStr = str()
    output = str()

    for x in range(len(blocks)): #process of calculation converting back to alphabetical form to be encrypted
        buffer = AutoSeperate(blocks[x],2)

        if verboseflag == True:
            print(txtcolors.WARNING + "content[{}] : {}".format(x, buffer)+txtcolors.RESET)

        for y in range(len(key)/2):
            sum = int(buffer[y]) + int(keyList[y])
            if sum > len(dictionTable)-1:
                sum = sum % len(dictionTable)
            bufferStr = bufferStr + str(sum).zfill(2)
        blocks[x] = bufferStr
        buffer = AutoSeperate(blocks[x],2)
        
        if verboseflag == True:
            print("sum[{}]     : {}".format(x,buffer) + txtcolors.RESET)

        blocks[x] = AlphaBize(buffer)
        bufferStr = ""
        output = output + blocks[x]

    del bufferStr
    del sum
    del buffer 
    print(txtcolors.BOLD + "[!]Encoded : " + txtcolors.OKGREEN + output)

def Decode(content,key):
    key = key.replace(' ', '+')
    content = content.strip()
    content = content.replace(' ', '')
    blocks = list()
    blocks = AutoSeperate(content, len(key))

    if(len(content)%len(key)!=0):
        print(txtcolors.FAIL + "Provided hash is Invalid")
        sys.exit()
    

    if(verboseflag == True):
        print("key : {}, content : {}".format(key, blocks))

    key = Digitize(key)
    for x in range(len(blocks)):
        blocks[x] = Digitize(blocks[x])
    
    if(verboseflag == True):
        print("key : {}, content : {}".format(key, blocks))
    
    keyList = list()
    keyList = AutoSeperate(key,2)


    bufferInt = int()
    bufferLst = list()
    bufferStr = str()
    output = str()
    for x in range(len(blocks)):

        bufferLst = AutoSeperate(blocks[x],2)

        if verboseflag == True:
            print(txtcolors.WARNING + "content[{}] : {}".format(x, bufferLst) + txtcolors.RESET)

        for y in range(len(key)/2):
            if(int(bufferLst[y])<int(keyList[y])):
                bufferInt = (int(bufferLst[y]) + len(dictionTable)) - int(keyList[y])
            else:
                bufferInt = int(bufferLst[y]) - int(keyList[y])
            bufferStr = bufferStr + str(bufferInt).zfill(2)
            blocks[x] = bufferStr

        bufferLst = AutoSeperate(blocks[x],2)

        if verboseflag == True:
            print("finali[{}]  : {}".format(x,bufferLst))

        blocks[x] = AlphaBize(bufferLst)
        bufferStr = ""

        output = output + blocks[x]

    del bufferInt
    del bufferStr
    del bufferLst

    output = output.replace('+',' ').strip()
    if(verboseflag==True):
        print("key \t   : {}".format(keyList))
    print(txtcolors.BOLD + "[!]Decoded : " + txtcolors.OKGREEN + output)


def Digitize(data): ########### This function convert the alphabets to its digital equavenlent according to the dictionTable and return the data back as string #######################
    buffer = str()
    for x in data:
        if(dictionTable.get(x) != None):
            buffer = buffer + str(dictionTable.get(x)).zfill(2)
            data = buffer
        else:
            print(txtcolors.FAIL + "[-]This program does not support other than english alphabet : EXITING...." + txtcolors.RESET)
            sys.exit()
    return data

def AlphaBize(data): #### This function convert the seperated number var to its alphabetical equivalent, the list must be seperated by AutoSeperate()
    buffer = str()
    for x in range(len(data)):
        for alphabet, digit in dictionTable.items():
            if int(data[x]) == digit:
                buffer = buffer + alphabet
    return buffer

def AutoSeperate(data,spacing): ################## This function equally seperate given string by custom spacing and stored as list to return back ###########################
    incremental = spacing
    bufferLst = list()
    for x in range(len(data)/spacing):
        if x == 0:
            bufferLst.append(data[x:incremental])
        else:
            bufferLst.append(data[incremental-spacing:incremental])
        incremental = incremental + spacing
    return bufferLst

        
try:
    mode = sys.argv[1].lower()
    key = sys.argv[2].lower()
    content = sys.argv[3].lower()
    verbose = sys.argv[4].lower()

    if verbose == '-v':
        verboseflag = True
    elif verbose == '-q':
        verboseflag = False
    else:
        sys.exit()
except IndexError:
    verboseflag = False
except:
    DisplayUsage()
    sys.exit()


banner = txtcolors.HEADER + "\n\nCreated by:\n" + text2art("KaungMyatThu")
githubLink = txtcolors.BOLD + "\nGithub : https://github.com/kaung-myat-thu/\n" + txtcolors.RESET
credit = banner + githubLink
print(credit)
main(mode)










