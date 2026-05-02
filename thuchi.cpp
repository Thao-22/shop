#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char ten[50];
    char size[5]; // S/M/L/XL
    float gia;
    int soLuongTon;
} SanPham;

typedef struct {
    char ngay[20];    
    char moTa[100]; 
    float soTien;
    int loai;        
} ThuChi;

const char* FILE_THUCHI = "thuchi.csv";

void ghiGiaoDichVaoFile(ThuChi tc) {
    FILE *file = fopen(FILE_THUCHI, "a"); 
    if (file == NULL) {
        printf("Loi: Khong the mo hoac tao file %s!\n", FILE_THUCHI);
        return;
    }
    fprintf(file, "%s,%s,%.2f,%d\n", tc.ngay, tc.moTa, tc.soTien, tc.loai);
    fclose(file);
}

void themGiaoDich() {
    ThuChi tc;
    printf("\n--- NHAP GIAO DICH MOI ---\n");
    
    printf("Nhap ngay (dd/mm/yyyy): ");
    scanf("%19s", tc.ngay);
    while(getchar() != '\n');
    printf("Nhap mo ta giao dich: ");
    fgets(tc.moTa, sizeof(tc.moTa), stdin);
    tc.moTa[strcspn(tc.moTa, "\n")] = 0;
    printf("Nhap so tien (VD: 150000): ");
    scanf("%f", &tc.soTien);
    printf("Loai giao dich (Nhap 1 neu la THU, Nhap 0 neu la CHI): ");
    scanf("%d", &tc.loai);
    ghiGiaoDichVaoFile(tc);
    printf("-> Da luu giao dich thanh cong vao file CSV!\n");
}

void lapBaoCaoThuChi() {
    FILE *file = fopen(FILE_THUCHI, "r"); 
    if (file == NULL) {
        printf("Chua co du lieu thu chi hoac file khong ton tai!\n");
        return;
    }

    char buffer[256];
    float tongDoanhThu = 0;
    float tongChiPhi = 0;

    printf("\n======================= BAO CAO THU CHI =======================\n");
    printf("%-12s | %-25s | %-15s | %-10s\n", "Ngay", "Mo ta", "So tien", "Loai");
    printf("---------------------------------------------------------------\n");

    while (fgets(buffer, sizeof(buffer), file)) {
        ThuChi tc;
        char *token = strtok(buffer, ",");
        if(token == NULL) continue;
        strcpy(tc.ngay, token);
        token = strtok(NULL, ",");
        if(token == NULL) continue;
        strcpy(tc.moTa, token);
        token = strtok(NULL, ",");
        if(token == NULL) continue;
        tc.soTien = atof(token);
        token = strtok(NULL, ",");
        if(token == NULL) continue;
        tc.loai = atoi(token); 

        printf("%-12s | %-25s | %-15.2f | %-10s\n", 
               tc.ngay, tc.moTa, tc.soTien, tc.loai == 1 ? "Thu" : "Chi");
        if (tc.loai == 1) {
            tongDoanhThu += tc.soTien;
        } else {
            tongChiPhi += tc.soTien;
        }
    }
    fclose(file);
    float loiNhuan = tongDoanhThu - tongChiPhi;
    printf("---------------------------------------------------------------\n");
    printf("TONG DOANH THU (Cac khoan THU) : +%.2f VND\n", tongDoanhThu);
    printf("TONG CHI PHI   (Cac khoan CHI) : -%.2f VND\n", tongChiPhi);
    printf("===============================================================\n");
    if (loiNhuan >= 0) {
        printf(">> LOI NHUAN TONG KET          :  %.2f VND (LAI)\n", loiNhuan);
    } else {
        printf(">> LOI NHUAN TONG KET          : %.2f VND (LO)\n", loiNhuan);
    }
    printf("===============================================================\n");
}

void menuQuanLySanPham() {
    printf("\n[KHU VUC CUA MODULE SAN PHAM]\n");
    printf("-> Nguoi lam module san pham se ghep cac ham (nhap hang, check ton kho) vao day.\n");
    printf("-> Khi ban hang hoac nhap hang thanh cong, co the goi ham ghiGiaoDichVaoFile() de tu dong dong bo qua module Thu Chi!\n");
}

int main() {
    int luaChon;
    do {
        printf("\n*********************************************\n");
        printf("* HE THONG QUAN LY SHOP QUAN AO         *\n");
        printf("*********************************************\n");
        printf("* 1. Quan ly San Pham (Module ghep code)    *\n");
        printf("* 2. Them ghi chep Thu / Chi thu cong       *\n");
        printf("* 3. Xem Bao cao Doanh Thu & Loi Nhuan      *\n");
        printf("* 0. Thoat chuong trinh                     *\n");
        printf("*********************************************\n");
        printf("Nhap lua chon cua ban (0-3): ");
        scanf("%d", &luaChon);

        switch (luaChon) {
            case 1:
                menuQuanLySanPham();
                break;
            case 2:
                themGiaoDich();
                break;
            case 3:
                lapBaoCaoThuChi();
                break;
            case 0:
                printf("\nCam on da su dung phan mem. Tam biet!\n");
                break;
            default:
                printf("\nLua chon khong hop le. Vui long chon lai!\n");
        }
    } while (luaChon != 0);

    return 0;
}