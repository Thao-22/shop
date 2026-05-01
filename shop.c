#include "shop.h"

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
void pauseConsole() {
    printf("\n\033[1;33m>>> Nhan Enter de tiep tuc...\033[0m");
    clearBuffer();
}

void setConsoleWindow() {
  
    SetConsoleOutputCP(65001); 
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void printHeader(const char* title) {
    system("cls");
    printf("\033[1;36m"); 
    printf("====================================================\n");
    printf("   %s\n", title);
    printf("====================================================\n");
    printf("\033[0m");  
}


int getMouseMenuChoice(int startY, int numOptions) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT); 

    INPUT_RECORD ir;
    DWORD read;
    while (1) {
        ReadConsoleInput(hInput, &ir, 1, &read);
        if (ir.EventType == MOUSE_EVENT) {
            if (ir.Event.MouseEvent.dwEventFlags == 0 && ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                int y = ir.Event.MouseEvent.dwMousePosition.Y;
                if (y >= startY && y < startY + numOptions) {
                    SetConsoleMode(hInput, prev_mode); 
                    return y - startY + 1; 
                } else if (y == startY + numOptions) {
                    SetConsoleMode(hInput, prev_mode);
                    return 0; 
                }
            }
        }
        if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
            char c = ir.Event.KeyEvent.uChar.AsciiChar;
            if (c >= '0' && c <= '0' + numOptions) {
                SetConsoleMode(hInput, prev_mode);
                return c - '0';
            }
        }
    }
}

// ================= MODULE 2: THU CHI =================
void ghiGiaoDichVaoFile(ThuChi tc) {
    FILE *file = fopen(FILE_THUCHI, "a"); 
    if (file == NULL) return;
    fprintf(file, "%s,%s,%.2f,%d\n", tc.ngay, tc.moTa, tc.soTien, tc.loai);
    fclose(file);
}

void themGiaoDich() {
    printHeader("NHAP GIAO DICH THU/CHI MOI");
    ThuChi tc;
    printf("Nhap ngay (dd/mm/yyyy): "); 
	scanf("%19s", tc.ngay); 
	clearBuffer();
    printf("Nhap mo ta giao dich: ");
    fgets(tc.moTa, sizeof(tc.moTa), stdin);
    tc.moTa[strcspn(tc.moTa, "\n")] = 0;
    printf("Nhap so tien: "); 
	scanf("%f", &tc.soTien);
    printf("Loai (1: THU, 0: CHI): "); 
	scanf("%d", &tc.loai);
    ghiGiaoDichVaoFile(tc);
    printf("\033[1;32m-> Da luu giao dich thanh cong!\033[0m\n");
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
		if(!token) 
			continue; 
		strcpy(tc.ngay, token);
        token = strtok(NULL, ","); 
		if(!token) continue; strcpy(tc.moTa, token);
        token = strtok(NULL, ","); 
		if(!token) continue; tc.soTien = atof(token);
        token = strtok(NULL, ","); 
		if(!token) continue; tc.loai = atoi(token);
        printf("%-12s | %-25s | %-15.2f | %-10s\n", tc.ngay, tc.moTa, tc.soTien, tc.loai == 1 ? "\033[1;32mThu\033[0m" : "\033[1;31mChi\033[0m");
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

// ================= MODULE 1: CHIC FLOW =================
void addProduct(Product **head) {
    Product *newNode = (Product *)malloc(sizeof(Product));
    printf("Nhap ID san pham thiet ke: "); 
	scanf("%d", &newNode->id); 
	clearBuffer(); 
    printf("Nhap ten san pham: ");
    fgets(newNode->name, MAX_STR, stdin);
 	newNode->name[strcspn(newNode->name, "\n")] = 0; 
    printf("Nhap gia niem yet: "); 
	scanf("%f", &newNode->price); 
	clearBuffer();
    newNode->next = *head; *head = newNode;
    printf("\033[1;32m=> Da them san pham vao kho.\033[0m\n"); 
	pauseConsole();
}

void sellProductChicFlow(Product *head) {
    if (head == NULL) { 
		printf("(!) Kho trong.\n"); 
		pauseConsole(); 
		return; 
	}
    int targetId; float discount = 0; 
	Product *current = head;
    printf("-> Nhap ID can mua: "); 
	scanf("%d", &targetId);
    while (current != NULL) {
        if (current->id == targetId) {
            printf("[Tim thay] %s - Gia: %.2f\n", current->name, current->price);
			printf("-> %% giam gia (0 neu khong): "); 
			scanf("%f", &discount); clearBuffer();
            float finalPrice = current->price * (1.0f - (discount / 100.0f));
            printf("\n\033[1;32mDA BAN THANH CONG: %.2f\033[0m\n", finalPrice);
            ThuChi tc = {"Auto", "Ban SP Thiet Ke (ChicFlow)", finalPrice, 1};
            ghiGiaoDichVaoFile(tc);
            pauseConsole();
		 	return;
        }
        current = current->next;
    }
    printf("(!) Khong tim thay ID.\n"); 
	clearBuffer(); 
	pauseConsole();
}

void saveToFile(Product *head) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) 
		return;
    Product *current = head;
    while (current != NULL) {
        fprintf(f, "%d|%s|%.2f\n", current->id, current->name, current->price);
        current = current->next;
    }
    fclose(f);
    printf("\033[1;32m=> Da luu du lieu Chic Flow.\033[0m\n"); 
	pauseConsole();
}

