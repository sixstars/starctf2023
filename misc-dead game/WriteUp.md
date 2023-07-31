# 题目解析
> As you can see，本题是整活题。

## misc解法
> 正常CTF的misc解法，已经给了充足信息了！

拿到地图文件，file一下，为MPQ压缩文件。查询资料可知可用MPQEditor解压。
解压后得到一系列文件，先尝试搜索flag和CTF相关字符串。
```bash
$ grep flag -r * && grep CTF -r *
MapScript.galaxy:string gv_flag_part1;
MapScript.galaxy:string gv_flag_part2;
MapScript.galaxy:string gv_flag_part3;
MapScript.galaxy:    gv_flag_part1 = "";
MapScript.galaxy:    gv_flag_part2 = "";
MapScript.galaxy:    gv_flag_part3 = "";
MapScript.galaxy:    gv_flag_part1 = IntToString(((UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent))*UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent)*UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsRegen, c_unitPropCurrent)+(UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent))*UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsRegen, c_unitPropCurrent)+(UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeRegen, c_unitPropCurrent))*UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsRegen, c_unitPropCurrent)*10+2*UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsRegen, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeRegen, c_unitPropCurrent)));
MapScript.galaxy:    gv_flag_part2 = (gv_flag_part2 + "K");
MapScript.galaxy:    gv_flag_part2 = (gv_flag_part2 + IntToString(TechTreeUnitCount(gv_p1_USER, "Zeratul", c_techCountQueuedOrBetter)));
MapScript.galaxy:    gv_flag_part2 = (gv_flag_part2 + IntToString((UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)/300)));
MapScript.galaxy:    gv_flag_part2 = (gv_flag_part2 + IntToString((UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent)/100)));
MapScript.galaxy:    gv_flag_part2 = (gv_flag_part2 + IntToString(((UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)+UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent))/80)));
MapScript.galaxy:    VictoryPanelAddCustomStatisticLine(StringExternal("Param/Value/2DAD549C"), StringToText(("*CTF{" + gv_flag_part1 + "_" + gv_flag_part2 + "_" + gv_flag_part3 + "}")));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(73), c_unitPropLife, c_unitPropCurrent)%71), (UnitGetPropertyInt(UnitFromId(73), c_unitPropLife, c_unitPropCurrent)%71)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(329), c_unitPropLife, c_unitPropCurrent)+1), (UnitGetPropertyInt(UnitFromId(329), c_unitPropLife, c_unitPropCurrent)+1)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(326), c_unitPropLife, c_unitPropCurrent)/10+13), (UnitGetPropertyInt(UnitFromId(326), c_unitPropLife, c_unitPropCurrent)/10+13)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, ((UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+3)*(UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+3)), ((UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+3)*(UnitGetPropertyInt(UnitFromId(1), c_unitPropMovementSpeed, c_unitPropCurrent)+3))));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(1560), c_unitPropLife, c_unitPropCurrent)%69), (UnitGetPropertyInt(UnitFromId(1560), c_unitPropLife, c_unitPropCurrent)%69)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)/2-66), (UnitGetPropertyInt(UnitFromId(1), c_unitPropLifeMax, c_unitPropCurrent)/2-66)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent)/2+4), (UnitGetPropertyInt(UnitFromId(1), c_unitPropShieldsMax, c_unitPropCurrent)/2+4)));
MapScript.galaxy:    gv_flag_part3 = (gv_flag_part3 + StringSub(gv_table, (SquareRootI(UnitGetPropertyInt(UnitFromId(192), c_unitPropLife, c_unitPropCurrent))+9), (SquareRootI(UnitGetPropertyInt(UnitFromId(192), c_unitPropLife, c_unitPropCurrent))+9)));
Triggers:            flag格式：&quot;*CTF{&quot;+flag_part1+&quot;_&quot;+flag_part2+&quot;_&quot;+flag_part3+&quot;}&quot;
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:﻿Conversation/TZeratul01/Line00047=flag的第一块碎片！真相很快就会揭开。
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_amon_comment1=毁灭已然降临！你无法从那里找到任何flag碎片！
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_amon_comment3=你的助手也离你而去，看你还如何获取flag！
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_robot_answer2=正在沟通圣堂武士文献馆······已获取第一块flag碎片！
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_robot_comment1=尊敬的泽拉图大人，您好！雷诺先生派我来协助你解 析flag碎片，希望我们的共同努力能够顺利达成目标。
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_robot_comment3=正在检索本机缓存，成功命中！已获得第二块flag碎 片！
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_zeratul_comment1=第一块flag碎片是一个文章id，指向域名https://sc2.blizzard.cn/home，创建于1668663000。
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_zeratul_comment2=第二块flag碎片被保存在我们星际专属的文件系统 内，地址为QmNnsSjJggQCT42bX2zzRpEnAcmuF4PwVy9EDRXBnq13pq。
zhCN.SC2Data/LocalizedData/ConversationStrings.txt:DOCSTR_zeratul_comment3=不，我已经窥见了最后一块flag碎片。就刻在石头 上。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00002=不错，可这些只是flag的碎片。我必须继续寻找 。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00005=或许只是只言片语。我无法合成这个flag。但是 我知道有人做得到：*CTF的出题人！
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00013=你或许能洞悉flag的真义，泽拉图——但你无法逃 脱我们都必须面对的厄运！
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00027=flag的第二块碎片！任务就要完成了。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00046=flag被分为几块残片，保存在不同的圣殿里。我 必须收集它们，再尽快逃脱这个世界。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00048=flag的最后一块碎片。上面写着：<n/><IMG path="Assets\Textures\btn-number-8.dds" height="32" width="32" alignment="absolutemiddle"/><IMG path="Assets\Textures\btn-number-1.dds" height="32" width="32" alignment="absolutemiddle"/> <IMG path="Assets\Textures\ui_btn_generic_exclemation.dds" height="32" width="32" alignment="absolutemiddle"/><IMG path="dds\3e5.dds" height="32" width="32" alignment="absolutemiddle"/><IMG path="dds\795.dds" height="32" width="32" alignment="absolutemiddle"/><s val="Kicker_Refinery">R</s><s val="Warning">d</s>
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00054=凯瑞甘和我都是为flag而来。有了您的帮助，希 望我们能够抢先一步。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00064=我无法合成flag碎片。但我知道有人做得到：*CTF的出题人。
zhCN.SC2Data/LocalizedData/GameStrings.txt:LoadingScreen/Tip=只有找到三块flag碎片，才能合成真正的flag。
zhCN.SC2Data/LocalizedData/GameStrings.txt:Param/Value/2DAD549C=解析得到flag
zhCN.SC2Data/LocalizedData/TriggerStrings.txt:Variable/Name/2722A73B=flag_part3
zhCN.SC2Data/LocalizedData/TriggerStrings.txt:Variable/Name/8D860E6B=flag_part1
zhCN.SC2Data/LocalizedData/TriggerStrings.txt:Variable/Name/C307F8C1=flag_part2
MapScript.galaxy:    VictoryPanelAddCustomStatisticLine(StringExternal("Param/Value/2DAD549C"), StringToText(("*CTF{" + gv_flag_part1 + "_" + gv_flag_part2 + "_" + gv_flag_part3 + "}")));
Triggers:        <ExpressionText>*CTF{~A~_~B~_~C~}</ExpressionText>
Triggers:            flag格式：&quot;*CTF{&quot;+flag_part1+&quot;_&quot;+flag_part2+&quot;_&quot;+flag_part3+&quot;}&quot;
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00005=或许只是只言片语。我无法合成这个flag。但是 我知道有人做得到：*CTF的出题人！
zhCN.SC2Data/LocalizedData/GameStrings.txt:Conversation/TZeratul01/Line00064=我无法合成flag碎片。但我知道有人做得到：*CTF的出题人。
```
可以发现，flag的格式，每块flag碎片的信息都有。由于信息主要在对话中给出，可以着重分析`zhCN.SC2Data/LocalizedData/ConversationStrings.txt`和`zhCN.SC2Data/LocalizedData/GameStrings.txt`。
第一块flag碎片是一个文章id，指向域名https://sc2.blizzard.cn/home，创建于1668663000。1668663000明显是一个时间戳，转换为北京时间为2022年11月17日13:30:00。域名https://sc2.blizzard.cn/home曾经是星际国服官网，现已无法访问，会302重定向至网易退款说明，对话中也给出了这一点。对于已经网页的历史版本，我们有一定可能在[web archive](http://web.archive.org/)中找到备份，这也是对话提示的`档案馆`。经搜索，可以找到历史文章备份http://web.archive.org/web/20230123041558/https://sc2.blizzard.cn/articles/1001/80867，这是暴雪停服公告，满足题目条件，flag碎片为文章id即80867。
第二块flag碎片被保存在我们星际专属的文件系统内，地址为QmNnsSjJggQCT42bX2zzRpEnAcmuF4PwVy9EDRXBnq13pq。这里指星际文件系统IPFS，是一个分布式文件系统，由多个网络节点组成。根据对话（或自己测试）知道题目给的文件系统地址访问不了，因为节点只在出题人本地有，未发布至全网。那么有没有可能通过文件地址恢复文件内容呢？由于flag碎片很短，这是可行的。IPFS的地址是base58编码的multi-hash，解码后去除开头的`'\x12\x20'`标识符，剩下32位为sha2-256哈希值，因此我们可以通过爆破哈希值来恢复文件内容。不过IPFS并非直接对文件内容做哈希，还存在一些元数据，可以通过本地add文件然后`ipfs object get <key>`来得到具体的元数据格式。对于哈希爆破，这里推荐使用hashcat的hex模式，具体参加脚本`brute_part2.py`。hashcat爆破得到`$HEX[0a0b080212054b313131351805]`，提取其中的文件内容为`K1115`。
第三块flag碎片为一堆xml格式数据，在没有银河编辑器的情况下无法直接看到，不过可以手动分析。`<s>`标签只是设置字体格式，无影响可以去除。`<img>`标签为图片，而后两张图片在MPQ解压已经得到。前三张图片为星际自带资源，不过根据文件名猜出8和1，最后的`ui_btn_generic_exclemation.dds`猜不出来，为了方便misc解法，我在MPQ的dds目录下备份了同名文件，可以看到是感叹号，最终组合得到flag碎片`81!zZ@Rd`。
根据flag格式可以将flag碎片拼接得到最终flag：`*CTF{80867_K1115_81!zZ@Rd}`，意为：国服的停运将杀死暴雪。


