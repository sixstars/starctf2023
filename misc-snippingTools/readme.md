*CTF{cve-2023-28303-windows-snipping-tool-is-not-secure-4E9019139D9A}

## solution

CVE-2023-28303

相关网站：

* https://en.wikipedia.org/wiki/ACropalypse
* https://msrc.microsoft.com/update-guide/vulnerability/CVE-2023-28303
* https://acropalypse.app/
* https://twitter.com/David3141593/status/1638222624084951040

本题两个难点。一是得知本题的内容是CVE-2023-28303，二是找到/编写PoC。

acropalypse.app是Pixel的PoC，如果要应用于Windows，需要一些修改（https://twitter.com/David3141593/status/1638272152087547911 ）。

可以在GitHub直接找到可以应用于Windows的PoC，https://github.com/frankthetank-music/Acropalypse-Multi-Tool