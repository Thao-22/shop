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
int isIdExists(Product *head, int id) {
    Product *curr = head;
    while (curr != NULL) {
        if (curr->id == id) {
            return 1; // ID chua ton tai 
        }
        curr = curr->next;
    }
    return 0; // ID da ton tai 
}
void importFromWarehouse(Product **head, int *nKho, Sanpham dsKho[]) {
    printHeader("NHAP HANG TU KHO VAO CUA HANG");
    int i;
    if (*nKho == 0) {
        printf("(!) Kho hien dang trong.\n");
        pauseConsole();
        return;
    }
    printf("\n%-5s | %-10s | %-20s | %-8s | %-10s\n", "STT", "ID KHO", "Ten san pham", "Size", "Ton kho");
    printf("----------------------------------------------------------------------\n");
    for (i = 0; i < *nKho; i++) {
        printf("%-5d | %-10d | %-20s | %-8s | %-10d\n", 
                i + 1, dsKho[i].id, dsKho[i].Tensp, dsKho[i].Size, dsKho[i].soLuong);
    }

    int targetId, sl;
    printf("\n-> Nhap ID KHO cua san pham muon lay: ");
    scanf("%d", &targetId);
    clearBuffer();

    int idx = -1;
    for (i = 0; i < *nKho; i++) {
        if (dsKho[i].id == targetId) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("\033[1;31m(!) Khong tim thay ID %d trong kho.\033[0m\n", targetId);
    } else {
        printf("San pham: %s | Ton kho: %d\n", dsKho[idx].Tensp, dsKho[idx].soLuong);
        printf("Nhap so luong muon chuyen: ");
        scanf("%d", &sl); clearBuffer();

        if (sl > 0 && sl <= dsKho[idx].soLuong) {
            dsKho[idx].soLuong -= sl;
            Product *newNode = (Product *)malloc(sizeof(Product));
            newNode->id = dsKho[idx].id; 
            strcpy(newNode->name, dsKho[idx].Tensp);
            newNode->price = dsKho[idx].Gia;
            newNode->soLuong = sl;
            newNode->giamgia = 0;
            newNode->daban = 0;
            newNode->next = *head;
            *head = newNode;

            printf("\033[1;32m=> Da chuyen thanh cong san pham ID %d ra cua hang!\033[0m\n", targetId);
        } else {
            printf("\033[1;31m(!) So luong khong hop le.\033[0m\n");
        }
    }
    pauseConsole();
}
void displayStoreProducts(Product *head) {
    printHeader("DANH SACH SAN PHAM DANG DUOC BAY BAN");
    
    if (head == NULL) {
        printf("\033[1;31m(!) Cua hang hien dang trong.\033[0m\n");
        pauseConsole();
        return;
    }

    printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "SO LUONG", "ÐA BAN");
    printf("--------------------------------------------------------------------------\n");

    Product *current = head;
    while (current != NULL) {
        printf("%-10d | %-25s | %-10.2f | %-10d | %-10d\n", 
               current->id, current->name, current->price, current->soLuong, current->daban);
        current = current->next;
    }

    printf("--------------------------------------------------------------------------\n");
    pauseConsole();
}
void discount(Product *head) {
    Product *current = head;
    while (current != NULL) {
        if (current->soLuong < 5 && current->soLuong > 0) {
            current->giamgia = 20; 
        } else {
            current->giamgia = 0;
        }
        current = current->next; 
    }
}

