# 题目解析
> 本题参考了RCTF 2022的S2DH，没想到正好是SIDH算法被攻破一周内纪念o(￣▽￣)ｄ 。

超奇异同源Diffie-Hellman(SIDH)秘钥交换算法，作为一种后量子安全秘钥交换协议，于2022年被攻破。

题目对于SIDH算法默认超奇异椭圆曲线 $y^2=x^3+6x^2+x$ 上的点做了一个明显的线性变换 $(x,y)\rightarrow(x',y'+1)$ 。（本来这里想弄复杂一点的，一看gcccd还是零解，算了）线性变换导致Castryck-Decru-SageMath的脚本和S2DH那题的脚本都不能用，根据论文自己写一个新的通用脚本比较麻烦，因此最好的办法是直接变换回去。简单实验可以发现，超奇异椭圆曲线上的点，超奇异椭圆曲线的同源，超奇异椭圆曲线经过同源映射后得到的点，都保持了 $(x,y)\rightarrow(x',y'+1)$ 线性变换这一性质，因此可以较为轻松地还原，从而直接利用Castryck-Decru-SageMath的攻击脚本求解私钥T。

此外，题目只给出了曲线上的点，并未给出曲线表达式。但可以发现，同源映射后椭圆曲线的参数只有一次项和常数项发生了变化，因此可以根据曲线上的两个点直接计算出来。

最后是恢复S，关于(3,3)-同源链暂时没有什么可直接用的脚本，所以题目给了秘钥交换后计算出的新点Pab,Qab。根据同源映射的线性性质： $E_1(P+S \cdot Q) \rightarrow E_2(\varphi(P)+S\cdot\varphi(Q)) = E_2(\varphi(P))+S\cdot E_2(\varphi(Q))$ 以及核的性质 $E_1(P+S\cdot Q) \rightarrow E_2(\varphi(P)+S\cdot\varphi(Q)) = E_2(0)$ ，可得 $E_2(\varphi(P))=-S\cdot E_2(\varphi(Q))$ 。由于题目中椭圆曲线的阶都是光滑的，可直接求解离散对数恢复S。

# 解题脚本
solution.sage（需放于[Castryck-Decru-SageMath](https://github.com/GiacomoPope/Castryck-Decru-SageMath)项目下）

# 注意事项
虽然Castryck-Decru-SageMath项目写着用sage9.5开发，但是用sage9.5跑起来有些地方不兼容需要修改，最好使用sage10。
