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


## OLED LCD 128x64

<img src= Image/oled.png width = "250" >
Hiển thị số lên LCD OLED
<img src= Image/oled2.png width = "250" >
Kết quả
<img src= Image/oled3.png width = "250" >
Bảng giá trị hiển thị OLED cơ bản
<img src= Image/oled4.png width = "250" >
Dưới đây là bảng chi tiết
<img src= Image/oled5.png width = "250" >

Kết quả
<img src= Image/oled6.png width = "250" >

## OLED LCD 128x64 BIGNUMBER
Kết quả
<img src= Image/oled7.png width = "250" >

<img src= Image/oled8.png width = "250" >

