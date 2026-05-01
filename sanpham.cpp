#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 100

typedef struct {
   char Tensp[100];
   char Size[10];
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
		int sl;
		float gia;
	printf("\n==San Pham %d==\n",i+1);
	
	printf("-Nhap ten san pham:");
	fgets(ten,sizeof(ten),stdin);
	ten[strcspn(ten,"\n")]=0;
	
	printf("-Nhap size(S,M,L,XL,..):");
    fgets(size,sizeof(size),stdin);
	size[strcspn(size,"\n")]=0;
	
	sl=nhapSoLuong();
	
	int found=-1;
	for(int j=0;j<n;j++){
		if(strcmp(ds[j].Tensp,ten)== 0 && strcmp(ds[j].Size,size)== 0){
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
	
	    strcpy(ds[n].Tensp,ten);
	    strcpy(ds[n].Size,size);
	    ds[n].Gia=gia;
	    ds[n].soLuong=sl;
	    ds[n].daban=0;
	    ds[n].giamgia=0;
	    
	    n++;
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


void sell(){
	char ten[100], size[10];
	int sl;
	printf("Ten san pham:");
	fgets(ten,sizeof(ten),stdin);
	ten[strcspn(ten,"\n")]=0;
	
	printf("Size do:");
	fgets(size,sizeof(size),stdin);
	size[strcspn(size,"\n")]=0;
	
	sl=nhapSoLuong();
	
	int found=0;
	for(int i=0;i<n;i++){
		if(strcmp(ds[i].Tensp,ten) ==0 && strcmp(ds[i].Size,size)== 0 ){
			found=1;
			
            if(ds[i].soLuong >= sl){
            	discount();
            	float giasauGiam = sl * ds[i].Gia * (1-ds[i].giamgia/100);
            	 
	            ds[i].soLuong-=sl;
		        ds[i].daban+=sl;
		        
		        printf("Ban thanh cong voi gia :%.2f\n",giasauGiam);
		    }else{
		    	printf("Hang trong kho khong du !\n");
			}
			break;
		}
	}
	if(found==0){
		printf("Khong tim thay san pham nay !");	
	}
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


void favProducts(){
    int max=-1;
    int index=-1;
        for(int i=0;i<n;i++){
     	    if(ds[i].daban > max){
     		    max=ds[i].daban;
     		    index=i;
     	    }
     	}
     	if(index!=-1){
     		printf("Cac san pham ban chay nhat:\n ");
     		for(int i=0;i<n;i++){
     			if(ds[i].daban == max){
     				printf("%s da ban %d \n",ds[i].Tensp, ds[i].daban);
     			}
     		}
     	}else{
     		printf("Khong tim thay du lieu ban hang !\n");
		}
}


int main(){
	int choice;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Nhap san pham\n");
        printf("2. Hien thi\n");
        printf("3. Ban hang\n");
        printf("4. Kiem tra ton kho\n");
        printf("5. Kiem tra san pham ban chay nhat\n");
        printf("0. Thoat\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: nhapHang(); break;
            case 2: show(); break;
            case 3: sell(); break;
            case 4: kiemTra(); break;
            case 5: favProducts(); break;
        }
    } while (choice != 0);

    return 0;
}
