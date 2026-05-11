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

void nhapChucVu(NhanVien *nv) {
    int chon;
    printf("Chuc vu:\n");
    printf(" 1.Quan ly   (He so: 4.20)\n ");
    printf(" 2.Nhan vien (He so: 2.20)\n");
    printf("-->Lua chon: ");
    scanf("%d", &chon);
    while (chon != 1 && chon != 2) {
        printf("\033[31m  Lua chon khong hop le, nhap lai: \033[0m");
        scanf("%d", &chon);
    }
    if (chon == 1) {
        strcpy(nv->chucVu, "Quan ly");
        nv->heSoluong = 4.2;
    } else {
        strcpy(nv->chucVu, "Nhan vien");
        nv->heSoluong = 2.2;
    }
}

void nhapNV(int *n, NhanVien a[]) {
	int i;
    printf("Nhap so luong nhan vien: "); 
	scanf("%d", n); 
    while (*n <= 0) { 
	printf("Nhap lai: "); 
	scanf("%d", n); } 
    for(i = 0; i < *n; i++) {
        printf("\nNhan vien thu %d:\n", i+1); 
		clearBuffer();
        printf("Ho ten: "); 
		fgets(a[i].hoTen, sizeof(a[i].hoTen), stdin); 
		a[i].hoTen[strcspn(a[i].hoTen, "\n")] = '\0';	
        printf("Tuoi: "); 
		scanf("%d", &a[i].tuoi);
        printf("Gioi tinh: "); 
		scanf("%s", a[i].gioiTinh);
        nhapChucVu(&a[i]);
    } 
}

void xuatNV(int n, NhanVien a[]) {
    printHeader("DANH SACH NHAN VIEN");

    if(n == 0) {
        printf("\033[1;31mKhong co nhan vien nao!\033[0m\n");
        pauseConsole();
        return;
    }

    float luongCoBan = 2500;

    printf("====================================================================================================\n");
    printf("%-5s %-25s %-8s %-15s %-15s %-15s %-15s\n", "STT","HO TEN", "TUOI", "GIOI TINH", "CHUC VU", "HE SO LUONG", "LUONG");
    printf("====================================================================================================\n");

    int i;
    for(i = 0; i < n; i++) {
        printf("%-5d %-25s %-8d %-15s %-15s %-15.2f %-15.2f\n", i+1, a[i].hoTen, a[i].tuoi, 
		    a[i].gioiTinh, a[i].chucVu, a[i].heSoluong, a[i].heSoluong * luongCoBan);
    }
    pauseConsole();
}

void themNhanVien(int *n, NhanVien a[]) {
    int k,i; 
	printf("So luong can them: "); 
	scanf("%d", &k);
    for (i=0 ; i<k ;i++) {
        printf("\nNV moi thu %d:\n", i+1); clearBuffer();
        printf("Ho ten: "); fgets(a[*n].hoTen, sizeof(a[*n].hoTen), stdin); a[*n].hoTen[strcspn(a[*n].hoTen, "\n")] = '\0';
        printf("Tuoi: "); scanf("%d", &a[*n].tuoi);
        printf("Gioi tinh: "); scanf("%s", a[*n].gioiTinh);
        nhapChucVu(&a[*n]);
        (*n)++; 
    }
}

void xoaNV(int *n, NhanVien a[]) {
    printHeader("XOA NHAN VIEN");
    if(*n == 0) {
        printf("Danh sach nhan vien trong!\n");
        pauseConsole();
        return;
    }
    char name[50];
    printf("Nhap ho ten nhan vien can xoa: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    int pos = -1;
    int i;
    for(i=0; i<*n; i++) {
        if(strcmp(a[i].hoTen, name) == 0) {
            pos = i;
            break;
        }
    }
    if(pos == -1) {
        printf("Khong tim thay nhan vien!\n");
        pauseConsole();
        return;
    }
    char confirm;
    printf("Ban co chac chan muon xoa nhan vien '%s'? (y/n): ", a[pos].hoTen);
    scanf("%c", &confirm);
    clearBuffer();
    if(confirm != 'y' && confirm != 'Y') {
        printf("Da huy thao tac xoa!\n");
        pauseConsole();
        return;
    }

    for(i=pos; i<*n-1; i++) {
        a[i] = a[i+1];
    }
    (*n)--;
    printf("Xoa nhan vien thanh cong!\n");
    pauseConsole();
}

void tinhLuong(int n, NhanVien a[]) {
    printf("\n-----BANG LUONG NHAN VIEN-----\n");
    float luongCoBan = 2500;
    int i;
    for (i = 0; i < n; i++) {
        printf("%-25s" " | ", a[i].hoTen);

        if (strcmp(a[i].chucVu, "Quan ly") == 0)
            printf("%-10s", a[i].chucVu);
        else
            printf("%-10s", a[i].chucVu);

        printf(" | " "He so: " "%.2f"
               " | " "Luong: " "%.2f VND\n",
               a[i].heSoluong, a[i].heSoluong * luongCoBan);
    }
}

void saveNVToFile(int n, NhanVien a[]) {
    FILE *f = fopen(FILE_NHANVIEN, "w");
    if(!f) return;
    int i;
    for(i=0; i<n; i++)
        fprintf(f, "%s|%d|%s|%s|%.2f\n", a[i].hoTen, a[i].tuoi, a[i].gioiTinh, a[i].chucVu, a[i].heSoluong);
    fclose(f);
}

void loadNVFromFile(int *n, NhanVien a[]) {
    FILE *f = fopen(FILE_NHANVIEN, "r");
    if(!f) return;
    *n = 0;
    while(fscanf(f,"%[^|]|%d|%[^|]|%[^|]|%f\n", a[*n].hoTen, &a[*n].tuoi, a[*n].gioiTinh, a[*n].chucVu, &a[*n].heSoluong) == 5)
{
    (*n)++;
    }
    fclose(f);
}

void menuNhanVien(int *n, NhanVien a[]) {
    while(1) {
        printHeader("QUAN LY NHAN SU");
        printf("  [1] Nhap danh sach ban dau\n");
        printf("  [2] Xem danh sach NV\n");
        printf("  [3] Them nhan vien\n");
        printf("  [4] Xoa nhan vien\n");
        printf("  [5] Tinh luong nhan vien\n");
        printf("  [0] Quay lai\n");
        printf("\n\033[1;35m[Vui long nhap so 0-5 de chon...]\033[0m\n");
        int choice = getKeyboardChoice(5);
        if(choice == 0) break;
        switch(choice) {
            case 1: nhapNV(n, a); break;
            case 2: xuatNV(*n, a); break;
            case 3: themNhanVien(n, a); break;
            case 4: xoaNV(n, a); break;
            case 5:
                if (*n == 0) printf("Chua co nhan vien nao!\n");
                else tinhLuong(*n, a);
        }
    }
}

int main() {
    int n = 0;
    NhanVien a[100];

    menuNhanVien(&n, a);
    return 0;
}
