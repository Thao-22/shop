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

int getKeyboardChoice(int numOptions) {
    int choice;
    while (1) {
        printf("-> Lua chon cua ban: ");
        if (scanf("%d", &choice) == 1) {
            if (choice >= 0 && choice <= numOptions) {
                clearBuffer();
                return choice;
            } else {
                printf("\033[1;31m(!) Vui long nhap so tu 0 den %d.\033[0m\n", numOptions);
            }
        } else {
            printf("\033[1;31m(!) Lua chon khong hop le. Vui long nhap so.\033[0m\n");
            clearBuffer();
        }
    }
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

void sellProduct(Product *head) {
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
            ghiGiaoDich(tc);
            pauseConsole();
		 	return;
        }
        current = current->next;
    }
    printf("(!) Khong tim thay ID.\n"); 
	clearBuffer(); 
	pauseConsole();
}
void deleteProduct(Product **head) {
    if (*head == NULL) {
        printf("(!) Kho trong.\n");
        pauseConsole();
        return;
    }
    int targetId;
    printf("Nhap ID san pham can xoa: ");
    scanf("%d", &targetId);
    clearBuffer();

    Product *temp = *head, *prev = NULL;
    if (temp != NULL && temp->id == targetId) {
        *head = temp->next; 
        free(temp);         
        printf("\033[1;32m=> Da xoa san pham thanh cong.\033[0m\n");
        pauseConsole();
        return;
    }
    while (temp != NULL && temp->id != targetId) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("(!) Khong tim thay ID.\n");
        pauseConsole();
        return;
    }
    prev->next = temp->next;
    free(temp);
    printf("\033[1;32m=> Da xoa san pham thanh cong.\033[0m\n");
    pauseConsole();
}

void updateProduct(Product *head) {
    if (head == NULL) {
        printf("(!) Kho trong.\n");
        pauseConsole();
        return;
    }
    int targetId;
    printf("Nhap ID san pham can sua: ");
    scanf("%d", &targetId);
    clearBuffer();

    Product *current = head;
    while (current != NULL) {
        if (current->id == targetId) {
            printf("Nhap ten moi (Hien tai: %s): ", current->name);
            fgets(current->name, MAX_STR, stdin);
            current->name[strcspn(current->name, "\n")] = 0; // XÃ³a k? t? \n
            
            printf("Nhap gia moi (Hien tai: %.2f): ", current->price);
            scanf("%f", &current->price);
            clearBuffer();
            
            printf("\033[1;32m=> Da cap nhat thong tin san pham.\033[0m\n");
            pauseConsole();
            return;
        }
        current = current->next;
    }
    printf("(!) Khong tim thay ID de sua.\n");
    pauseConsole();
}

void sortProducts(Product **head) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("(!) Khong du san pham de sap xep.\n");
        pauseConsole();
        return;
    }
    int swapped;
    Product *ptr1;
    Product *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (ptr1->price > ptr1->next->price) { 
                int tempId = ptr1->id;
                char tempName[MAX_STR];
                strcpy(tempName, ptr1->name);
                float tempPrice = ptr1->price;

                ptr1->id = ptr1->next->id;
                strcpy(ptr1->name, ptr1->next->name);
                ptr1->price = ptr1->next->price;

                ptr1->next->id = tempId;
                strcpy(ptr1->next->name, tempName);
                ptr1->next->price = tempPrice;
                
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    printf("\033[1;32m=> Da sap xep danh sach tang dan theo gia tien.\033[0m\n");
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
void exportReport(Product *head) {
    if (head == NULL) {
        printf("(!) Khong co du lieu de xuat bao cao.\n");
        pauseConsole();
        return;
    }

    FILE *f = fopen("BaoCao_ChicFlow.txt", "w");
    if (!f) {
        printf("(!) Loi: Khong the tao file bao cao.\n");
        return;
    }
    fprintf(f, "======================================================================\n");
    fprintf(f, "                 BAO CAO DANH SACH SAN PHAM THIET KE                 \n");
    fprintf(f, "======================================================================\n");
    fprintf(f, "| %-10s | %-30s | %-15s |\n", "ID", "TEN SAN PHAM", "GIA NIEM YET");
    fprintf(f, "----------------------------------------------------------------------\n");

    Product *current = head;
    while (current != NULL) {
        fprintf(f, "| %-10d | %-30s | %-15.2f |\n", current->id, current->name, current->price);
        current = current->next;
    }

    fprintf(f, "======================================================================\n");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "Ngay xuat bao cao: %02d/%02d/%d luc %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(f);
    printf("\033[1;32m=> Da xuat bao cao thanh cong vao file 'BaoCao_ChicFlow.txt'!\033[0m\n");
    pauseConsole();
}
void menuChicFlow(Product **head) {
    while(1) {
        printHeader("QUAN LY SAN PHAM THIET KE (CHIC FLOW)");
        printf("  [1] Them moi san pham\n");
        printf("  [2] Ban hang & Giam gia\n");
        printf("  [3] Sua san pham\n");          
        printf("  [4] Xoa san pham\n");          
        printf("  [5] Sap xep (Theo gia)\n");    
        printf("  [6] Luu du lieu vao tep\n");
        printf("  [7] Xuat bao cao dang bang (.txt)\n");
        printf("  [0] Quay lai menu chinh\n");
        printf("\n\033[1;35m[Vui long nhap so 0-6 de chon...]\033[0m\n");
        int choice = getKeyboardChoice(7); 
        if(choice == 0) break;
        switch(choice) {
            case 1: addProduct(head); break;
            case 2: sellProduct(*head); break;
            case 3: updateProduct(*head); break; 
            case 4: deleteProduct(head); break;  
            case 5: sortProducts(head); break;   
            case 6: saveToFile(*head); break;
            case 7: exportReport(*head); break;
        }
    }
}

// ================= MODULE 2: THU CHI =================
void ghiGiaoDich(ThuChi tc) {
    FILE *file = fopen("thuchi.txt", "a"); 
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
    ghiGiaoDich(tc);
    printf("-> Da luu giao dich thanh cong!\n");
    pauseConsole();
}

void lapBaoCaoThuChi() {
    printHeader("BAO CAO THU CHI DOANH NGHIEP");
    FILE *file = fopen("thuchi.txt", "r");
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

// ================= MODULE 3: NHAN VIEN =================
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


// ================= MODULE 4: KHO HANG =================
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
                ghiGiaoDich(tc);
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
            ThuChi tc = {"Auto", "Nhap them hang Kho", boSung * (ds[i].Gia * 0.6), 0}; 
            ghiGiaoDich(tc);
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
        printf("\n\033[1;35m[Vui long nhap so 0-5 de chon...]\033[0m\n");
        int choice = getKeyboardChoice(5);
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
