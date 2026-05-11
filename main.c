#include "shop.h"

int main() {
    setConsoleWindow();
    
    Product *chicFlowData = NULL;
    loadFromFile(&chicFlowData);
    
    Sanpham khoHang[MAX_SP];
    int soLuongSPKho = 0;
    loadKhoFromFile(&soLuongSPKho, khoHang); //Update kho hang 
    
    NhanVien danhSachNV[MAX_NV];
    int soLuongNV = 0;
    loadNVFromFile(&soLuongNV, danhSachNV); //Update nhan vien 

    while (1) {
        system("cls");
        
        printf("\n\n");
        printf("\t\033[1;36m=========================================================\033[0m\n");
        printf("\t\033[1;36m||\033[0m\033[1;33m         HE THONG QUAN LY SHOP QUAN AO 4 IN 1        \033[0m\033[1;36m||\033[0m\n");
        printf("\t\033[1;36m=========================================================\033[0m\n");
        printf("\t\033[1;36m||\033[0m                                                     \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m   \033[1;32m[1]\033[0m \033[1;37mQuan ly Thiet Ke & San Pham Moi (Chic Flow)\033[0m   \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m   \033[1;32m[2]\033[0m \033[1;37mQuan ly Kho Hang & Ban Hang (Inventory)\033[0m       \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m   \033[1;32m[3]\033[0m \033[1;37mQuan ly Nhan Vien (HR)\033[0m                        \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m   \033[1;32m[4]\033[0m \033[1;37mQuan ly Tai Chinh & Thu Chi (Finance)\033[0m         \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m                                                    \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m||\033[0m   \033[1;31m[0]\033[0m \033[1;31mThoat & Luu du lieu\033[0m                           \033[1;36m||\033[0m\n");
        printf("\t\033[1;36m=========================================================\033[0m\n");
        printf("\n\t"); 

        int choice = getKeyboardChoice(4);

        if (choice == 0) {
            saveToFile(chicFlowData); 
            saveNVToFile(soLuongNV, danhSachNV);
            saveKhoToFile(soLuongSPKho, khoHang);
            printf("\n\t\033[1;32m=> Du lieu da duoc luu an toan. Tam biet!\033[0m\n\n");
            break;
        }

        switch (choice) {
            case 1: menuChicFlow(&chicFlowData, &soLuongSPKho, khoHang); break;
            case 2: menuKhoHang(&soLuongSPKho, khoHang); break;
            case 3: menuNhanVien(&soLuongNV, danhSachNV); break;
            case 4: menuTaiChinh(); break;
                }
        }
    freeList(chicFlowData);
    return 0;
}