void sellAtStore(Product *head) {
    int targetId, sl;
    printHeader("BAN SAN PHAM THIET KE");
    printf("Nhap ID san pham can mua: ");
    scanf("%d", &targetId); clearBuffer();

    Product *current = head;
    int found = 0;
    while (current != NULL) {
        if (current->id == targetId) {
            found = 1;
            if (current->soLuong < 5 && current->soLuong > 0) current->giamgia = 20;
            else current->giamgia = 0;
            printf("San pham: %s | Kho: %d | Gia: %.2f | Giam: %.0f%%\n", current->name, current->soLuong, current->price, current->giamgia);
            printf("Nhap so luong mua: ");
            scanf("%d", &sl); clearBuffer();
            if (current->soLuong >= sl && sl > 0) {
                float tongTien = sl * current->price * (1 - current->giamgia / 100);
                current->soLuong -= sl; 
                current->daban += sl;
                printf("\n\033[1;32m=> Thanh toan: %.2f VND thanh cong!\033[0m\n", tongTien);
                ThuChi tc;
                time_t t = time(NULL); struct tm tm = *localtime(&t);
                sprintf(tc.ngay, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                sprintf(tc.moTa, "Ban thiet ke ID %d", current->id);
                tc.soTien = tongTien; tc.loai = 1; 
                ghiGiaoDich(tc);
            } else {
                printf("\033[1;31m(!) Khong du hang.\033[0m\n");
            }
            break;
        }
        current = current->next;
    }
    if (!found) printf("\033[1;31m(!) ID khong ton tai!\033[0m\n");
    pauseConsole();
}

void deleteProduct(Product **head) {
    printHeader("XOA SAN PHAM CUA HANG");
    if(*head == NULL) {
        printf("Cua hang trong!\n");
        pauseConsole();
        return;
    }
    int id;
    printf("Nhap ID san pham can xoa: ");
    if(scanf("%d", &id) != 1) {
        clearBuffer();
        printf("ID khong hop le!\n");
        pauseConsole();
        return;
    }
    clearBuffer();
    Product *curr = *head;
    Product *prev = NULL;
    while(curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    if(curr == NULL) {
        printf("Khong tim thay san pham voi ID %d\n", id);
        pauseConsole();
        return;
    }
    
    // Xac nhan truoc khi xoa 
    char confirm;
    printf("Ban co chac chan muon xoa san pham '%s' (ID: %d)? (y/n): ", curr->name, curr->id);
    scanf("%c", &confirm);
    clearBuffer();
    if(confirm != 'y' && confirm != 'Y') {
        printf("Da huy thao tac xoa!\n");
        pauseConsole();
        return;
    }

    if(prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
    printf("Xoa san pham thanh cong!\n");
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
            current->name[strcspn(current->name, "\n")] = 0; 
            
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
                char tempName[MAX_STR]; strcpy(tempName, ptr1->name);
                float tempPrice = ptr1->price;
                int tempSoLuong = ptr1->soLuong;
                float tempGiamGia = ptr1->giamgia;
                int tempDaBan = ptr1->daban;

                ptr1->id = ptr1->next->id;
                strcpy(ptr1->name, ptr1->next->name);
                ptr1->price = ptr1->next->price;
                ptr1->soLuong = ptr1->next->soLuong;
                ptr1->giamgia = ptr1->next->giamgia;
                ptr1->daban = ptr1->next->daban;

                ptr1->next->id = tempId;
                strcpy(ptr1->next->name, tempName);
                ptr1->next->price = tempPrice;
                ptr1->next->soLuong = tempSoLuong;
                ptr1->next->giamgia = tempGiamGia;
                ptr1->next->daban = tempDaBan;
                
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("\033[1;32m=> Da sap xep danh sach tang dan theo gia tien.\033[0m\n");
    pauseConsole();
}
void favProductsChicFlow(Product *head) {
    printHeader("San pham ban chay nhat");

    if (head == NULL) {
        printf("\033[1;31m(!) Hien chua co san pham nao tai shop.\033[0m\n");
        pauseConsole();
        return;
    }

    // Tim so luong ban ra lon  
    int maxBan = -1;
    Product *current = head;
    while (current != NULL) {
        if (current->daban > maxBan) {
            maxBan = current->daban;
        }
        current = current->next;
    }

    // Kiem tra va in ket qua 
    if (maxBan <= 0) {
        printf("\033[1;33m(!) CHUA CO DU LIEU BAN HANG CHO SAN PHAM NAO6.\033[0m\n");
    } else {
        printf("\033[1;32m--- DANH SACH SAN PHAM BAN CHAY (ÐA bán: %d) ---\033[0m\n", maxBan);
        printf("%-10s | %-30s | %-15s\n", "ID", "Ten san pham", "Gia ban");
        printf("------------------------------------------------------------\n");
        
        current = head;
        while (current != NULL) {
            if (current->daban == maxBan) {
                printf("%-10d | %-30s | %-15.2f\n", current->id, current->name, current->price);
            }
            current = current->next;
        }
    }
    pauseConsole();
}

void saveToFile(Product *head) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) return;
    Product *current = head;
    while (current != NULL) {
        fprintf(f, "%d|%s|%.2f|%d|%.2f|%d\n", current->id, current->name, current->price, current->soLuong, current->giamgia, current->daban);
        current = current->next;
    }
    fclose(f);
    printf("\033[1;32m=> Da luu du lieu Chic Flow.\033[0m\n"); 
	pauseConsole();
}

void loadFromFile(Product **head) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return; 
    Product temp;
    while (fscanf(f, "%d|%[^|]|%f|%d|%f|%d\n", &temp.id, temp.name, &temp.price, &temp.soLuong, &temp.giamgia, &temp.daban) == 6) {
        Product *newNode = (Product *)malloc(sizeof(Product));
        *newNode = temp; 
        newNode->next = *head; 
        *head = newNode;
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

void menuChicFlow(Product **head, int *nKho, Sanpham dsKho[]) {
    while(1) {
        printHeader("QUAN LY CUA HANG (CHIC FLOW)");
        printf("  [1] Nhap hang tu kho vao cua hang\n");
        printf("  [2] Danh sach san pham dang duoc bay ban\n"); 
        printf("  [3] Ban hang\n");
        printf("  [4] Sua thong tin san pham\n");
        printf("  [5] Xoa san pham\n");
        printf("  [6] Sap xep theo gia\n");
        printf("  [7] San pham ban chay nhat\n");
        printf("  [8] Luu vao file\n");
        printf("  [0] Quay lai\n");
        
        int choice = getKeyboardChoice(8);
        if(choice == 0) break;
        switch(choice) {
            case 1: importFromWarehouse(head, nKho, dsKho); break;
            case 2: displayStoreProducts(*head); break;
            case 3: sellAtStore(*head); break; 
            case 4: updateProduct(*head); break;
            case 5: deleteProduct(head); break;
            case 6: sortProducts(head); break;
            case 7: favProductsChicFlow(*head); break;
            case 8: saveToFile(*head); break;
        }
    }
}


// ================= MODULE 2: THU CHI =================
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
    int i;
    for(i = 0; i < strlen(tc.moTa); i++) {
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
        tc.soTien = atof(token);
        
        token = strtok(NULL, ","); 
		if(!token) continue; 
        tc.loai = atoi(token);
        
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


// ================= MODULE 3: NHAN VIEN =================
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
                if (*n == 0) {
                    printf("Chua co nhan vien nao!\n");
                    pauseConsole(); 
                } else {
                    tinhLuong(*n, a);
                    pauseConsole();
                }
                break;
        }
    }
}

// ================= MODULE 4: KHO HANG =================
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

void menuKhoHang(int *n, Sanpham ds[]) {
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
