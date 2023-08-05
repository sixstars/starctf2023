class chiper():
    def __init__(self):
        self.d = 0x87654321
        k0 = 0x67452301
        k1 = 0xefcdab89
        k2 = 0x98badcfe
        k3 = 0x10325476
        self.k = [k0, k1, k2, k3]

    def e(self, n, v):
        from ctypes import c_uint32

        def MX(z, y, total, key, p, e):
            temp1 = (z.value >> 6 ^ y.value << 4) + \
                (y.value >> 2 ^ z.value << 5)
            temp2 = (total.value ^ y.value) + \
                (key[(p & 3) ^ e.value] ^ z.value)
            return c_uint32(temp1 ^ temp2)
        key = self.k
        delta = self.d
        rounds = 6 + 52//n
        total = c_uint32(0)
        z = c_uint32(v[n-1])
        e = c_uint32(0)

        while rounds > 0:
            total.value += delta
            e.value = (total.value >> 2) & 3
            for p in range(n-1):
                y = c_uint32(v[p+1])
                v[p] = c_uint32(v[p] + MX(z, y, total, key, p, e).value).value
                z.value = v[p]
            y = c_uint32(v[0])
            v[n-1] = c_uint32(v[n-1] + MX(z, y, total,
                              key, n-1, e).value).value
            z.value = v[n-1]
            rounds -= 1
        return v

    def bytes2ints(self,cs:bytes)->list:
        new_length=len(cs)+(8-len(cs)%8)%8
        barray=cs.ljust(new_length,b'\x00')
        i=0
        v=[]
        while i < new_length:
            v0 = int.from_bytes(barray[i:i+4], 'little')
            v1 = int.from_bytes(barray[i+4:i+8], 'little')
            v.append(v0)
            v.append(v1)
            i += 8
        return v
    
    def ints2bytes(self,v:list)->bytes:
        n=len(v)
        res=b''
        for i in range(n//2):
            res+=int.to_bytes(v[2*i],4,'little') 
            res+=int.to_bytes(v[2*i+1],4,'little')
        return res
    
    def de(self,n, v):
        key=self.k
        delta = self.d 
        rounds = 6 + 52//n 
        from ctypes import c_uint32
        def MX(z, y, total, key, p, e):
            temp1 = (z.value>>6 ^ y.value<<4) + (y.value>>2 ^ z.value<<5)
            temp2 = (total.value ^ y.value) + (key[(p&3) ^ e.value] ^ z.value)
            return c_uint32(temp1 ^ temp2)
        total = c_uint32(rounds * delta)
        y = c_uint32(v[0])
        e = c_uint32(0)

        while rounds > 0:
            e.value = (total.value >> 2) & 3
            for p in range(n-1, 0, -1):
                z = c_uint32(v[p-1])
                v[p] = c_uint32((v[p] - MX(z,y,total,key,p,e).value)).value
                y.value = v[p]
            z = c_uint32(v[n-1])  
            v[0] = c_uint32(v[0] - MX(z,y,total,key,0,e).value).value
            y.value = v[0]  
            total.value -= delta
            rounds -= 1
    
if __name__=="__main__":
    ans=[1374278842, 2136006540, 4191056815, 3248881376]
    c=chiper()
    c.de(len(ans),ans)
    res=c.ints2bytes(ans).decode()
    print(res)