#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 100
#define FILE_KHOHANG "khohang.txt"

typedef struct {
    int id;
    char Tensp[100];
    char Size[10];
    int soLuong;
    int daban;
    int hangLoi;
    int hangTra;
    float Gia;
    float giamgia;
} Sanpham;

Sanpham ds[MAX];
int n=0;

int nhapSoNguyenDuong(char message[]) {
    int x;

    while (1) {
        printf("%s", message);

        if (scanf("%d", &x) == 1 && x > 0) {
            clearBuffer();
            return x;
        }

        printf("\033[1;31mNhap sai! Vui long nhap so nguyen duong.\033[0m\n");
        clearBuffer();
    }
}

float nhapGia() {
    float gia;

    while (1) {
        printf("- Gia: ");

        if (scanf("%f", &gia) == 1 && gia > 0) {
            clearBuffer();
            return gia;
        }

        printf("\033[1;31mGia khong hop le!\033[0m\n");
        clearBuffer();
    }
}

void nhapSize(char size[]) {
    while(1) {
        printf("Nhap size (S/M/L/XL/XXL): ");
        fgets(size, 10, stdin);
        size[strcspn(size, "\n")] = 0;

        // chuyen thanh in hoa
        int i;
        for(i = 0; size[i] != '\0'; i++) {
            if(size[i] >= 'a' && size[i] <= 'z') {
                size[i] -= 32;
            }
        }
        if(strcmp(size,"S")==0 || strcmp(size,"M")==0 || strcmp(size,"L")==0 || strcmp(size,"XL")==0 ||  strcmp(size,"XXL")==0) {
            return;
        }
        printf("\033[1;31mSize khong hop le!\033[0m\n");
    }
}

void chuanHoaTen(char ten[]) {
    int i;
    ten[strcspn(ten, "\n")] = 0;

    // chuyen ky tu dau thanh in hoa
    if(ten[0] >= 'a' && ten[0] <= 'z') {
        ten[0] -= 32;
    }
    // cac ky tu sau thanh thuong
    for(i = 1; ten[i] != '\0'; i++) {
        if(ten[i] >= 'A' && ten[i] <= 'Z') {
            ten[i] += 32;
        }
    }
} 

int giaTriSize(char size[]) {
	
    if(strcmp(size,"S")==0) return 1;
    if(strcmp(size,"M")==0) return 2;
    if(strcmp(size,"L")==0) return 3;
    if(strcmp(size,"XL")==0) return 4;
    if(strcmp(size,"XXL")==0) return 5;
    return 999;
}

void themSanPham(int *n, Sanpham ds[]) {

    int soSP;
    int i, j;

    printHeader("THEM SAN PHAM");

    soSP = nhapSoNguyenDuong("Nhap so san pham can them: ");

    for(i = 0; i < soSP; i++) {

        Sanpham sp;
        int trung = -1;

        printf("\n========== SAN PHAM %d ==========\n", i + 1);

        // ===== NHAP TEN =====
        printf("Ten san pham: ");
        fgets(sp.Tensp, sizeof(sp.Tensp), stdin);
        sp.Tensp[strcspn(sp.Tensp, "\n")] = 0;

        chuanHoaTen(sp.Tensp);

        // ===== NHAP SIZE =====
        nhapSize(sp.Size);

        // ===== KIEM TRA TRUNG TEN + SIZE =====
        for(j = 0; j < *n; j++) {

            char ten1[100];
            char ten2[100];

            char size1[10];
            char size2[10];

            strcpy(ten1, ds[j].Tensp);
            strcpy(ten2, sp.Tensp);

            strcpy(size1, ds[j].Size);
            strcpy(size2, sp.Size);

            // ===== DONG BO TEN =====
            strlwr(ten1);
            strlwr(ten2);

            // ===== DONG BO SIZE =====
            strupr(size1);
            strupr(size2);

            if(strcmp(ten1, ten2) == 0 && strcmp(size1, size2) == 0) {
                trung = j;
                break;
            }
        }

        // ==================================================
        // ===== SAN PHAM DA TON TAI =====
        // ==================================================
        if(trung != -1) {

            int them;
            printf("\n\033[1;33mSan pham da ton tai!\033[0m\n");
            them = nhapSoNguyenDuong("Nhap so luong them: ");
            ds[trung].soLuong += them;

            printf("\033[1;32mDa cong don so luong!\033[0m\n");
            continue;
        }

        // ==================================================
        // ===== SAN PHAM MOI =====
        // ==================================================

        printf("\nSan pham moi!\n");

        sp.id = nhapSoNguyenDuong("Nhap ID: ");
        sp.soLuong = nhapSoNguyenDuong("Nhap so luong: ");
        sp.Gia = nhapGia();
        sp.daban = 0;
        sp.hangLoi = 0;
        sp.hangTra = 0;
        sp.giamgia = 0;

        ds[*n] = sp;
        (*n)++;

        printf("\033[1;32mThem san pham thanh cong!\033[0m\n");
    }
    pauseConsole();
}

