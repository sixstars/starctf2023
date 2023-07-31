import os

for i in range(1,7):
    os.system('hashcat -a 3 -m 1400 06b7801b9797a0609f452524e2472f37d2b74913d5a82842d7ddb3551e0462be --hex-charset 0a%02x080212%02x%s18%02x'%(i+6,i,'?a'*i,i))