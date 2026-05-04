#include<stdio.h>
#include<string.h> 
typedef struct {
	char hoTen[50];
	int tuoi;
	char gioiTinh[20];
	float heSoluong; 
}NhanVien;

void nhap ( int *n, NhanVien a[]){
	printf("Nhap so luong nhan vien cua shop : ");
	scanf("%d", n); 
	while ( *n <= 0){
		printf("Vui long nhap lai so luong nhan vien : ");
		scanf("%d", n); 
	} 	
	for( int i = 0; i<*n ; i++){
		printf("\nNhan vien thu %d : \n", i+1);
		while (getchar() != '\n');
		printf("Ho ten nhan vien : ");
	    fgets(a[i].hoTen, sizeof(a[i].hoTen), stdin);
		a[i].hoTen[strcspn(a[i].hoTen, "\n")] = '\0';	
		printf("Tuoi : ");
	    scanf("%d", &a[i].tuoi);
	    printf("Gioi tinh : ");
	    scanf("%s", a[i].gioiTinh);
    	printf("He so luong : ");
    	scanf("%f",&a[i].heSoluong); 
	} 
}

void xuat(int n, NhanVien a[]){
	printf("\n-----DANH SACH NHAN VIEN-----\n"); 
	for( int i=0; i<n; i++){
		printf("\nNhan vien thu %d : \n", i+1);
		printf("Ho ten nhan vien : %s\n", a[i].hoTen);
		printf("Tuoi : %d\n", a[i].tuoi);
		printf("Gioi tinh : %s\n", a[i].gioiTinh);
		printf("He so luong : %.2f\n", a[i].heSoluong); 
	} 
} 

void themNhanVien(int *n, NhanVien a[]){
    int k;
    printf("Nhap so luong nhan vien muon them: ");
    scanf("%d", &k);
    for (int i=0 ; i<k ;i++) {
        printf("\nNhap nhan vien moi thu %d:\n", i+1);
        while (getchar() != '\n');
        printf("Ho ten nhan vien: ");
        fgets(a[*n].hoTen, sizeof(a[*n].hoTen), stdin);
        a[*n].hoTen[strcspn(a[*n].hoTen, "\n")] = '\0';
        printf("Tuoi: ");
        scanf("%d", &a[*n].tuoi);
        printf("Gioi tinh: ");
        scanf("%s", a[*n].gioiTinh);
        printf("He so luong: ");
        scanf("%f", &a[*n].heSoluong);
        (*n)++; 
    }
}
 void xoaNhanVien(int *n, NhanVien a[]){
    int viTri;
    printf("Nhap vi tri nhan vien can xoa (1 -> %d): ", *n);
    scanf("%d", &viTri);
    if (viTri < 1 || viTri > *n) {
        printf("Vi tri khong hop le!\n");
        return;
    }
    for (int i = viTri-1 ; i<*n - 1 ; i++) {
        a[i] = a[i + 1];
    }
    (*n)--; 
    printf("Da xoa nhan vien!\n");
}  

float tongHeSoLuong(int n, NhanVien a[]){
    float tong = 0;
    for (int i=0; i<n; i++) {
        tong += a[i].heSoluong;
    }
    return tong;
}

float trungBinhHeSoLuong(int n, NhanVien a[]) {
    if (n == 0) return 0;
    return tongHeSoLuong(n, a)/n ;
}

void menuNhanVien(int *n, NhanVien a[]) {
    int chon;
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Nhap danh sach nhan vien\n");
        printf("2. Xuat danh sach nhan vien\n");
        printf("3. Them nhan vien\n");
        printf("4. Xoa nhan vien\n");
        printf("5. Tong he so luong\n");
        printf("6. Trung binh he so luong\n");
        printf("0. Thoat\n");
        printf("==========================\n");
        printf("Nhap lua chon: ");
        scanf("%d", &chon);

        switch(chon) {
            case 1:
                nhap(n, a);
                break;
            case 2:
                xuat(*n, a);
                break;
            case 3:
                themNhanVien(n, a);
                break;
            case 4:
                xoaNhanVien(n, a);
                break;
            case 5:
                printf("Tong he so luong: %.2f\n", tongHeSoLuong(*n, a));
                break;
            case 6:
                printf("Trung binh he so luong: %.2f\n", trungBinhHeSoLuong(*n, a));
                break;
            case 0:
                printf("Thoat chuong trinh!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }

    } while(chon != 0);
}
	
int main() {
    int n = 0;
    NhanVien a[100];

    menuNhanVien(&n, a);

    return 0;
}