void loadKhoFromFile(int *n, Sanpham ds[]) {

    FILE *f = fopen("Khohang.txt", "r");

    if(f == NULL) {
        return;
    }
    *n = 0;
    Sanpham sp;

    while(fscanf(f,"%d|%[^|]|%[^|]|%d|%d|%d|%d|%f|%f\n",
          &sp.id,
          sp.Tensp,
          sp.Size,
          &sp.soLuong,
          &sp.daban,
          &sp.hangLoi,
          &sp.hangTra,
          &sp.Gia,
          &sp.giamgia) == 9) {

        // ===== CHUAN HOA =====
        chuanHoaTen(sp.Tensp);
        int k;
        for(k = 0; sp.Size[k] != '\0'; k++) {
            if(sp.Size[k] >= 'a' && sp.Size[k] <= 'z') {
                sp.Size[k] -= 32;
            }
        }
        int i;
        int trung = -1;

        // ===== KIEM TRA TRUNG =====
        for(i = 0; i < *n; i++) {
            if(strcmp(ds[i].Tensp, sp.Tensp) == 0 && strcmp(ds[i].Size, sp.Size) == 0) {

                trung = i;
                break;
            }
        }
        // ===== NEU TRUNG =====
        if(trung != -1) {

            ds[trung].soLuong += sp.soLuong;
            ds[trung].daban += sp.daban;
            ds[trung].hangLoi += sp.hangLoi;
            ds[trung].hangTra += sp.hangTra;

            // ===== KIEM TRA GIA =====
            if(ds[trung].Gia != sp.Gia) {

                printf("\n[CANH BAO] %s Size %s co gia khac nhau!\n", sp.Tensp, sp.Size);
                printf("Gia cu : %.0f\n", ds[trung].Gia);
                printf("Gia moi: %.0f\n", sp.Gia);
                printf("Dang giu gia cu.\n");
            }
        }

        // ===== NEU CHUA TON TAI =====
        else {
            ds[*n] = sp;
            (*n)++;
        }
    }

    fclose(f);
    printf("\n\033[1;32mDa tai du lieu tu file!\033[0m\n");
}
void showKho(int n, Sanpham ds[]) {

    int i, j;
    // ===== SAP XEP =====
    for(i = 0; i < n - 1; i++) {
        for(j = i + 1; j < n; j++) {
            int doi = 0;

            if(strcmp(ds[i].Tensp, ds[j].Tensp) > 0) {
                doi = 1;
            }else if(strcmp(ds[i].Tensp, ds[j].Tensp) == 0) {
                if(giaTriSize(ds[i].Size) > giaTriSize(ds[j].Size)) {
                    doi = 1;
                }
            }
            if(doi) {
                Sanpham temp = ds[i];
                ds[i] = ds[j];
                ds[j] = temp;
            }
        }
    }
    printHeader("DANH SACH KHO HANG");
    printf("%-5s %-20s %-8s %-10s %-10s %-10s\n", "ID","TEN","SIZE","SO LUONG","GIA","DA BAN");

    for(i = 0; i < n; i++) {
        printf("%-5d %-20s %-8s %-10d %-10.0f %-10d\n", ds[i].id, ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia, ds[i].daban);
    }

    pauseConsole();
}