void loadFromFile(Product **head) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) 
		return; 
    while (1) {
        Product *newNode = (Product *)malloc(sizeof(Product));
        if (fscanf(f, "%d|%[^|]|%f\n", &newNode->id, newNode->name, &newNode->price) == 3) {
        	newNode->next = *head; *head = newNode;
        } 
		else { 
			free(newNode); 
			break; 
		}
    }
    fclose(f);
}

void freeList(Product *head) {
    while (head != NULL) { 
		Product *temp = head; 
		head = head->next; 
		free(temp); 
	}
}

void menuChicFlow(Product **head) {
    while(1) {
        printHeader("QUAN LY SAN PHAM THIET KE (CHIC FLOW)");
        printf("  [1] Them moi san pham\n");
        printf("  [2] Ban hang & Giam gia\n");
        printf("  [3] Luu du lieu vao tep\n");
        printf("  [0] Quay lai menu chinh\n");
        printf("\n\033[1;35m[Dung chuot click hoac an so de chon...]\033[0m");
        int choice = getMouseMenuChoice(3, 3);
        if(choice == 0) break;
        switch(choice) {
            case 1: addProduct(head); break;
            case 2: sellProductChicFlow(*head); break;
            case 3: saveToFile(*head); break;
        }
    }
}

// ================= MODULE 3: NHÂN VIÊN =================
void nhapNV(int *n, NhanVien a[]) {
    printf("Nhap so luong nhan vien: "); 
	scanf("%d", n); 
    while (*n <= 0) { 
	printf("Nhap lai: "); 
	scanf("%d", n); } 
    for(int i = 0; i < *n; i++) {
        printf("\nNV %d:\n", i+1); 
		clearBuffer();
        printf("Ho ten: "); 
		fgets(a[i].hoTen, sizeof(a[i].hoTen), stdin); 
		a[i].hoTen[strcspn(a[i].hoTen, "\n")] = '\0';	
        printf("Tuoi: "); 
		scanf("%d", &a[i].tuoi);
        printf("Gioi tinh: "); 
		scanf("%s", a[i].gioiTinh);
        printf("He so luong: "); 
		scanf("%f",&a[i].heSoluong); 
    } 
}

void xuatNV(int n, NhanVien a[]) {
    printf("\n----- DANH SACH NHAN VIEN -----\n"); 
    for(int i=0; i<n; i++) {
        printf("%d. %s - %d tuoi - %s - HSL: %.2f\n", i+1, a[i].hoTen, a[i].tuoi, a[i].gioiTinh, a[i].heSoluong);
    } 
    printf("\nTong HSL: %.2f | Trung binh: %.2f\n", tongHeSoLuong(n,a), trungBinhHeSoLuong(n,a));
    pauseConsole();
} 

