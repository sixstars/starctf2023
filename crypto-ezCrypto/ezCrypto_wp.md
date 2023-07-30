## ezCrypto MD

​	  压缩包提供了两个文件，分别是`ezCrypto.py` 与 `output.txt` 。`output.txt`为加密后输出的相关信息，可以先不用理会，先分析`ezCrypto.py`。

### 1. 程序分析

​	  开头给了3个`string`类型值，分别是`characters`，`digits`，`ascii_letters`。结合代码语句：

```python
characters = string.printable[:-6]
digits = string.digits
ascii_letters = string.ascii_letters
```

​	  可以很轻松判断出`characters`是所有可见字符组成的字符串，`digits`是数字，`ascii_letters`则是字母。

​	

​	  接着是`Ran_str()`函数：

``` python
def Ran_str(seed : int, origin: str):
    random.seed(seed)
    random_sequence = random.sample(origin, len(origin))
    return ''.join(random_sequence)
```

​	  阅读代码，可以知晓该函数通过传入的种子`seed`，对传入的字符串`origin`进行了随机化处理，并返回随机后的字符串。



​	  然后是要求输入种子`rseed`，并根据`rseed`，对`characters`随机化处理后得到3个`map_string`字符串的代码，同时也对`rseed`做了范围限制：

``` python
rseed = int(input())
assert rseed <= 1000 and rseed >= 0

map_string1 = Ran_str(rseed, characters)
map_string2 = Ran_str(rseed * 2, characters)
map_string3 = Ran_str(rseed * 3, characters)
```



​	接着是3个功能类函数：

``` python
def util(flag):
    return flag[9: -1]

def util1(map_string: str, c):
    return map_string.index(c)

def str_xor(s: str, k: str):
    return ''.join(chr((ord(a)) ^ (ord(b))) for a, b in zip(s, k))
```

​	  各自实现了取字符子串、返回目标字符在目标字符串的索引值，以及进行字符串异或的功能。



​	  该程序的主要函数`mess_sTr()`紧随其后：

``` python
def mess_sTr(s : str, index : int):
   
    map_str = Ran_str(index, ascii_letters + digits)
    new_str = str_xor(s, map_str[index])
    
    if not characters.find(new_str) >= 0:
        new_str = "CrashOnYou??" + s
    
    return new_str, util1(map_str, s)
```

​	  根据先前提到的几个函数的作用，`mess_sTr()`应该实现了这样的一个功能：

 1. 接受传入的字符串参数`s`与整型参数`index`。

 2. 将整型参数`index`作为种子，对字母与数字组成的字符串进行随机化处理。

 3. 将传入的字符串参数`s`，与随机化后的`map_str`在整型参数`index`上的值进行异或处理。

 4. 返回异或后的（或进入`new_str = "CrashOnYou??" + s`）的字符串`new_str`，以及`s`在`map_str`上的索引值`util1(map_str, s)`。

    显然，根据这里面涉及到的`map_str[index]`与`util1(map_str, s)`，以及字符串异或、返回字符串索引值的功能，可以初步推断`s`是长度为1的字符串。

    

    该程序的另一个主要函数是`crypto_phase1()`：

    ``` python
    def crypto_phase1(flag):
        flag_list1 = util(flag).split('_')
        newlist1 = []
        newlist2 = []
        index = 1
        k = 0
        for i in flag_list1:
            if len(i) % 2 == 1:
                i1 = ""
                for j in range(len(i)):
                    p, index = mess_sTr(i[j], index)
                    i1 += p
               
                p, index = mess_sTr(i[0], index)
                i1 += p
                
                i1 += str(k)
                k += 1
                newlist1.append(i1)
            
            else:
                i += str(k)
                k += 1
                newlist2.append(i)
        
        return newlist1, newlist2
    ```

    ​	该函数传入一个`flag`参数，应该可以很轻松判断出该参数是字符串。然后利用`util(flag).split('_')`对`flag`取子串并分段，赋值给`list`类型变量`flag_list1`。

    ​	然后进入循环。可以很明显的发现，循环对`flag_list1`中长度为奇数偶数的字符串进行了分别处理，并将处理后的字符串分别加入`newlist1`，`newlist2`。此外还有一个整型变量`k`表示当前处理的字符串位于`flag_list1`的索引值。

    ​	对于偶数长度字符串，只是简单的在字符串后面附加上当前的`k`，然后加入`newlist2`。

    ​	对于奇数长度字符串，则是将字符串的每一字符与`index`一同传入`mess_sTr()`，得到一个处理过后的字符串值。将这些处理后的字符串值连在一起，最后再进行一次对于首字符的`mess_sTr()`处理，并附加当前的`k`，即可加入`newlist1`。

    ​	对于奇数长度字符串的处理，印证先前在`mess_sTr`讨论的长度为1的字符串情况。并且对于整型变量`index`，其初始值为1，但会随着`mess_sTr()`的处理而发生变化，变化后的值为传入的字符在`mess_sTr()`中的`map_str`的索引值。



