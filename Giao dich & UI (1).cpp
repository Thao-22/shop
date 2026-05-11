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

    printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "SO LUONG", "�A BAN");
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
                printf("\n\033[1;32m=> Thanh toan: %.2f VND th�nh c�ng!\033[0m\n", tongTien);
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
        printf("\033[1;32m--- DANH SACH SAN PHAM BAN CHAY (�A b�n: %d) ---\033[0m\n", maxBan);
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
