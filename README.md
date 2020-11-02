#### description

  Linux环境，C++语言实现，运用GMP高精度算术运算库（GNU Multiple Precision Arithmetic Library）

#### main content

1. 密钥生成

   生成两个大素数p, q，计算n=p×q和(n)=(p-1)(q-1)，然后选择与(n)互素且小于(n)的整数e，计算d=e-1mod(n)，最后得到公钥{e,n}和私钥{d,n}。要求p,q至少均大于10^10,将生成的整数p、q、n、e、d分别写入文件p.txt、q.txt、n.txt、e.txt、d.txt中。注意，所有整数都必须用16进制表示。必须将整数转化成字符串后再写入文件，例如素数 p=6B1BCF(用16进制表示)，则写入文件的应是字符串"6B1BCF"（文件中无引号）而非整数6B1BCF。

2. 数据加密

   用公钥{e,n}对指定的明文进行加密。加密时先分别从指定的明文文件、密钥文件中读取有关信息，然后进行加密，最后将密文写入指定的密文文件。注意，密文(一个整数)必须用 16进制表示。必须将密文转化成字符串后再写入文件，例如密文c=154A6B(用16进制表示)，则写入文件的应是字符串"154A6B"而非整数154A6B。

3. 数字签名

   用私钥{d,n}对指定的明文进行加密。流程类似数据加密。

4. 测试数据

   明文：rsa_plain.txt文件,16 进制表示为：63727970746F677261706879 

   公钥：

   ​	rsa_np.txt文件，n = 73299B42DBD959CDB3FB176BD1 

   ​	rsa_ep.txt文件，e = 10001 

   私钥：

   ​	rsa_np.txt文件，n = 73299B42DBD959CDB3FB176BD1 

   ​	rsa_prid.txt文件，d= 63C3264A0BF3A4FC0FF0940935 

   密文：

   ​	数据加密：rsa_cipher.txt文件，16进制表示为：6326DC198AAE1DB64FDC32D440 

   ​	数字签名：rsa_sign.txt文件，16进制表示为：CA653B30EED2C6B77DCB8381F

5. 命令行具体格式：

   e3rsa -p plainfile -n nfile [-e efile] [-d dfile] -c cipherfile 

   参数：

   ​	 -p plainfile 指定明文文件的位置和名称 

   ​	-n nfile 指定存放整数 n 的文件的位置和名称 

   ​	-e efile 在数据加密时，指定存放整数 e 的文件的位置和名称 

   ​	-d dfile 在数字签名时，指定存放整数 d 的文件的位置和名称 

   ​	-c cipherfile 指定密文文件的位置和名称

6. 参考结果

   ![image](https://github.com/etamsylate-pupu/RSA/result/result1.png)

   ​																			RSA加解密及数字签名

   ![image](https://github.com/etamsylate-pupu/RSA/result/result2.png)

   ![image](https://github.com/etamsylate-pupu/RSA/result/result3.png)

   ​																		大素数p, q生成及计算n,d

#### steps:

   1. Linux虚拟机安装GMP库；
   2. 使用g++对.cpp文件进行编译生成可执行程序，格式为:g++ rsa_demo.cpp -o RSA_run -lgmp -lgmpxx；
   3. 在可执行程序文件目录下，终端命令行输入：./RSA_run -p rsa_plain.txt  -n rsa_np.txt -e rsa_ep.txt -d rsa_prid.txt -c rsa_cipher.txt

   
