#ifndef SHOP_H
#define SHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define MAX_STR 100
#define FILE_NAME "chic_flow_data.txt"
#define FILE_THUCHI "thuchi.csv"
#define MAX_SP 100
#define MAX_NV 100

// 1. Module Chic Flow (San pham thiet ke)
typedef struct Product {
    int id;
    char name[MAX_STR];
    float price;
    struct Product *next;
} Product;

// 2. Module Thu Chi
typedef struct {
    char ngay[20];
    char moTa[100];
    float soTien;
    int loai; // 1: Thu, 0: Chi
} ThuChi;

// 3. Module Nhan Vien
typedef struct {
    char hoTen[50];
    int tuoi;
    char gioiTinh[20];
    float heSoluong; 
} NhanVien;

// 4. Module Kho Hang San Pham
typedef struct {
   char Tensp[100];
   char Size[10];
   int soLuong;
   float Gia;
   int daban;
   float giamgia;
} Sanpham;

// Tien ich UI & He thong
void clearBuffer();
void setConsoleWindow();
int getKeyboardChoice(int numOptions); // Ð? ð?i tên và tham s?
void printHeader(const char* title);
void pauseConsole();

// Module Thu Chi
void ghiGiaoDich(ThuChi tc);
void themGiaoDich();
void lapBaoCaoThuChi();

// Module Chic Flow (Thiet ke)
void addProduct(Product **head);
void sellProductChicFlow(Product *head);
void saveToFile(Product *head);
void loadFromFile(Product **head);
void freeList(Product *head);
void menuChicFlow(Product **head);

// Module Nhan Vien
void nhapNV(int *n, NhanVien a[]);
void xuatNV(int n, NhanVien a[]);
void themNhanVien(int *n, NhanVien a[]);
void xoaNhanVien(int *n, NhanVien a[]);
float tongHeSoLuong(int n, NhanVien a[]);
float trungBinhHeSoLuong(int n, NhanVien a[]);
void menuNhanVien(int *n, NhanVien a[]);

// Module Kho Hang
int nhapSoLuong();
void nhapHang(int *n, Sanpham ds[]);
void showKho(int n, Sanpham ds[]);
void discountKho(int n, Sanpham ds[]);
void sellKho(int n, Sanpham ds[]);
void kiemTraKho(int n, Sanpham ds[]);
void favProducts(int n, Sanpham ds[]);
void menuKhoHang(int *n, Sanpham ds[]);

#endif