void banHangKho(int n, Sanpham ds[]) {

    printHeader("BAN HANG TAI KHO");

    if(n == 0) {
        printf("\033[1;31mKho hang dang trong!\033[0m\n");
        pauseConsole();
        return;
    }

    int tiepTuc = 1;
    float tongTien = 0;

    while(tiepTuc) {

        int i;
        int found = 0;
        int idCanBan;
        int soLuongMua;
        // ===== HIEN THI DANH SACH =====
        printf("\n%-5s %-20s %-8s %-10s %-10s\n","ID", "TEN SP", "SIZE", "SO LUONG", "GIA");
        for(i = 0; i < n; i++) {
            printf("%-5d %-20s %-8s %-10d %-10.0f\n",ds[i].id, ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia);
        }
        // =====================================================
        // ===== NHAP ID =====
        // =====================================================

        while(1) {

            idCanBan = nhapSoNguyenDuong("\nNhap ID san pham can mua: ");
            found = 0;
            for(i = 0; i < n; i++) {
                if(ds[i].id == idCanBan) {
                    found = 1;

                    // ===== KIEM TRA HET HANG =====
                    if(ds[i].soLuong <= 0) {
                        printf("\033[1;31mSan pham da het hang!\033[0m\n");
                        found = 0;
                    }
                    break;
                }
            }
            if(found == 1) {
                break;
            }
            printf("\033[1;31mID khong ton tai! Vui long nhap lai.\033[0m\n");
        }

        // =====================================================
        // ===== GIAM GIA =====
        // =====================================================

        if(ds[i].soLuong < 5) {
            ds[i].giamgia = 20;
        }
        else {
            ds[i].giamgia = 0;
        }

        // =====================================================
        // ===== NHAP SO LUONG =====
        // =====================================================

        while(1) {
            soLuongMua = nhapSoNguyenDuong("Nhap so luong mua: ");
            if(soLuongMua <= ds[i].soLuong) {
                break;
            }
            printf("\033[1;31mKhong du hang trong kho!\033[0m\n");
        }

        // =====================================================
        // ===== TINH TIEN =====
        // =====================================================

        float thanhTien;
        thanhTien = soLuongMua * ds[i].Gia * (1 - ds[i].giamgia / 100.0);
        tongTien += thanhTien;

        // =====================================================
        // ===== CAP NHAT KHO =====
        // =====================================================

        ds[i].soLuong -= soLuongMua;
        ds[i].daban += soLuongMua;

        // =====================================================
        // ===== GHI THU CHI =====
        // =====================================================

        ThuChi tc;

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        sprintf(tc.ngay, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        sprintf(tc.moTa, "Ban SP %s Size %s", ds[i].Tensp, ds[i].Size);

        tc.soTien = thanhTien;
        tc.loai = 1;

        ghiGiaoDich(tc);

        // =====================================================
        // ===== THONG BAO =====
        // =====================================================

        printf("\n\033[1;32mBan hang thanh cong!\033[0m\n");

        printf("----------------------------------\n");
        printf("Ten SP     : %s\n", ds[i].Tensp);
        printf("Size       : %s\n", ds[i].Size);
        printf("So luong   : %d\n", soLuongMua);
        printf("Giam gia   : %.0f%%\n", ds[i].giamgia);
        printf("Thanh tien : %.0f VND\n", thanhTien);
        printf("Con lai    : %d\n", ds[i].soLuong);
        printf("----------------------------------\n");

        // =====================================================
        // ===== HOI MUA TIEP =====
        // =====================================================

        int chon;

        printf("\n1. Mua tiep\n");
        printf("0. Ket thuc\n");
        do {
            printf("Nhap lua chon: ");
            scanf("%d", &chon);
            clearBuffer();

        } while(chon != 0 && chon != 1);

        if(chon == 0) {
            tiepTuc = 0;
        }
    }

    // =====================================================
    // ===== HOA DON TONG =====
    // =====================================================

    printf("\n====================================\n");
    printf("TONG TIEN THANH TOAN: %.0f VND\n", tongTien);
    printf("====================================\n");

    pauseConsole();
}

void thongKeKho(int n, Sanpham ds[]) {
    int i;
    int maxBan = 0;
    int coSapHet = 0;
    // ===== TIM SO LUONG BAN CAO NHAT =====
    for(i = 0; i < n; i++) {
        if(ds[i].daban > maxBan) {
            maxBan = ds[i].daban;
        }
    }

    printHeader("BAO CAO THONG KE KHO HANG");

    // =========================================================
    // ===== BANG BAO CAO TONG HOP =====
    // =========================================================

    printf("\n=========================================================================================================\n");

    printf("%-5s %-20s %-8s %-10s %-10s %-10s %-10s %-12s\n","ID", "TEN SP", "SIZE","TON","DA BAN", "HANG LOI","TRA VE", "SAP HET");

    printf("=========================================================================================================\n");
    for(i = 0; i < n; i++) {
        char sapHet[10];

        // ===== KIEM TRA SAP HET =====
        if(ds[i].soLuong < 5) {
            strcpy(sapHet, "CO");
        }
        else {
            strcpy(sapHet, "KHONG");
        } 
        printf("%-5d %-20s %-8s %-10d %-10d %-10d %-10d %-12s\n", ds[i].id, ds[i].Tensp, ds[i].Size, ds[i].soLuong,
                                         ds[i].daban, ds[i].hangLoi, ds[i].hangTra, sapHet);
    }

    printf("=========================================================================================================\n");

    // =========================================================
    // ===== SAN PHAM BAN CHAY =====
    // =========================================================

    printf("\n========== SAN PHAM BAN CHAY NHAT ==========\n");
    if(maxBan == 0) {
        printf("Chua co du lieu ban hang!\n");
    }
    else {
        for(i = 0; i < n; i++) {
            if(ds[i].daban == maxBan) {
                printf("Ten SP  : %s\n", ds[i].Tensp);
                printf("Size    : %s\n", ds[i].Size);
                printf("Da ban  : %d\n", ds[i].daban);
                printf("--------------------------------------\n");
            }
        }
    }

    // =========================================================
    // ===== BO SUNG HANG SAU BAO CAO =====
    // =========================================================

    printf("\n========== KIEM TRA BO SUNG HANG ==========\n");
    for(i = 0; i < n; i++) {
        if(ds[i].soLuong < 5) {
        	coSapHet = 1;
            printf("\nSP: %s | Size: %s\n", ds[i].Tensp, ds[i].Size);
            printf("Ton kho hien tai: %d\n", ds[i].soLuong);

            int them;
            them = nhapSoNguyenDuong("Nhap so luong bo sung: ");
            ds[i].soLuong += them;

            // ===== GHI THU CHI =====
            ThuChi tc;

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            sprintf(tc.ngay, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            sprintf(tc.moTa, "Bo sung hang %s Size %s", ds[i].Tensp, ds[i].Size);

            tc.soTien = them * ds[i].Gia;
            tc.loai = 0;
            ghiGiaoDich(tc);

            printf("\033[1;32mDa bo sung thanh cong!\033[0m\n");
        
	}
	}
        // ===== KHONG CO HANG SAP HET =====
    if(coSapHet == 0) {
    printf("\n\033[1;32mKhong co san pham sap het hang!\033[0m\n");
}
    pauseConsole();
}

void searchSanPham(int n, Sanpham ds[]) {

    char ten[100];
    int i;
    while(1) {

        int found = 0;

        printHeader("TIM KIEM SAN PHAM");

        printf("Nhap ten san pham: ");
        fgets(ten, sizeof(ten), stdin);
        ten[strcspn(ten, "\n")] = 0;

        // ===== CHUAN HOA =====
        chuanHoaTen(ten);

        printf("\n================ KET QUA TIM KIEM ================\n");

        printf("%-5s %-25s %-10s %-10s\n","ID", "TEN SAN PHAM", "SIZE","SO LUONG");

        printf("--------------------------------------------------\n");

        // ==================================================
        // ===== TIM KIEM GAN DUNG =====
        // ==================================================

        for(i = 0; i < n; i++) {

            char temp[100];
            char key[100];

            strcpy(temp, ds[i].Tensp);
            strcpy(key, ten);

            strlwr(temp);
            strlwr(key);

            // ===== CO CHUA TU KHOA =====
            if(strstr(temp, key) != NULL) {

                found = 1;
                printf("%-5d %-25s %-10s %-10d\n", ds[i].id, ds[i].Tensp, ds[i].Size, ds[i].soLuong);
            }
        }

        // ==================================================
        // ===== KHONG TIM THAY =====
        // ==================================================

        if(found == 0) {

            printf("\033[1;31mKhong tim thay san pham!\033[0m\n");
        }

        // ==================================================
        // ===== MENU TIEP TUC =====
        // ==================================================

        int chon;

        printf("\n1. Tim tiep\n");
        printf("0. Quay lai\n");

        do {
            printf("Nhap lua chon: ");
            scanf("%d", &chon);
            clearBuffer();

        } while(chon != 0 && chon != 1);
        if(chon == 0) {
            break;
        }
    }
}

void searchTheoSize(int n, Sanpham ds[]) {

    char size[10];
   	int i;
   	int found;

   	while(1) {

        found = 0;

        printHeader("TIM KIEM SAN PHAM THEO SIZE");

        printf("Nhap size (S/M/L/XL/XXL): ");
        fgets(size, sizeof(size), stdin);
        size[strcspn(size, "\n")] = 0;

        // ===== CHUYEN SIZE THANH IN HOA =====
        strupr(size);

        // ===== KIEM TRA SIZE =====
        if(strcmp(size,"S") != 0 && strcmp(size,"M") != 0 && strcmp(size,"L") != 0 &&  strcmp(size,"XL") != 0 && strcmp(size,"XXL") != 0 && strcmp(size,"XXXL") != 0) {

            printf("\n\033[1;31mSize khong hop le!\033[0m\n");
            pauseConsole();
            continue;
        }

        printf("\n============= KET QUA TIM KIEM =============\n");
        printf("%-5s %-20s %-10s\n", "ID", "TEN SAN PHAM", "SO LUONG");
        printf("------------------------------------------------\n");

        for(i = 0; i < n; i++) {

            char temp[10];

            strcpy(temp, ds[i].Size);
            strupr(temp);

            if(strcmp(temp, size) == 0) {

                found = 1;
                printf("%-5d %-20s %-10d\n", ds[i].id, ds[i].Tensp, ds[i].soLuong);
            }
        }

        if(found == 0) {
            printf("\033[1;31m\nKhong tim thay san pham nao!\033[0m\n");
        }

        printf("\nTim tiep? (Y/N): ");

        char ch;
        scanf("%c", &ch);
        clearBuffer();
        if(ch == 'N' || ch == 'n') {
            break;
        }
    }

    pauseConsole();
}

void sapXepTheoSize(int n, Sanpham ds[]) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(ds[i].Size, ds[j].Size) > 0) {
                Sanpham temp = ds[i];
                ds[i] = ds[j];
                ds[j] = temp;
            }
        }
    }
}
void saveKhoToFile(int n, Sanpham ds[]) {
    FILE *f = fopen(FILE_KHOHANG, "w");
    if(!f) return;
    int i;
    for(i=0; i<n; i++)
       fprintf(f, "%d|%s|%s|%d|%d|%d|%d|%.2f|%.2f\n", ds[i].id, ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].daban,
                                                        ds[i].hangLoi, ds[i].hangTra, ds[i].Gia, ds[i].giamgia );
    fclose(f);
}