void themNhanVien(int *n, NhanVien a[]) {
    int k; printf("So luong can them: "); scanf("%d", &k);
    for (int i=0 ; i<k ;i++) {
        printf("\nNV moi thu %d:\n", i+1); clearBuffer();
        printf("Ho ten: "); fgets(a[*n].hoTen, sizeof(a[*n].hoTen), stdin); a[*n].hoTen[strcspn(a[*n].hoTen, "\n")] = '\0';
        printf("Tuoi: "); scanf("%d", &a[*n].tuoi);
        printf("Gioi tinh: "); scanf("%s", a[*n].gioiTinh);
        printf("He so luong: "); scanf("%f", &a[*n].heSoluong);
        (*n)++; 
    }
}

void xoaNhanVien(int *n, NhanVien a[]) {
    int viTri; 
	printf("Vi tri xoa (1 -> %d): ", *n); 
	scanf("%d", &viTri);
    if (viTri < 1 || viTri > *n) 
		return;
    for (int i = viTri-1 ; i<*n - 1 ; i++) 
		a[i] = a[i + 1];
    (*n)--; 
	printf("\033[1;32mDa xoa!\033[0m\n"); 
	clearBuffer(); 
	pauseConsole();
}  

float tongHeSoLuong(int n, NhanVien a[]) { 
	float t = 0; 
	for(int i=0; i<n; i++) t += a[i].heSoluong; 
	return t; 
}
float trungBinhHeSoLuong(int n, NhanVien a[]) { 
	return n == 0 ? 0 : 
	tongHeSoLuong(n, a)/n; 
}

void menuNhanVien(int *n, NhanVien a[]) {
    while(1) {
        printHeader("QUAN LY NHAN SUNG");
        printf("  [1] Nhap danh sach ban dau\n");
        printf("  [2] Xem danh sach NV\n");
        printf("  [3] Them nhan vien\n");
        printf("  [4] Xoa nhan vien\n");
        printf("  [0] Quay lai\n");
        printf("\n\033[1;35m[Dung chuot click hoac an so de chon...]\033[0m");
        int choice = getMouseMenuChoice(3, 4);
        if(choice == 0) break;
        switch(choice) {
            case 1: nhapNV(n, a); break;
            case 2: xuatNV(*n, a); break;
            case 3: themNhanVien(n, a); break;
            case 4: xoaNhanVien(n, a); break;
        }
    }
}

// ================= MODULE 4: KHO HÀNG =================
int nhapSoLuong() {
    int x; char c;
    while(1) {
        printf("- So luong: ");
        if(scanf("%d", &x) == 1) {
            if((c = getchar()) != '\n') { 
				while(getchar() != '\n'); 
				continue;
			}
            if(x > 0) 
				return x; 
			else 
				printf("So luong phai > 0!\n");
        } 
		else { 
			while(getchar() != '\n'); 
		}
    }
}

void nhapHang(int *n, Sanpham ds[]) {
    int m; 
	printf("Nhap so luong mau can them: "); 
	scanf("%d",&m); 
	getchar();
    for(int i=0; i<m; i++) {
        char ten[100], size[10]; 
		int sl; 
		float gia;
        printf("\nSP %d:\n- Ten: ", i+1); 
		fgets(ten,sizeof(ten),stdin); 
		ten[strcspn(ten,"\n")]=0;
        printf("- Size: "); 
		fgets(size,sizeof(size),stdin); 
		size[strcspn(size,"\n")]=0;
        sl = nhapSoLuong();
        int found = -1;
        for(int j=0; j<*n; j++) 
			if(strcmp(ds[j].Tensp,ten)==0 && strcmp(ds[j].Size,size)==0) { 
				found=j; 
				break; 
			}
        if(found != -1) { 
			ds[found].soLuong += sl; 
			printf("\033[1;33mTrung -> da cong don kho\033[0m\n"); }
        else {
            printf("- Gia: "); 
			scanf("%f",&gia); 
			getchar();
            strcpy(ds[*n].Tensp,ten); 
			strcpy(ds[*n].Size,size);
            ds[*n].Gia=gia; 
			ds[*n].soLuong=sl; 
			ds[*n].daban=0; 
			ds[*n].giamgia=0;
            (*n)++;
        }
    }
    pauseConsole();
}

void showKho(int n, Sanpham ds[]) {
    printHeader("DANH SACH KHO HANG");
    printf("%-20s %-6s %-10s %-10s %-10s %-10s\n","Ten san pham","Size","So Luong","Gia","Giam gia","Da ban");
    for(int i=0; i<n; i++) {
        printf("%-20s %-6s %-10d %-10.0f %-10.0f %-10d\n", ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia, ds[i].giamgia, ds[i].daban);
    }
    pauseConsole();
}

