#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_THUCHI "thuchi.txt"

// Định nghĩa cấu trúc dữ liệu ThuChi
typedef struct {
    char ngay[20];
    char moTa[100];
    float soTien;
    int loai; // 1: Thu, 0: Chi
} ThuChi;

// --- CÁC HÀM HỖ TRỢ GIAO DIỆN & TIỆN ÍCH ---

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseConsole() {
    printf("\nNhan Enter de tiep tuc...");
    clearBuffer();
    getchar();
}

void printHeader(const char *title) {
    system("cls"); // Dung "clear" neu ban xai Linux/MacOS
    printf("========================================\n");
    printf("   %s\n", title);
    printf("========================================\n");
}

int getKeyboardChoice(int max) {
    int choice;
    printf("Lua chon cua ban (0-%d): ", max);
    while (scanf("%d", &choice) != 1 || choice < 0 || choice > max) {
        printf("Nhap sai! Vui long nhap lai: ");
        clearBuffer();
    }
    return choice;
}

// --- CÁC HÀM CHỨC NĂNG CHÍNH ---

void ghiGiaoDich(ThuChi tc) {
    FILE *file = fopen(FILE_THUCHI, "a"); 
    if (file == NULL) return;
    fprintf(file, "%s,%s,%.2f,%d\n", tc.ngay, tc.moTa, tc.soTien, tc.loai);
    fclose(file);
}

void themGiaoDich() {
    printHeader("NHAP GIAO DICH MOI");
    ThuChi tc;
    printf("Ngay (dd/mm/yyyy): "); scanf("%19s", tc.ngay); clearBuffer();
    
    printf("Mo ta: "); fgets(tc.moTa, sizeof(tc.moTa), stdin);
    tc.moTa[strcspn(tc.moTa, "\n")] = 0;
    
    // Xu ly dau phay de khong lam hong file CSV
    int i;
    for(i = 0; i < (int)strlen(tc.moTa); i++) {
        if(tc.moTa[i] == ',') tc.moTa[i] = '-';
    }
    
    printf("So tien: "); scanf("%f", &tc.soTien);
    printf("Loai (1: THU, 0: CHI): "); scanf("%d", &tc.loai);
    
    ghiGiaoDich(tc);
    printf("-> Luu thanh cong!\n");
    pauseConsole();
}

void lapBaoCaoThuChi() {
    printHeader("BAO CAO THU CHI DOANH NGHIEP");
    FILE *file = fopen(FILE_THUCHI, "r");
    if (file == NULL) {
        printf("(!) Chua co du lieu thu chi.\n"); 
        pauseConsole(); 
        return;
    }
    
    char buffer[256];
    float tongThu = 0, tongChi = 0;
    printf("%-12s | %-25s | %-15s | %-10s\n", "Ngay", "Mo ta", "So tien", "Loai");
    printf("---------------------------------------------------------------\n");
    
    while (fgets(buffer, sizeof(buffer), file)) {
        ThuChi tc;
        char *token = strtok(buffer, ","); 
        if(!token) continue; 
        strcpy(tc.ngay, token);
        
        token = strtok(NULL, ","); 
        if(!token) continue; 
        strcpy(tc.moTa, token);
        
        token = strtok(NULL, ","); 
        if(!token) continue; 
        tc.soTien = (float)atof(token);
        
        token = strtok(NULL, ","); 
        if(!token) continue; 
        tc.loai = atoi(token);
        
        printf("%-12s | %-25s | %-15.2f | %-10s\n", 
               tc.ngay, tc.moTa, tc.soTien, 
               tc.loai == 1 ? "\033[1;32mThu\033[0m" : "\033[1;31mChi\033[0m");
               
        if (tc.loai == 1) 
            tongThu += tc.soTien; 
        else 
            tongChi += tc.soTien;
    }
    fclose(file);
    
    float loiNhuan = tongThu - tongChi;
    printf("---------------------------------------------------------------\n");
    printf("TONG THU : \033[1;32m+%.2f VND\033[0m\n", tongThu);
    printf("TONG CHI : \033[1;31m-%.2f VND\033[0m\n", tongChi);
    printf("LOI NHUAN: %s%.2f VND\033[0m\n", loiNhuan >= 0 ? "\033[1;32m" : "\033[1;31m", loiNhuan);
    pauseConsole();
}

void menuTaiChinh() {
    while (1) {
        printHeader("QUAN LY TAI CHINH & THU CHI");
        printf("  [1] Nhap giao dich moi\n");
        printf("  [2] Lap bao cao thu chi\n");
        printf("  [0] Quay lai\n");
        printf("\n\t");
        
        int tcChoice = getKeyboardChoice(2);
        if(tcChoice == 0) break;
        if(tcChoice == 1) themGiaoDich();
        if(tcChoice == 2) lapBaoCaoThuChi();
    }
}

int main() {
    // Goi menu chinh
    menuTaiChinh();
    return 0;
}