#coding=utf-8

"""
工具
"""
import os,sys
import hashlib
import logging


def get_path_key_old(path):
    m = hashlib.md5(path)
    # 取后8位，组成32位整数
    key = m.hexdigest()[24:32]
    return int(key, 16)


def get_path_key(path):
    #BKDRHash
    seed = 131313 # 31 131 1313 13131 131313 
    hash_value1 = 0
    for c in path:
        hash_value1 = (hash_value1 * seed) + ord(c);
         
    hash_value2 = 0
    i = 0
    for c in path:
        if i&1 == 0:
            hash_value2 ^= ((hash_value2 << 7) ^ ord(c) ^ (hash_value2 >> 3))
        else:
            hash_value2 ^=  (~((hash_value2 << 11) ^ ord(c) ^ (hash_value2 >> 5)));    
        i+= 1    
    #print hash_value1, hash_value2
    #hash_value2 = 0
    hash_value = ((hash_value1 + hash_value2) & 0x7FFFFFFF)%65535
    logging.info("get_path_key %s=%d"%(path, hash_value))
    return hash_value
    #return hash_value3
    #return (hash_value & 0x7FFFFFFF)

KEYS = {}
def VisitDir(arg,dirname,names):
    for name in names:
        path = os.path.join(os.path.normcase(dirname),name)[2:]
        key = get_path_key(path)
        #print key, path
        if key in KEYS:
            print key, KEYS[key], path
        else:
            KEYS[key] = path
        
        
def init_logger(logfile, loglevel):
    if loglevel == "info":
        level = logging.INFO
    elif loglevel == "debug":
        level = logging.DEBUG 
    elif loglevel == "error":
        level = logging.ERROR
    else:
        level = logging.DEBUG
    logging.basicConfig(level=level,
                    format='%(asctime)s %(levelname)-8s %(message)s',
                    datefmt='%a, %d %b %Y %H:%M:%S',
                    filename= logfile,
                    filemode='a'
                    )  
    # define a Handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # set a format which is simpler for console use
    formatter = logging.Formatter('%(levelname)-6s %(message)s')
    # tell the handler to use this format
    console.setFormatter(formatter)
    # add the handler to the root logger
    logging.getLogger('').addHandler(console)     
   
   
def copy_file(src, dest):
    open(dest, "wb").write(open(src, "rb").read())   

def check_same_key():
    path="C:\\github\\lotus-game\\resource\\client\\480x800"
    #path = "C:\\workplace\\work\\GameClient_Resource\\native320x480"
    os.chdir(path)
    os.path.walk(".",VisitDir,())    
        
if __name__=="__main__":
    if len(sys.argv) == 2:
        print get_path_key(sys.argv[1])
    else:
        check_same_key()

