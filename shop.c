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

    printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "SO LUONG", "DA BAN");
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
    if (head == NULL) {
        printf("\033[1;31m(!) Cua hang hien dang trong.\033[0m\n");
        pauseConsole();
        return;
    }

    printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "SO LUONG", "DA BAN");
    printf("--------------------------------------------------------------------------\n");

    Product *c = head;
    while (c != NULL) {
        printf("%-10d | %-25s | %-10.2f | %-10d | %-10d\n", 
               c->id, c->name, c->price, c->soLuong, c->daban);
        c = c->next;
    }

    printf("--------------------------------------------------------------------------\n");
    printf("Nhap ID san pham can mua: ");
    scanf("%d", &targetId); 
	clearBuffer();

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
    printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "SO LUONG", "DA BAN");
    printf("--------------------------------------------------------------------------\n");

    Product *c = *head;
    while (c != NULL) {
        printf("%-10d | %-25s | %-10.2f | %-10d | %-10d\n", 
               c->id, c->name, c->price, c->soLuong, c->daban);
        c = c->next;
    }

    printf("--------------------------------------------------------------------------\n");
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
        printf("\033[1;32m--- DANH SACH SAN PHAM BAN CHAY (DA BAN: %d) ---\033[0m\n", maxBan);
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

    fprintf(f, "==========================================================================================\n");
    fprintf(f, "                         BAO CAO DOANH THU & TON KHO SAN PHAM                             \n");
    fprintf(f, "==========================================================================================\n");
    fprintf(f, "| %-8s | %-25s | %-12s | %-10s | %-10s |\n", "ID", "TEN SAN PHAM", "GIA NIEM YET", "TON KHO", "DA BAN");
    fprintf(f, "------------------------------------------------------------------------------------------\n");

    Product *current = head;
    int tongTon = 0;
    int tongBan = 0;

    while (current != NULL) {
        fprintf(f, "| %-8d | %-25s | %-12.2f | %-10d | %-10d |\n", 
                current->id, current->name, current->price, current->soLuong, current->daban);
        tongTon += current->soLuong;
        tongBan += current->daban;
        
        current = current->next;
    }

    fprintf(f, "------------------------------------------------------------------------------------------\n");
    fprintf(f, "| %-50s | %-10d | %-10d |\n", "TONG CONG:", tongTon, tongBan);
    fprintf(f, "==========================================================================================\n");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "Ngay xuat bao cao: %02d/%02d/%d luc %02d:%02d:%02d\n", 
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    fclose(f);
    printf("\033[1;32m=> Da xuat bao cao chi tiet vao file 'BaoCao_ChicFlow.txt'!\033[0m\n");
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
            case 8: exportReport(*head); break;
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
    printf("========================================================================================\n");
    printf("%-5s %-25s %-8s %-15s %-15s %-15s\n", "STT","HO TEN", "TUOI", "GIOI TINH", "CHUC VU", "HE SO LUONG");
    printf("========================================================================================\n");

    int i;
    for(i = 0; i < n; i++) {
        printf("%-5d %-25s %-8d %-15s %-15s %-15.2f\n", i+1, a[i].hoTen, a[i].tuoi,a[i].gioiTinh, a[i].chucVu, a[i].heSoluong);
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
        printf("%-15s" " | ", a[i].hoTen);

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
int nhapSoLuong() {
    int x;
    while(1) {
        printf("- So luong: ");
        if(scanf("%d", &x) == 1 && x > 0) {
            clearBuffer();
            return x;
        } else {
            printf("\033[1;31m(!) Loi: Vui long nhap so nguyen duong.\033[0m\n");
            clearBuffer(); 
        }
    }
}

void nhapHang(int *n, Sanpham ds[]) {
    int m; 
    printf("Nhap so luong mau can them: "); 
    scanf("%d", &m); 
    getchar(); 
    int i;
    for(i = 0; i < m; i++) {
        char ten[100], size[10]; 
        int sl, idMoi; 
        float gia;

        printf("\nSP %d:\n", i + 1);
        printf("- Nhap ID cho san pham: ");
        scanf("%d", &idMoi);
        clearBuffer(); 

        printf("- Ten: "); 
        fgets(ten, sizeof(ten), stdin); 
        ten[strcspn(ten, "\n")] = 0;

        printf("- Size: "); 
        fgets(size, sizeof(size), stdin); 
        size[strcspn(size, "\n")] = 0;
  
        sl = nhapSoLuong();
        int found = -1;
        int j;
        for(j = 0; j < *n; j++) {
            if(ds[j].id == idMoi || (strcmp(ds[j].Tensp, ten) == 0 && strcmp(ds[j].Size, size) == 0)) { 
                found = j; 
                break; 
            }
        }

        if(found != -1) { 
            ds[found].soLuong += sl; 
            printf("\033[1;33mSan pham da ton tai (ID: %d) -> Da cong don so luong.\033[0m\n", ds[found].id); 
        } 
		else {
            printf("- Gia: "); 
            scanf("%f", &gia); 
            clearBuffer(); 
            
            float tongChi = sl * gia;

            ThuChi tc;
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            sprintf(tc.ngay, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            sprintf(tc.moTa,"Nhap kho SP ID %d - %s", idMoi, ten);

            tc.soTien = tongChi;
            tc.loai = 0;
            ghiGiaoDich(tc);

            ds[*n].id = idMoi; 
            strcpy(ds[*n].Tensp, ten); 
            strcpy(ds[*n].Size, size);
            ds[*n].Gia = gia; 
            ds[*n].soLuong = sl;
            (*n)++;
            printf("\033[1;32mDa them san pham moi voi ID %d thanh cong!\033[0m\n", idMoi);
        }
    }
    pauseConsole();
}
void showKho(int n, Sanpham ds[]) {
    printHeader("DANH SACH KHO HANG");
    printf("%-20s %-6s %-10s %-10s \n","Ten san pham","Size","So Luong","Gia");
    int i;
    for(i=0; i<n; i++) {
        printf("%-20s %-6s %-10d %-10.0f \n", ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia);
    }
    pauseConsole();
}

void kiemTraKho(int n, Sanpham ds[]) {
    int check = 0;
    int i;
    for(i=0; i<n; i++) {
        if(ds[i].soLuong < 5) {
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

void saveKhoToFile(int n, Sanpham ds[]) {
    FILE *f = fopen(FILE_KHOHANG, "w");
    if(!f) return;
    int i;
    for(i=0; i<n; i++)
        fprintf(f, "%s|%s|%d|%.2f|\n", ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia);
    printf("Da luu du lieu vao file thanh cong!"); 
    pauseConsole(); 
    fclose(f);
}

void loadKhoFromFile(int *n, Sanpham ds[]) {
    FILE *f = fopen(FILE_KHOHANG, "r");
    if(!f) return;
    *n = 0;
    while(fscanf(f, "%[^|]|%[^|]|%d|%f|\n", ds[*n].Tensp, ds[*n].Size, &ds[*n].soLuong, &ds[*n].Gia) == 4) {
        (*n)++;
    }
    fclose(f);
}
void menuKhoHang(int *n, Sanpham ds[]) {
    while(1) {
        printHeader("QUAN LY KHO HANG SHOP");
        printf("  [1] Nhap hang vao kho\n");
        printf("  [2] Hien thi ton kho\n");
        printf("  [3] Kiem tra & Bo sung\n");
        printf("  [4] Luu du lieu vao file\n"); 
        printf("  [0] Quay lai\n");
        printf("\n\033[1;35m[Vui long nhap so 0-4 de chon...]\033[0m\n");
        
        int choice = getKeyboardChoice(4); 
        if(choice == 0) break;
        switch(choice) {
            case 1: nhapHang(n, ds); break;
            case 2: showKho(*n, ds); break;
            case 3: kiemTraKho(*n, ds); break;
            case 4: saveKhoToFile(*n, ds); break;
        }
        
        
    }
}