## 纯游戏解法
> 适合白金及以上级别的指挥官挑战，也考虑给选手无聊时打发时间。

通关游戏，即可在结算页面获得flag！纯游戏通关需要一些技巧，高玩可以自行尝试，其他选手可以观看视频https://www.bilibili.com/video/BV1uV411G7vd了解。由于没禁存档，采用save-load大法的话难度会低一些。

# 解题脚本
brute_part2.py

# 花絮
1. 本题没有像其他比赛一样提前让选手准备游戏环境，就是为了给传统misc解法留出时间。毕竟国服没了，几十个G的游戏本体估计几个小时都下不完。
2. 本题原计划做成只有最后一个版本的国服客户端才能打开的地图，可惜没有好的实现方法，只能这样了。
3. 本地地图改自“自由之翼”战役泽拉图线第一关——“黑暗耳语”，预言了毁灭与终焉。默认残酷难度，同时调整了敌我双方实力，不过给泽拉图升级了三攻三盾，单人RPG玩法。
4. 本题添加了突变因子“强磁雷场”，地图上布满了麦格天雷，进一步增大了游戏难度。虽然泽拉图很强，但最多只能承受两发麦格天雷。（本来想再添加一个更契合主题的突变“暴风雪”，可惜没时间调好兼容性，遂作罢）
5. 虽然代码中加了一些作弊检测，但肯定不可能面面俱到，比如减伤100%就检测不出来。至于作弊码，确实是疏漏没考虑到，毕竟平时不用。。。
6. 为了能让纯游戏解法能拿到flag，本题在游戏代码中进行了所有的flag生成工作。为了增加逆向和直接用银河编辑器改代码的难度，本题通过删配置文件进行了简单的混淆（不过也能通过复制原版战役配置恢复╮(╯▽╰)╭，这就管不了了），关于具体代码的一些分析可以参考[@探姬_Official](https://www.bilibili.com/video/BV1hp4y1G7uc)的分析。
7. 本来这题出在二月份整活效果更好，不过似乎因为XCTF赛程安排推到了现在，感觉马上暴雪都要秽土转生了。。。