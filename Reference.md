# Đồng hồ DS1307 Sử dụng Attiny24A Microcontroller
RTC DS1307: đồng hồ thời gian thực theo dõi ngày giờ hiện tại. Thường sử dụng trong PC, Latop, điện thoại di động, thiết bị ứng dụng hệ thống nhúng. \
Trong nhiều hệ thống ta cần lấy dấu thời gian để ghi dữ liệu từ cảm biến, toạ độ GPS, ...vì vậy sử dụng TIMER là không đủ.\
RTC chạy nguồn pin 3V, nguồn ngoài 5V chỉ để giao tiếp với VĐK. Chân phát xung SOUT, nó có thể xuất ra một trong 4 f sóng vuông sau: 1Hz, 4kHz, 8kHz, 32kHz thông qua thiết lập các bit thanh ghi bên trong.\
Một số VĐk như LPC2148, LPC1768 có RTC trên chip. Nhưng trong các vi điều khiển khác như PIC, ATmega chúng không có RTC nên phải sử dụng chip RTC bên ngoài.\
![image](https://github.com/LeChung0990/LaptrinhAttiny/assets/126931730/7cbc5729-076e-4e7c-bad6-3d8c44538fe0)
