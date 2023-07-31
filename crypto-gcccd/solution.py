#!/usr/bin/env python3

import time

import tqdm
import string
import socket
import hashlib
import itertools
from Crypto.Util.number import *

def proof(difficult=4):
    res=sh.recv(1024)
    while not res.endswith(b'Give me XXXX: '):
        res+=sh.recv(1024)
    salt=res[res.find(b'+')+1:res.find(b')')]
    result=res[res.rfind(b'==')+3:res.find(b'\n')].decode()
    print(res.decode(),end='')
    t=tqdm.tqdm(total=len(key)**difficult,ascii=True)
    for c in itertools.product(key,repeat=difficult):
        if hashlib.sha256(''.join(c).encode()+salt).hexdigest()==result:
            sh.send(''.join(c).encode()+b'\n')
            t.close()
            return
        else:
            t.update()

def gcd_trace(a,b):
    s=0
    while b:
        s+=1
        a,b=b,a%b
    return s


def check(t,a,b):
    #check if not overflow
    sh.send(b'%d\n'%t)
    sh.recv(1024)
    sh.send(b'%d\n'%a)
    sh.recv(1024)
    sh.send(b'%d\n'%b)
    res=sh.recv(1024)
    while not res.endswith(b'type:'):
        res+=sh.recv(1024)
    return b'Bad input!' not in res


t0=time.time()
key=string.ascii_letters+string.digits
sh=socket.socket()
sh.connect(('localhost',23333))
proof()

t=[0,1]
for i in range(1024):
    t.append(t[-1]+t[-2])
assert check(3,t[986],t[985]) and not check(3,t[987],t[986])
m=1
k=1
while check(3,t[986]<<k,t[985]<<k):
    #gcd(2**k,flag) --> gcd(2**k,0)
    m*=2
    k+=1
while k<500:
    r1=gcd_trace(2**(k+1),m)
    r2=gcd_trace(2**(k+1),m+2**k)
    if r1>r2:
        if check(3,t[986-r2]<<(k+1),t[985-r2]<<(k+1)):
            #gcd(2**(k+1),flag) --> gcd(2**(k+1),m)
            m+=2**k
    else:
        if not check(3,t[986-r1]<<(k+1),t[985-r1]<<(k+1)):
            #gcd(2**(k+1),flag) --> gcd(2**(k+1),m)
            m+=2**k
    k+=1
    flag=long_to_bytes(m)
    print(m,flag)
    if flag.startswith(b'CTF'):
        flag='*'+flag.decode()
        print(flag)
        break

t1=time.time()
print(t1-t0)