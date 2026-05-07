# Hệ Thống Quản Lý Shop Quần Áo 4 IN 1:

Dự án bài tập lớn môn Kỹ thuật lập trình (Ngôn ngữ C). 
Chương trình hỗ trợ quản lý toàn diện các hoạt động của một cửa hàng thời trang, từ thiết kế sản phẩm, kho hàng đến nhân sự và tài chính 

## Các Chức Năng Chính

Chương trình bao gồm 4 phân hệ chính:

1. Quản lý Thiết Kế & Sản Phẩm Mới (Chic Flow):
    * Sử dụng cấu trúc dữ liệu Danh sách liên kết đơn (Linked List).
    * Nhập hàng từ kho lên cửa hàng để trưng bày.
    * Bán hàng và áp dụng chương trình giảm giá.
    * Thống kê sản phẩm bán chạy nhất.
    * Chỉnh sửa sản phẩm
    * Xóa sản phẩm mà cửa hàng không còn bán.
    * Sắp xếp sản phẩm theo giá tiền.
    * Lưu trữ và đọc dữ liệu từ file `chic_flow_data.txt`.

2. Quản lý Kho Hàng & Bán Hàng (Inventory):
    * Quản lý hàng tồn kho theo tên sản phẩm và size.
    * Tự động cộng dồn số lượng nếu sản phẩm đã tồn tại.
    * Cảnh báo hàng sắp hết và hỗ trợ nhập bổ sung.
   
3. Quản lý Nhân Viên (HR):
    * Quản lý danh sách nhân sự: Họ tên, tuổi, chức vụ, giới tính, hệ số lương.
    * Thêm, xóa và hiển thị danh sách nhân viên.
    * Chức vụ khác nhau --> Hệ số lương khác nhau.
    * Tính lương cho từng nhân viên: lương cơ bản x hệ số lương.
      
4. Quản lý Tài Chính & Thu Chi (Finance):
    * Ghi chép lịch sử thu/chi thủ công.
    * Tự động ghi lại các giao dịch bán hàng từ các module khác.
    * Tự đông ghi lại các khoản chi từ việc nhập hàng vào kho.
    * Lập báo cáo doanh thu, chi phí và lợi nhuận tổng quát.
    * Dữ liệu được lưu trữ trong file `thuchi.txt`.

## Công Nghệ Sử Dụng

*Ngôn ngữ: C (Chuẩn C99).
*Môi trường: Console Windows.
*Thư viện: `stdio.h`, `stdlib.h`, `string.h`, `windows.h`.
* **Tính năng đặc biệt:** Sử dụng mã màu ANSI để làm đẹp giao diện console.

## Cấu Trúc Thư Mục

* `main.c`: Chứa luồng xử lý chính và menu điều hướng của chương trình.
* `shop.c`: Chứa định nghĩa chi tiết của tất cả các hàm xử lý logic và giao diện.
* `shop.h`: File header chứa các định nghĩa cấu trúc (struct), hằng số và nguyên mẫu hàm.
* `chic_flow_data.txt`: File lưu trữ dữ liệu sản phẩm thiết kế.
* `thuchi.txt`: File lưu trữ lịch sử giao dịch tài chính.

## Hướng Dẫn Cài Đặt và Chạy

1.  **Tải code:** Clone repository này về máy hoặc tải file ZIP.
2.  **Biên dịch:** Sử dụng trình biên dịch C (như C-Free, Dev-C++, hoặc GCC).
    * Nếu dùng GCC: `gcc main.c shop.c -o shop_management`
3.  **Chạy:** Thực thi file `.exe` vừa tạo.

## Hướng Dẫn Cho Thành Viên Nhóm (Git)

Mọi người thực hiện quy trình sau để tránh xung đột code:

1.  **Trước khi làm:** Luôn chạy `git pull origin main` để lấy code mới nhất.
2.  **Sau khi sửa:** * `git add .`
    * `git commit -m "Mô tả việc bạn đã làm"`
    * `git push origin main`

---
*Dự án được thực hiện bởi Nhóm Chic Flow *
