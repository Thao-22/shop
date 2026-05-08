#include<stdio.h>
#include<string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

typedef struct {
    char hoTen[50];
    int tuoi;
    char gioiTinh[20];
    char chucVu[30];
    float heSoLuong;
} NhanVien;

float luongCoBan = 0;

void nhapChucVu(NhanVien *nv) {
    int chon;
    printf(CYAN "Chuc vu:\n" RESET);
    printf(YELLOW "  1. Quan ly   (He so: 4.20)\n" RESET);
    printf(YELLOW "  2. Nhan vien (He so: 2.20)\n" RESET);
    printf(CYAN "  Lua chon: " RESET);
    scanf("%d", &chon);
    while (chon != 1 && chon != 2) {
        printf(RED "  Lua chon khong hop le, nhap lai: " RESET);
        scanf("%d", &chon);
    }
    if (chon == 1) {
        strcpy(nv->chucVu, "Quan ly");
        nv->heSoLuong = 4.2;
    } else {
        strcpy(nv->chucVu, "Nhan vien");
        nv->heSoLuong = 2.2;
    }
}

void nhap(int *n, NhanVien a[]) {
    printf(CYAN "Nhap so luong nhan vien cua shop: " RESET);
    scanf("%d", n);
    while (*n <= 0) {
        printf(RED "Vui long nhap lai so luong nhan vien: " RESET);
        scanf("%d", n);
    }
    for (int i = 0; i < *n; i++) {
        printf(BOLD MAGENTA "\nNhan vien thu %d:\n" RESET, i+1);
        while (getchar() != '\n');
        printf(CYAN "Ho ten: " RESET);
        fgets(a[i].hoTen, sizeof(a[i].hoTen), stdin);
        a[i].hoTen[strcspn(a[i].hoTen, "\n")] = '\0';
        printf(CYAN "Tuoi: " RESET);
        scanf("%d", &a[i].tuoi);
        printf(CYAN "Gioi tinh: " RESET);
        scanf("%s", a[i].gioiTinh);
        nhapChucVu(&a[i]);
    }
}

void xuat(int n, NhanVien a[]) {
    printf(BOLD BLUE "\n-----DANH SACH NHAN VIEN-----\n" RESET);
    for (int i = 0; i < n; i++) {
        printf(BOLD MAGENTA "\nNhan vien thu %d:\n" RESET, i+1);
        printf(CYAN "  Ho ten      : " RESET "%s\n",  a[i].hoTen);
        printf(CYAN "  Tuoi        : " RESET "%d\n",  a[i].tuoi);
        printf(CYAN "  Gioi tinh   : " RESET "%s\n",  a[i].gioiTinh);
        
        if (strcmp(a[i].chucVu, "Quan ly") == 0)
            printf(CYAN "  Chuc vu     : " RESET YELLOW "%s\n" RESET, a[i].chucVu);
        else
            printf(CYAN "  Chuc vu     : " RESET WHITE "%s\n" RESET, a[i].chucVu);

        printf(CYAN "  He so luong : " RESET "%.2f\n", a[i].heSoLuong);
        printf(CYAN "  Luong thang : " RESET GREEN "%.2f VND\n" RESET, a[i].heSoLuong * luongCoBan);
    }
}

void themNhanVien(int *n, NhanVien a[]) {
    int k;
    printf(CYAN "Nhap so luong nhan vien muon them: " RESET);
    scanf("%d", &k);
    for (int i = 0; i < k; i++) {
        printf(BOLD MAGENTA "\nNhap nhan vien moi thu %d:\n" RESET, i+1);
        while (getchar() != '\n');
        printf(CYAN "Ho ten: " RESET);
        fgets(a[*n].hoTen, sizeof(a[*n].hoTen), stdin);
        a[*n].hoTen[strcspn(a[*n].hoTen, "\n")] = '\0';
        printf(CYAN "Tuoi: " RESET);
        scanf("%d", &a[*n].tuoi);
        printf(CYAN "Gioi tinh: " RESET);
        scanf("%s", a[*n].gioiTinh);
        nhapChucVu(&a[*n]);
        (*n)++;
    }
}

void xoaNhanVien(int *n, NhanVien a[]) {
    int viTri;
    printf(CYAN "Nhap vi tri nhan vien can xoa (1 -> %d): " RESET, *n);
    scanf("%d", &viTri);
    if (viTri < 1 || viTri > *n) {
        printf(RED "Vi tri khong hop le!\n" RESET);
        return;
    }
    for (int i = viTri - 1; i < *n - 1; i++) {
        a[i] = a[i + 1];
    }
    (*n)--;
    printf(GREEN "Da xoa nhan vien thanh cong!\n" RESET);
}

void tinhLuong(int n, NhanVien a[]) {
    printf(BOLD BLUE "\n-----BANG LUONG NHAN VIEN-----\n" RESET);
    for (int i = 0; i < n; i++) {
        printf(MAGENTA "%-25s" RESET " | ", a[i].hoTen);

        if (strcmp(a[i].chucVu, "Quan ly") == 0)
            printf(YELLOW "%-10s" RESET, a[i].chucVu);
        else
            printf(WHITE "%-10s" RESET, a[i].chucVu);

        printf(" | " CYAN "He so: " RESET "%.2f"
               " | " CYAN "Luong: " RESET GREEN "%.2f VND\n" RESET,
               a[i].heSoLuong, a[i].heSoLuong * luongCoBan);
    }
}

void menuNhanVien(int *n, NhanVien a[]) {
    int chon;
    printf(BOLD BLUE "========== QUAN LY NHAN VIEN SHOP ==========\n" RESET);
    do {
        printf(CYAN "Nhap luong co ban (VND): " RESET);
        scanf("%f", &luongCoBan);
        if (luongCoBan <= 0)
            printf(RED "Luong co ban phai lon hon 0, vui long nhap lai!\n" RESET);
    } while (luongCoBan <= 0);

    do {
        printf(BOLD BLUE "\n========== QUAN LY NHAN VIEN SHOP ==========\n" RESET);
        printf(YELLOW "1. " RESET "Nhap danh sach nhan vien\n");
        printf(YELLOW "2. " RESET "Xuat danh sach nhan vien\n");
        printf(YELLOW "3. " RESET "Them nhan vien\n");
        printf(YELLOW "4. " RESET "Xoa nhan vien\n");
        printf(YELLOW "5. " RESET "Tinh luong nhan vien\n");
        printf(RED    "0. " RESET "Thoat\n");
        printf(BOLD BLUE "=============================================\n" RESET);
        printf(CYAN "Nhap lua chon: " RESET);
        scanf("%d", &chon);
        switch (chon) {
            case 1: nhap(n, a); break;
            case 2:
                if (*n == 0) printf(RED "Chua co nhan vien nao!\n" RESET);
                else xuat(*n, a);
                break;
            case 3: themNhanVien(n, a); break;
            case 4: xoaNhanVien(n, a); break;
            case 5:
                if (*n == 0) printf(RED "Chua co nhan vien nao!\n" RESET);
                else tinhLuong(*n, a);
                break;
            case 0: printf(RED "Thoat chuong trinh!\n" RESET); break;
            default: printf(RED "Lua chon khong hop le!\n" RESET);
        }
    } while (chon != 0);
}

int main() {
    int n = 0;
    NhanVien a[100];

    menuNhanVien(&n, a);
    return 0;
}