​		对于函数`crypto_phase2()`与`crypto_phase3()`：

``` python
def crypto_phase2(list):
    newlist = []
    for i in list:
        str = ""
        for j in i:
            str += map_string1[util1(map_string3, j)]
           
        newlist.append(str)
    return newlist

def crypto_phase3(list):
    newlist = []
    for i in list:
        str = ""
        for j in i:
            str += map_string2[util1(map_string3, j)]
            
        newlist.append(str)
    return newlist
```

​		其实可以发现实现的效果类似，是获取`map_string3`在`j`值上的索引值，将索引值传入`map_string1`或`map_string2`，最后构筑成新的`newlist`的过程，本质上是一种映射·。



​	最后的`crypto_final()`，显然实现了一种逆向的功能：

``` python
def crypto_final(list):
    str=""
    for i in list[::-1]:
        str += i
    return str
```



### 2. 解题步骤

​	关注一下主函数：

``` python
if __name__ == '__main__':
    format="sixstars{XXX}"
    flag="Nothing normal will contribute to a crash. So when you find nothing, you find A Crashhhhhhhh!!! "   
    
    
    flaglist1, flaglist2 = crypto_phase1(flag)
    cipher = crypto_final(crypto_phase3(crypto_phase2(flaglist1) + flaglist1) + crypto_phase2(crypto_phase3(flaglist2)))
    
    print("map_string2: " + map_string2)
    print("cipher: " + cipher)
```

​	以及`output.txt`：

``` txt
map_string2: \W93VnRHs<CU#GI!d^7;'Lyfo`qt68&Y=Pr(b)O2[|mc0z}BvKkh5~lJeXM-iNgaTZ]*4F?upw>A,x@DQ.Sj:_$E/%"+{1
cipher: &I1}ty~A:bR>)Q/;6:*6`1;bum?8i[LL*t`1;bum?8i[LL?Ia`1;bum?8i[LL72;xl:mvHF"z4_/DD+c:mvHF"z4_/DDzbZ:mvHF"z4_/DDr}vS?
```

​	可以判断`flag`的格式是`sixstars{}`，将`flag`先做`crypto_phase1()`处理，得到奇偶字符串各自对应的`flag_list1`与`flag_list2`，接着进行其他crypto处理。



​	由于`map_string2`是已经给出的，并且`map_string2`是由输入的`rseed`种子确定的，在加上范围限制：

``` python
assert rseed <= 1000 and rseed >= 0
```

​	可以先行对`rseed`进行遍历，得到`rseed`值与其他`map_string`值。



​	简单遍历如下：

``` python
f = open("ezCrypto\\output.txt","r")
map_string2 = f.readline().split("map_string2: ")[1].strip()

for rseed in range(1001):
    map_string = Ran_str(rseed * 2, characters)
    if map_string2 == map_string:
        print(rseed)
        break
```

​	得到`rseed`值为`667`，同时能得到`map_string1`，`map_string3`的值：

``` txt
map_string1: d*T[RJDKkbZ>"Fs\&X}Q6:h7a{VUj#=Y1tLI~P^qBg9A.)Mz@frvGwn<ie,y|m;'3x54]8-p%W(oS!0lN`?2+H/_Euc$CO
map_string3: .2K6b@/~5+=l<7wXj8TaJ?]Z,CMRkY&gG(}tsf)Du^OUx-qdQNiyV$01L["moA*3P'IF#pnhe`\;v>H:z%!c{|WEBS94_r
```



​	`cipher`的最后一步用`crypto_final`进行了逆向：

``` python
cipher = crypto_final(crypto_phase3(crypto_phase2(flaglist1) + flaglist1) + crypto_phase2(crypto_phase3(flaglist2)))
    
```

​	也即`cipher`前面是`crypto_phase2(crypto_phase3(flaglist2)))`所代表的部分，后面是`crypto_phase3(crypto_phase2(flaglist1) + flaglist1)`所代表的部分，注意每个`list`都会被逆向。即若``crypto_phase2(crypto_phase3(flaglist2)))``是`["123", "45", "678"]`，则`cipher`的前面应当是`["678", "45", "123"]` 

​	但由于`cipher`中出现了两次`flaglist1`，以及`crypto_phase2`，`crypto_phase3`均为不改变列表字符串长度的映射函数，所以可以通过遍历得到`flaglist1`与`flaglist2`。

​	通过下列操作可以得到`flaglist1`对应的密文及其长度：

``` python
i1_ori = len(cipher) - 1
i2_ori = len(cipher) - 2

i1 = i1_ori
i2 = i2_ori


while True:
    p1 = map_string3[util1(map_string1, map_string3[util1(map_string2, cipher[i1])])]
    p2 =  map_string3[util1(map_string2, cipher[i2])]
    
    if p1 != p2:
        i1 = i1_ori
        i2_ori -= 1
        i2 = i2_ori
        continue
    
    elif p1 == p2:
        
        if i1 - 1 == i2_ori:
            break
        else:
            i1 -= 1
            i2 -= 1
            continue
            
