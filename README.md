# CrossNext for Seeed reTerminal Sticky (Tiếng Việt)

Bản phân phối **CrossNext** được tối ưu hóa cho phần cứng **Seeed reTerminal Sticky** (ESP32-S3), tích hợp **bản địa hóa Tiếng Việt hoàn chỉnh 100%**, khắc phục lỗi hiển thị chữ hoa/thường tiếng Việt và tinh chỉnh các cài đặt phần cứng phù hợp với Sticky.

---

## 🌟 Tín chỉ & Lời cảm ơn (Credits & Acknowledgements)

Dự án này được xây dựng và phát triển dựa trên các công trình nguồn mở xuất sắc:

- **[CrossNext](https://github.com/manuelx900/CrossNext)** bởi **[manuelx900](https://github.com/manuelx900)**: Tác giả firmware gốc CrossNext với giao diện Toybox, ứng dụng PET, kho sách, công cụ học tập và trò chơi e-ink phong phú.
- **[CrossPoint Reader](https://github.com/crosspoint-reader/crosspoint-reader)** bởi **CrossPoint Organization**: Nền tảng hệ điều hành đọc sách nguồn mở e-reader cho ESP32.
- **[CrossPlay](https://github.com/ma-r-s/crossplay)** bởi **ma-r-s** và các biến thể cộng đồng ([CrossInk](https://github.com/uxjulia/CrossInk), [CrossDiTo](https://github.com/dito94/CrossDiTo), [VCodex](https://github.com/franssjz/cpr-vcodex)).
- **[Seeed Studio](https://www.seeedstudio.com/)**: Nhà sản xuất thiết bị phần cứng **reTerminal Sticky** (ESP32-S3, màn hình E-Ink 3.7", cảm ứng, Wi-Fi/BLE) và bộ SDK [freeink-sdk](https://github.com/Seeed-Studio).

---

## ✨ Điểm nổi bật trên phiên bản này (Features)

1. **Bản địa hóa Tiếng Việt hoàn chỉnh (100% Vietnamese Localization)**:
   - Toàn bộ 4,063 chuỗi ký tự giao diện (Settings, Reader, Bookshelf, Tools, Games, PET, Quản lý từ vựng, Thống kê...).
   - Bổ sung tùy chọn ngôn ngữ **Tiếng Việt** trực tiếp trong menu **Cài đặt > Hệ thống > Ngôn ngữ**.

2. **Khắc phục lỗi hiển thị chữ tiếng Việt (Typography & Casing Fix)**:
   - Sửa thuật toán xử lý UTF-8 multi-byte trong `copyNatural()` và `copyCaps()` tại `PixelText.h`. Toàn bộ các nguyên âm tiếng Việt có dấu (À..Ỵ, Đ, Ă, Â, Ê, Ô, Ơ, Ư) được chuyển đổi hoa/thường chuẩn xác, không còn tình trạng bị xen kẽ chữ cao chữ thấp (như `CỜ vua`, `CỜ Đam`, `ChiẾn hẠm`).
   - Sửa thanh tiêu đề thư mục (`ShelfFolderActivity.cpp`) hiển thị chuẩn uppercase tiếng Việt (`TRÒ CHƠI`, `ỨNG DỤNG`).
   - Chuẩn hóa tên ứng dụng/trò chơi sang dạng viết hoa đầu từ tự nhiên (`Cờ vua`, `Cờ đam`, `Chiến hạm`, `Mật mã (The Intercept)`, `Dò mìn`...).

3. **Bàn phím & Nhãn điều khiển**:
   - Giữ nguyên các ký tự và phím chức năng quốc tế trên bàn phím: `OK`, `Space`, `Shift`, `Del`, `ABC`, `?123` (không bị dịch sai thành "Được rồi" hay "không gian").

4. **Tối ưu hóa cho Seeed reTerminal Sticky**:
   - Ẩn các cài đặt đèn nền / đèn vàng không có trên phần cứng Sticky.
   - Sửa lỗi nút Home trong trình đọc sách (Epub Reader Home fix).
   - Thiết lập cấu hình phím vật lý và quản lý nguồn tối ưu cho pin sạc reTerminal Sticky.

---

## 🚀 Hướng dẫn nạp Firmware (Flashing Instructions)

### Tải Firmware
Tải các file binary từ mục [Releases](https://github.com/loitq07/crossnext-sticky/releases):
- `CrossNext-Sticky-v1.4.0-sticky-vi-factory.bin` (Bản nạp đầy đủ từ địa chỉ `0x0000`, bao gồm Bootloader + Partition table + Boot_app0 + App)
- `CrossNext-Sticky-v1.4.0-sticky-vi-app.bin` (Chỉ phân vùng ứng dụng tại `0x10000`)
- `CrossNext-Sticky-v1.4.0-sticky-vi.zip` (Gói cài đặt đầy đủ kèm hướng dẫn và patch)

### Cách 1: Nạp qua `esptool.py` (Khuyên dùng)
Kết nối reTerminal Sticky với máy tính qua cáp USB-C, xác định cổng COM (ví dụ: `COM9` trên Windows hoặc `/dev/ttyUSB0` trên Linux/macOS):

```bash
esptool.py --chip esp32s3 --port COM9 --baud 921600 write_flash 0x0 CrossNext-Sticky-v1.4.0-sticky-vi-factory.bin
```

Sau khi nạp xong, xóa phân vùng coredump nếu cần:
```bash
esptool.py --chip esp32s3 --port COM9 --baud 921600 erase_region 0xff0000 0x10000
```

### Cách 2: Nạp qua trình duyệt (Web Flasher)
Bạn có thể sử dụng [ESP Web Tools](https://espressif.github.io/esptool-js/) trên Chrome/Edge và chọn nạp file `CrossNext-Sticky-v1.4.0-sticky-vi-factory.bin` tại địa chỉ `0x0`.

---

## 🛠️ Hướng dẫn Build từ mã nguồn

```bash
# Cài đặt PlatformIO
pip install platformio

# Tạo và kích hoạt môi trường ảo
python -m venv .venv
source .venv/bin/activate  # Hoặc .venv\Scripts\activate trên Windows

# Sinh file mã hóa ngôn ngữ C++
python scripts/gen_i18n.py

# Biên dịch firmware cho reTerminal Sticky
platformio run -e gh_release_sticky
```

---

## 📄 Giấy phép (License)

Dự án tuân theo giấy phép nguồn mở của các dự án gốc CrossNext và CrossPoint (GPL / MIT tương ứng). Vui lòng tôn trọng bản quyền và ghi nhận tác giả gốc khi phân phối lại.
