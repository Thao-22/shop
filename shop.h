#ifndef SHOP_H
#define SHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define MAX_STR 100
#define FILE_NAME "chic_flow_data.txt"
#define FILE_THUCHI "thuchi.csv"
#define FILE_NHANVIEN "nhanvien.txt"
#define FILE_KHOHANG "khohang.txt"
#define MAX_SP 100
#define MAX_NV 100
typedef struct Product {
    int id;
    char name[MAX_STR];
    float price;
    int soLuong;  
    float giamgia; 
    int daban;
    struct Product *next;
} Product;

typedef struct {
    char ngay[20];
    char moTa[100];
    float soTien;
    int loai;
} ThuChi;

typedef struct {
    char hoTen[50];
    int tuoi;
    char gioiTinh[20];
    char chucVu[30];
    float heSoluong; 
} NhanVien;

typedef struct {
	int id; 
 	char Tensp[100];
   	char Size[10];
   	int soLuong;
   	float Gia;
   	int daban;
   	float giamgia;
} Sanpham;

// --- Prototypes ---
void clearBuffer();
void setConsoleWindow();
int getKeyboardChoice(int numOptions); 
void printHeader(const char* title);
void pauseConsole();

// Module Thu Chi
void ghiGiaoDich(ThuChi tc);
void themGiaoDich();
void lapBaoCaoThuChi();
void menuTaiChinh();

// Module Chic Flow
void importFromWarehouse(Product **head, int *nKho, Sanpham dsKho[]);
void displayStoreProducts(Product *head);
void sellAtStore(Product *head); 
void updateProduct(Product *head);   
void deleteProduct(Product **head);  
void sortProducts(Product **head);   
void saveToFile(Product *head);
void loadFromFile(Product **head);
void exportReport(Product *head);
void freeList(Product *head);
void favProductsChicFlow(Product *head);
void menuChicFlow(Product **head, int *nKho, Sanpham dsKho[]);

// Module Nhan Vien
void saveNVToFile(int n, NhanVien a[]);
void loadNVFromFile(int *n, NhanVien a[]);
void nhapNV(int *n, NhanVien a[]);
void xuatNV(int n, NhanVien a[]);
void themNhanVien(int *n, NhanVien a[]);
void xoaNV(int *n, NhanVien a[]);
void tinhLuong(int n, NhanVien a[]);
void menuNhanVien(int *n, NhanVien a[]);

// Module Kho Hang
void saveKhoToFile(int n, Sanpham ds[]);
void loadKhoFromFile(int *n, Sanpham ds[]);
void nhapHang(int *n, Sanpham ds[]);
void showKho(int n, Sanpham ds[]);
void menuKhoHang(int *n, Sanpham ds[]);

#endif
