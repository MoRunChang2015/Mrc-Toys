MD5 Algorithm
=======
+ Author: MoRunChang, SYSU
+ Environment: Ubuntu 16.04, G++ 5.4.0

### usage
```
make
bin/MD5
```

### Testsuit
+ **Request Google Test**

```
make test/md5EncryptTest
```

+ **Result**

```
[==========] Running 3 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 3 tests from Md5EncryptTest
[ RUN      ] Md5EncryptTest.EmptyInput
[       OK ] Md5EncryptTest.EmptyInput (0 ms)
[ RUN      ] Md5EncryptTest.SimpleString
[       OK ] Md5EncryptTest.SimpleString (0 ms)
[ RUN      ] Md5EncryptTest.LongString
[       OK ] Md5EncryptTest.LongString (0 ms)
[----------] 3 tests from Md5EncryptTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test case ran. (0 ms total)
[  PASSED  ] 3 tests.

```

### Reference
+ [wikipedia](https://en.wikipedia.org/wiki/MD5)
