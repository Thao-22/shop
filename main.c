#include "shop.h"

int main() {
    setConsoleWindow();
    Product *chicFlowData = NULL;
    loadFromFile(&chicFlowData);
    
    Sanpham khoHang[MAX_SP];
    int soLuongSPKho = 0;
    
    NhanVien danhSachNV[MAX_NV];
    int soLuongNV = 0;

    while (1) {
        system("cls");
        printf("\033[1;36m");
        printf("=========================================================\n");
        printf("            HE THONG QUAN LY SHOP QUAN AO 4 IN 1         \n");
        printf("=========================================================\n");
        printf("\033[0m");
        printf("  [1] Quan ly Thiet Ke & San Pham Moi (Chic Flow)\n"); 
        printf("  [2] Quan ly Kho Hang & Ban Hang (Inventory)\n");
        printf("  [3] Quan ly Nhan Vien (HR)\n");
        printf("  [4] Quan ly Tai Chinh & Thu Chi (Finance)\n");
        printf("  [0] Thoat chuong trinh\n");
        printf("=========================================================\n");
        printf("\033[1;35m>>> HAY CLICK CHUOT VAO DONG BAN MUON CHON <<<\033[0m\n");

        int choice = getMouseMenuChoice(3, 4);

        if (choice == 0) {
            printf("\n\033[1;32mCam on da su dung phan mem. Tam biet!\033[0m\n");
            break;
        }

        switch (choice) {
            case 1:
                menuChicFlow(&chicFlowData);
                break;
            case 2:
                menuKhoHang(&soLuongSPKho, khoHang);
                break;
            case 3:
                menuNhanVien(&soLuongNV, danhSachNV);
                break;
            case 4:
                while(1) {
                    printHeader("QUAN LY TAI CHINH - THU CHI");
                    printf("  [1] Them ghi chep Thu / Chi thu cong\n");
                    printf("  [2] Xem Bao cao Doanh Thu & Loi Nhuan\n");
                    printf("  [0] Quay lai\n");
                    printf("\n\033[1;35m[Dung chuot click hoac an so de chon...]\033[0m");
                    int tcChoice = getMouseMenuChoice(3, 2);
                    if(tcChoice == 0) break;
                    if(tcChoice == 1) themGiaoDich();
                    if(tcChoice == 2) lapBaoCaoThuChi();
                }
                break;
        }
    }
    freeList(chicFlowData);
    return 0;
}