# GoGpt

solved by 83 teams

This is a challenge provided by GPT-4, this is our conversation.

> Please provide me with and easy reverse challenge in go in CTF

Then I added

> The encryption method seems to be too easy, please give me a more difficult one

And it forms this challenge :P

(This challenge may be reported malicious, but it does not contain any malicious code)

## solve

the binary is a stripped binary written in go, but using several IDA golang plugins can help you retrive the symbols. [link](go parser)

After that, life will be easier. The whole encryption method is just using xor