#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef CHIC_FLOW_H
#define CHIC_FLOW_H
#define MAX_STR 100
#define FILE_NAME "chic_flow_data.txt"
#endif
typedef struct Product {
    int id;
    char name[MAX_STR];
    float price;
    int soLuong;
    float giamgia;
    int daban;
    struct Product *next;
} Product;
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseConsole() {
    printf("\n\033[1;33m>>> Nhan Enter de tiep tuc...\033[0m");
    clearBuffer();
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
void displayStoreProducts(Product *head) {
    printHeader("DANH SACH SAN PHAM CUA HÀNG");
    if (head == NULL) {
        printf("\033[1;31m(!) Cua hang hien dang trong.\033[0m\n");
    } else {
        printf("%-10s | %-25s | %-10s | %-10s | %-10s\n", "ID", "TEN SAN PHAM", "GIA", "TON KHO", "DA BAN");
        printf("--------------------------------------------------------------------------\n");
        Product *curr = head;
        while (curr != NULL) {
            printf("%-10d | %-25s | %-10.2f | %-10d | %-10d\n", 
                   curr->id, curr->name, curr->price, curr->soLuong, curr->daban);
            curr = curr->next;
        }
    }
    pauseConsole();
}

void addProduct(Product **head) {
    printHeader("THEM MOI SAN PHAM THIET KE");
    Product *newNode = (Product *)malloc(sizeof(Product));
    printf("Nhap ID san pham: ");
    scanf("%d", &newNode->id);
    clearBuffer(); 
    printf("Nhap ten san pham: ");
    fgets(newNode->name, MAX_STR, stdin);
    newNode->name[strcspn(newNode->name, "\n")] = 0; 
    printf("Nhap gia niem yet: ");
    scanf("%f", &newNode->price);
    printf("Nhap so luong nhap kho: ");
    scanf("%d", &newNode->soLuong);
    newNode->giamgia = 0;
    newNode->daban = 0;
    newNode->next = *head;
    *head = newNode;
    printf("\033[1;32m=> Da them san pham vao he thong thanh cong.\033[0m\n");
    clearBuffer();
    pauseConsole();
}
void sellProduct(Product *head) {
    printHeader("BAN HANG & THANH TOAN");
    if (head == NULL) {
        printf("(!) Cua hang trong.\n");
        pauseConsole();
        return;
    }
    int targetId, sl;
    printf("-> Nhap ID san pham can mua: ");
    scanf("%d", &targetId);
    Product *current = head;
    while (current != NULL) {
        if (current->id == targetId) {
            float rate = (current->soLuong < 5) ? 20.0 : 0.0;
            printf("[Tim thay] %s | Ton kho: %d | Gia: %.2f | Giam gia: %.0f%%\n", 
                    current->name, current->soLuong, current->price, rate);
            printf("-> Nhap so luong mua: ");
            scanf("%d", &sl);
            if (sl > 0 && sl <= current->soLuong) {
                float total = sl * current->price * (1 - rate / 100);
                current->soLuong -= sl;
                current->daban += sl;
                printf("\n\033[1;32m--- HOA DON CHIC FLOW ---\n");
                printf(" San pham : %s (x%d)\n", current->name, sl);
                printf(" TONG TIEN: %.2f VND\033[0m\n", total);
            } else {
                printf("\033[1;31m(!) So luong khong hop le hoac khong du hang.\033[0m\n");
            }
            clearBuffer();
            pauseConsole();
            return;
        }
        current = current->next;
    }
    printf("\033[1;31m(!) Khong tim thay ID %d.\033[0m\n", targetId);
    clearBuffer();
    pauseConsole();
}
void saveToFile(Product *head) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) return;
    Product *current = head;
    while (current != NULL) {
        // Luu theo format cua shop.c
        fprintf(f, "%d|%s|%.2f|%d|%.2f|%d\n", 
                current->id, current->name, current->price, 
                current->soLuong, current->giamgia, current->daban);
        current = current->next;
    }
    fclose(f);
    printf("\033[1;32m=> Du lieu da duoc dong bo vao tep %s.\033[0m\n", FILE_NAME);
    pauseConsole();
}
void loadFromFile(Product **head) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return; 
    Product temp;
    while (fscanf(f, "%d|%[^|]|%f|%d|%f|%d\n", 
           &temp.id, temp.name, &temp.price, &temp.soLuong, &temp.giamgia, &temp.daban) == 6) {
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
int main() {
    Product *storeData = NULL;
    loadFromFile(&storeData);
    while (1) {
        printHeader("HE THONG QUAN LY CHIC FLOW");
        printf("  [1] Them moi san pham thiet ke\n");
        printf("  [2] Xem danh sach cua hang\n");
        printf("  [3] Ban hang & Xuat hoa don\n");
        printf("  [4] Luu du lieu vao tep tin\n");
        printf("  [0] Thoat chuong trinh\n");
        int choice = getKeyboardChoice(4);
        if (choice == 0) break;
        switch (choice) {
            case 1: addProduct(&storeData); break;
            case 2: displayStoreProducts(storeData); break;
            case 3: sellProduct(storeData); break;
            case 4: saveToFile(storeData); break;
        }
    }
    freeList(storeData);
    printf("Chuong trinh ket thuc. Tam biet!\n");
    return 0;
}