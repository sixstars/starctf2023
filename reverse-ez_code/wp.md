简单的powershell混淆技术
解法：
    1. 在powershell终端中运行chall,使得变量与定义生效
    2. 输入"IEX ${@*}"，获取源代码tea_chall.py
    3. 魔改TEA算法，直接解密即可，代码见 tea_sol.py