void discountKho(int n, Sanpham ds[]) {
    for(int i=0; i<n; i++) 
		ds[i].giamgia = (ds[i].soLuong < 5 && ds[i].soLuong > 0) ? 20 : 0;
}

void sellKho(int n, Sanpham ds[]) {
    char ten[100], size[10]; 
	int sl;
    printf("Ten san pham can ban: "); 
	fgets(ten,sizeof(ten),stdin); 
	ten[strcspn(ten,"\n")]=0;
    printf("Size: "); 
	fgets(size,sizeof(size),stdin); 
	size[strcspn(size,"\n")]=0;
    sl = nhapSoLuong();
    int found = 0;
    for(int i=0; i<n; i++) {
        if(strcmp(ds[i].Tensp,ten) == 0 && strcmp(ds[i].Size,size) == 0) {
            found = 1;
            if(ds[i].soLuong >= sl) {
                discountKho(n, ds);
                float giasauGiam = sl * ds[i].Gia * (1 - ds[i].giamgia/100);
                ds[i].soLuong -= sl; ds[i].daban += sl;
                printf("\033[1;32mBan thanh cong! Thu duoc: %.2f\033[0m\n", giasauGiam);
                ThuChi tc = {"Auto", "Ban SP Kho", giasauGiam, 1};
                ghiGiaoDichVaoFile(tc);
            } else printf("Kho khong du!\n");
            break;
        }
    }
    if(found == 0) printf("Khong tim thay!\n");
    pauseConsole();
}

void kiemTraKho(int n, Sanpham ds[]) {
    int check = 0;
    for(int i=0; i<n; i++) {
        if(ds[i].soLuong < 2) {
            check = 1; int boSung;
            printf("\nSP %s (Size %s) sap het. Nhap SL bo sung: ", ds[i].Tensp, ds[i].Size);
            while(1) {
                if(scanf("%d", &boSung) == 1 && boSung > 0) break;
                else { printf("Nhap lai: "); while(getchar()!='\n'); }
            }
            ds[i].soLuong += boSung;
            printf("\033[1;32mDa bo sung!\033[0m\n");
            ThuChi tc = {"Auto", "Nhap them hang Kho", boSung * (ds[i].Gia * 0.6), 0}; // Gia dinh chi phi nhap bang 60% gia ban
            ghiGiaoDichVaoFile(tc);
        }
    }
    if(check == 0) printf("\nKhong co hang sap het.\n");
    clearBuffer(); pauseConsole();
}

void favProducts(int n, Sanpham ds[]) {
    int max = -1, index = -1;
    for(int i=0; i<n; i++) 
		if(ds[i].daban > max) { 
			max = ds[i].daban; index = i; 
		}
    if(index != -1 && max > 0) {
        printf("--- BAN CHAY NHAT ---\n");
        for(int i=0; i<n; i++) 
			if(ds[i].daban == max) 
				printf("%s (Size %s) - Da ban: %d\n", ds[i].Tensp, ds[i].Size, ds[i].daban);
    } 
	else 
		printf("Chua co data ban hang.\n");
    pauseConsole();
}

void menuKhoHang(int *n, Sanpham ds[]) {
    while(1) {
        printHeader("QUAN LY KHO HANG SHOP");
        printf("  [1] Nhap hang vao kho\n");
        printf("  [2] Hien thi ton kho\n");
        printf("  [3] Ban hang kho\n");
        printf("  [4] Kiem tra & Bo sung\n");
        printf("  [5] San pham ban chay\n");
        printf("  [0] Quay lai\n");
        printf("\n\033[1;35m[Dung chuot click hoac an so de chon...]\033[0m");
        int choice = getMouseMenuChoice(3, 5);
        if(choice == 0) break;
        switch(choice) {
            case 1: nhapHang(n, ds); break;
            case 2: showKho(*n, ds); break;
            case 3: sellKho(*n, ds); break;
            case 4: kiemTraKho(*n, ds); break;
            case 5: favProducts(*n, ds); break;
        }
    }
}