int main(){
    int choice;

    do {
        printHeader("QUAN LY KHO HANG SHOP");

        printf("\n");
        printf("=============== MENU KHO HANG ===============\n");

        printf(" [1] Them san pham moi\n");
        printf(" [2] Nhap hang tu file\n");
        printf(" [3] Hien thi ton kho\n");
        printf(" [4] Tim kiem san pham theo ten\n");
        printf(" [5] Tim kiem san pham theo size\n");
        printf(" [6] Ban hang\n");
        printf(" [7] Thong ke kho hang\n");
        printf(" [8] Luu du lieu kho\n");

        printf(" [0] Quay lai\n");

        printf("=============================================\n");

        choice = getKeyboardChoice(8);

        switch(choice) {

            case 1: themSanPham(n, ds); break;

            case 2: loadKhoFromFile(n, ds);
			        pauseConsole(); break;

            case 3: showKho(*n, ds); break;

            case 4: searchSanPham(*n, ds); break;
            
            case 5: searchTheoSize(*n, ds); break;
            
            case 6: banHangKho(*n, ds); break;
            
            case 7: thongKeKho(*n, ds); break;

            case 8: saveKhoToFile(*n, ds);

                printf("\n\033[1;32mDa luu du lieu kho!\033[0m\n");
                pauseConsole(); break;

            case 0:
                printf("\n\033[1;33mDang quay lai menu chinh...\033[0m\n");
                break;

            default:
                printf("\n\033[1;31mLua chon khong hop le!\033[0m\n");
                pauseConsole();
        }

    } while(choice != 0);
}
