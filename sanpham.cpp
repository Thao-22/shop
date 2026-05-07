#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 100
#define FILE_KHOHANG "khohang.txt"

typedef struct {
   char Tensp[100];
   char Size[10];
   int id;
   int soLuong;
   float Gia;
   int daban;
   float giamgia;
} Sanpham;

Sanpham ds[MAX];
int n=0;

int nhapSoLuong(){
    int x;
    char c;

    while(1){
        printf("-Nhap so luong cua san pham: ");

        if(scanf("%d", &x) == 1){
            if((c = getchar()) != '\n'){
                while(getchar() != '\n'); 
                printf("Khong hop le! Chi duoc nhap so.\n");
                continue;
            }

            if(x > 0){
                return x;
            }else{
                printf("So luong phai > 0!\n");
            }
        }else{
            printf("Khong hop le! Phai nhap so.\n");
            while(getchar() != '\n');
        }
    }
}

void nhapHang(){
	int m;
	printf("Nhap so luong san pham cua cua hang:");
	scanf("%d",&m);
	getchar();
	for(int i=0;i<m;i++){
		char ten[100],size[10];
		int sl, idMoi;
		float gia;
	printf("\n==San Pham %d==\n",i+1);
	
    printf("- Nhap ID cho san pham: ");
    scanf("%d", &idMoi);
    getchar(); 

	printf("-Nhap ten san pham:");
	fgets(ten,sizeof(ten),stdin);
	ten[strcspn(ten,"\n")]=0;
	
	printf("-Nhap size(S,M,L,XL,..):");
    fgets(size,sizeof(size),stdin);
	size[strcspn(size,"\n")]=0;
	
	sl=nhapSoLuong();
	
	int found=-1;
	for(int j=0;j<n;j++){
		if(ds[j].id == idMoi || strcmp(ds[j].Tensp,ten)== 0 && strcmp(ds[j].Size,size)== 0){
		    found=j;
		    break;
		}
	}
	if(found!=-1){
	   ds[found].soLuong+=sl;
	   printf("\nTrung ten sp va size -> da cong don");
	}else{
		printf("-Nhap gia cua san pham:");
        scanf("%f",&gia);
	    getchar();
	    
	    ds[n].id = idMoi; 
	    strcpy(ds[n].Tensp,ten);
	    strcpy(ds[n].Size,size);
	    ds[n].Gia=gia;
	    ds[n].soLuong=sl;
	    ds[n].daban=0;
	    ds[n].giamgia=0;
	    
	    n++;
        printf("\033[1;32mDa them san pham moi voi ID %d thanh cong!\033[0m\n", idMoi);
	}
}
}


void show(){
	printf("%*s%s\n", (80 - strlen("== DANH SACH SAN PHAM ==")) / 2, "","==Danh Sach San Pham==");
	printf("%-20s %-6s %-10s %-10s %-10s %-10s","Ten san pham","Size","So Luong","Gia","Giam gia","Da ban");
	for(int i=0;i<n;i++){
		printf("\n%-20s %-6s %-10d %-10.0f %-10.0f %-10d\n",
		ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia, ds[i].giamgia, ds[i].daban);
}
}

void discount(){
	for(int i=0;i<n;i++){
	if(ds[i].soLuong<5 && ds[i].soLuong>0){
		ds[i].giamgia=20;
	}else{
		ds[i].giamgia=0;	
	}
	}
	printf("Gia da cap nhat\n");
}

void kiemTra(){
   	for(int i=0;i<n;i++){
	    if(ds[i].soLuong<2){
		    int boSung;
	     	printf("\nSan pham %s sap het hang",ds[i].Tensp);
		    while(1){
				printf("\nNhap so luong can bo sung: ");
			
				if(scanf("%d", &boSung) == 1 && boSung > 0){
					break;
				}else{
					printf("Khong hop le ! Nhap lai\n");
					while(getchar() != '\n');
				}
			}
	        ds[i].soLuong+=boSung;
	        printf("\nDa bo sung thanh cong ! So luong cua %s sau khi bo sung la: %d",ds[i].Tensp, ds[i].soLuong);
	}
}  
   	printf("\nKhong co hang can bo sung");
}

void saveKhoToFile() {
    FILE *f = fopen(FILE_KHOHANG, "w");
    if(!f) return;
    int i;
    for(i=0; i<n; i++)
        fprintf(f, "%s|%s|%d|%.2f|%d|%.2f\n", ds[i].Tensp, ds[i].Size, ds[i].soLuong, ds[i].Gia, ds[i].daban, ds[i].giamgia);
    fclose(f);
}

void loadKhoFromFile() {
    FILE *f = fopen(FILE_KHOHANG, "r");
    if(!f) return;
    n = 0;
    while(fscanf(f, "%[^|]|%[^|]|%d|%f|%d|%f\n", ds[n].Tensp, ds[n].Size, &ds[n].soLuong, &ds[n].Gia, &ds[n].daban, &ds[n].giamgia) == 6) {
        n++;
    }
    fclose(f);
}

int main(){
	int choice;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Nhap san pham\n");
        printf("2. Hien thi\n");
        printf("3. Kiem tra ton kho\n");
        printf("0. Thoat\n");
        printf("4. Luu du lieu vao file\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: nhapHang(); break;
            case 2: show(); break;
            case 3: kiemTra(); break;
            case 4: saveKhoToFile(); break;
        }
    } while (choice != 0);

    return 0;
}
