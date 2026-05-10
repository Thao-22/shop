#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef CHIC_FLOW_H
#define CHIC_FLOW_H
#define MAX_STR 100
#define FILE_NAME "chic_flow_data.txt"
#endif
typedef struct Product {
    int id;
    char name[MAX_STR];
    float price;
    struct Product *next;
} Product;
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void displayMenu() {
    printf("\n========= CHIC FLOW MENU =========\n");
    printf("1. Them moi san pham (Thiet ke)\n");
    printf("2. Ban hang & Giam gia\n");
    printf("3. Luu du lieu vao tep tin\n");
    printf("4. Thoat chuong trinh\n");
    printf("==================================\n");
}
void sellProduct(Product *head) {
    if (head == NULL) {
        printf("(!) Cua hang trong. Vui long them san pham truoc.\n");
        return;
    }
    int targetId;
    float discount = 0;
    Product *current = head;
    printf("-> Nhap ID san pham can mua: ");
    scanf("%d", &targetId);
    while (current != NULL) {
        if (current->id == targetId) {
            printf("[Tim thay] %s - Gia goc: %.2f\n", current->name, current->price);
            printf("-> Nhap phan tram giam gia (%%): ");
            scanf("%f", &discount);
            float finalPrice = current->price * (1.0f - (discount / 100.0f));
            printf("\n--- HOA DON CHIC FLOW ---\n");
            printf(" San pham : %s\n", current->name);
            printf(" Giam gia : %.0f%%\n", discount);
            printf(" TONG TIEN: %.2f\n", finalPrice);
            printf("-------------------------\n");
            return;
        }
        current = current->next;
    }
    printf("(!) Khong tim thay ID %d trong he thong.\n", targetId);
}
void addProduct(Product **head) {
    Product *newNode = (Product *)malloc(sizeof(Product));
    if (newNode == NULL) return;
    printf("Nhap ID san pham: ");
    scanf("%d", &newNode->id);
    clearBuffer(); 
    printf("Nhap ten san pham: ");
    fgets(newNode->name, MAX_STR, stdin);
    newNode->name[strcspn(newNode->name, "\n")] = 0; 
    printf("Nhap gia niem yet: ");
    scanf("%f", &newNode->price);
    newNode->next = *head;
    *head = newNode;
    printf("=> Da them san pham vao kho.\n");
}
void saveToFile(Product *head) {
    FILE *f = fopen(FILE_NAME, "w");
    if (f == NULL) {
        printf("Loi: Khong the mo file de ghi!\n");
        return;
    }
    Product *current = head;
    while (current != NULL) {
        fprintf(f, "%d|%s|%.2f\n", current->id, current->name, current->price);
        current = current->next;
    }
    fclose(f);
    printf("=> Du lieu da duoc dong bo vao tep tin.\n");
}
void loadFromFile(Product **head) {
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) return; 
    while (1) {
        Product *newNode = (Product *)malloc(sizeof(Product));
        if (fscanf(f, "%d|%[^|]|%f\n", &newNode->id, newNode->name, &newNode->price) == 3) {
            newNode->next = *head;
            *head = newNode;
        } else {
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
int main() {
    Product *storeData = NULL;
    int choice;
    loadFromFile(&storeData);
    while (1) {
        displayMenu();
        printf("Lua chon cua ban: ");
        if (scanf("%d", &choice) != 1) {
            printf("(!) Loi: Vui long chi nhap con so.\n");
            clearBuffer();
            continue;
        }
        if(choice == 4) break;
        switch (choice) {
            case 1: addProduct(&storeData); break;
            case 2: sellProduct(storeData); break;
            case 3: saveToFile(storeData); break;
            default: printf("(!) Lua chon khong ton tai.\n");
        }
    }
    freeList(storeData);
    printf("Chuong trinh ket thuc. Tam biet!\n");
    return 0;
}