# Code dùng trình biên dịch avr-gcc từ arduino để biên dịch cho Attiny24A

    Một thư mục Template cơ bản như sau:
    |lib.c
    |lib.h
    |main.c
    |Makefile
    |README --> THIS FILE
**Trong file main.c sẽ khai báo tên chip sử dụng, tần số hoạt động**
<img src= Image/template_code.png width = "250" >

**Mở CMD và gõ make để biên dịch chương trình**

<img src= Image/build_cmd.png width = "250" >


### I2C_24C04

Ghi số 52 vào vị trí 0x00, 34 vào 0x10, 35 vào 0x20, 36 vào 0x30

<img src= Image/24C04-2.png width = "250" >
<img src= Image/24C04.png width = "250" >