print(i1)
print(i2) 
print(cipher[i1:])
print(len(cipher[i1:]))
```

​	解得`crypto_phase3(crypto_phase2(flaglist1))`对应的密文为：`:mvHF"z4_/DD+c:mvHF"z4_/DDzbZ:mvHF"z4_/DDr}vS?`，长度为`46`，则`crypto_phase2(crypto_phase3(flaglist2))`对应的密文为`&I1}ty~A:bR>)Q/;6:*6`，长度为`20`。

​	仿照上述函数，进行逆映射，可以得到`flaglist1`为`CrashOnYou??FRCrashOnYou??nw3CrashOnYou??TDa>0`，`flaglist2`为`cR7PtO5ln4s0m32F1nD1`。

​	但其实两个`flaglist`都是`list`类，所以还需要继续重新拼接。

​	

​	根据之前对`crypto_phase1`的分析，偶数长度字符串的相关加密算法比较简便，可以先尝试解出这部分，即`flaglist2`对应的密文。

``` python
i += str(k)
k += 1
newlist2.append(i)
```

​	注意到`flaglist2`其中的每个字符串结尾应当都是对应的索引值。注意到``cR7PtO5ln4s0m32F1nD1`` 结尾是`1`，显然只能是`flaglist2`中有一个字符串在`flag_list1 = util(flag).split('_')`的索引为1的地方，所以``cR7PtO5ln4s0m32F1nD1``之中的另一个`1`必是含于原字符串的。而`2`又是与`3`紧贴，且`2`与刚刚讨论的`F1nD1`紧贴，所以`2`也必是索引。所以得到`flag_list1 = util(flag).split('_')`的一个部分:`F1nD`，索引为1，且恰好长度为偶数。

​	继续往前推，由于是偶数长度字符串，再结合索引出现的位置，所以索引为`2`对应的字符串可能是`m3`，`s0m3`，也可能是`cR7PtO5ln4s0m3`。此时无法确定，可以先去分析`flaglist1`，根据索引值以互相验照。当然，也可以大胆猜测是`s0m3`（因为贴近some这个单词），从而推出`cR7PtO`（贴近crypto这个单词），不过这个属于是灵光一现了。

​	（出题人正是在这里考虑不周，以为只能得出`s0m3`这一种结果，但其实3种结果中有2种在之后的计算中都符合，所以导致本题有2解的情况。为此向参赛选手们表示深深的歉意）



​	于是考虑`flaglist1= CrashOnYou??FRCrashOnYou??nw3CrashOnYou??TDa>0`，结合`mess_sTr()`算法：

``` python
def mess_sTr(s : str, index : int):
   
    map_str = Ran_str(index, ascii_letters + digits)
    new_str = str_xor(s, map_str[index])
    
    if not characters.find(new_str) >= 0:
        new_str = "CrashOnYou??" + s
    
    return new_str, util1(map_str, s)
```

​	并由主函数的提示：

``` python
Nothing normal will contribute to a crash. So when you find nothing, you find A Crashhhhhhhh!!!
```

​	显然`CrashOnYou??`就是对应`mess_sTr()`的`if not`情况。（可能存在完全不进入`if not`的情况，但是主函数的提示ban掉了这个可能）。

​	首先还是先找数字对应的索引，`flaglist1`的结尾是`0`，所以必定有索引值为0的字符串，所以先前`flaglist2`的`m3`可能性不存在（仔细想想为什么）。然后发现有数字`3`，先前在`flaglist2`的讨论中没有出现索引为3的可能，所以这里含有索引值为3的字符串。

​	所以`flaglist1`可以分段为`CrashOnYou??TDa>`与`CrashOnYou??FRCrashOnYou??nw`，不妨分别记为`str1`与`str2`。

​	`str1`的第一个字母显然为`T`，再根据`crypto_phase1`中的`index`初始值为`1`，将结果输入`mess_sTr()`，得到新的`index`为`38`。将`index = 38`输入`Ran_str`，得到`map_str = OABWUgexSD2cMQkLwurvNtE5zofHFj7qhamlZR69PC3pdiy8Y0bK4TnIsJXVG1`，于是`map_str[index]`为`6`，将`D`与`6`异或得到`r`，所以新的`index`为`18`。由此逐一类推。

​	最终得到`str1`为`TrY`，`str2`为`F4n`，分别对应索引为0与索引为3。

​	

​	结合先前`flaglist2`的讨论，则索引为1的字符串为`F1nD`，并且可以`cR7PtO5ln4s0m3`作为索引2的字符串，或者`s0m3`作为索引2的字符串，然后`ln`作为索引为4的字符串，`cR7PtO`作为索引为5的字符串。

​	

所以最终的flag为`sixstars{TrY_F1nD_s0m3_F4n_ln_cR7PtO}`或`sixstars{TrY_F1nD_cR7PtO5ln4s0m3_F4n}`。

​	

​	