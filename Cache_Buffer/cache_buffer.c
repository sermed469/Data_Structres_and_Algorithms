#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct cachebuffer{
	
	char *adr;
	int sayac;
	struct cachebuffer *prev;
	struct cachebuffer *next;	
	
}BUFFER;

BUFFER *createNode(void);
void deleteEndNode( BUFFER *);
void insertFront1( BUFFER **, BUFFER ** );
void insertFront2( BUFFER **, BUFFER ** );
void printlist( BUFFER * );
void freeNode( BUFFER ** );

int main(){
	
	BUFFER *head, *tempnode, *tempnode1;
	int T, L, maxsize = 0, control;
	char temp[MAX], c[2], a[6];
	FILE *fs = fopen( "input.txt", "r" );
	
	printf( "Dosya uzerinden mi giris yapilacak yoksa console uzerinden mi?(Dosya icin 0'e console icin 1'ye basin)" );
	scanf( "%d", &control );
	
	if (control){//Consoledan ya da Dosyadan istek al�n�p al�nmayaca��n�n kontrol�
		
		printf( "Sayac sinirini giriniz: " );
		scanf( "%d", &T );
	
		printf( "Cache Bufferin kapasitesini giriniz: " );
		scanf( "%d", &L );
	}
	else{//Dosyadan istek al�n�rsa
		
		fgets( &c[0], MAX, fs );//Sayac�n e�ik de�erinin dosyadan al�nmas�
		fgets( &c[1], MAX, fs );//Buffer�n boyutunun dosyadan al�nmas�
		
		T = c[0] - '0';
		L = c[1] - '0';
	}
	
	head = createNode();//�lk d���m�n olu�turulmas�
	head->sayac = 1;
	head->prev = NULL;
	head->next = NULL;;
	
	if (control){
		
		scanf( "%s", temp );
	}
	else{
		
		fscanf( fs, "%s", temp );
	}
	
	strcpy( head->adr, temp );//Sayfan�n adresinin d���me at�lmas�
	maxsize++;
	printlist(head);
	
	if (control){
		
		scanf( "%s", temp );
	}
	else{
		
		fscanf( fs, "%s", temp );
	}
	
	while( (strcmp( temp, "end" )) && (!feof(fs)) ){
		
		tempnode = head;
		
		while ( ( strcmp( tempnode->adr, temp ) ) && ( tempnode->next != NULL ) ){//Girilen sayfan�n linkli listede olup olmad���n�n kontrol�
			
			tempnode = tempnode->next;
		}

		if ( !strcmp( tempnode->adr, temp ) ){//E�er girilen sayfa linkli listede varsa
			
			(tempnode->sayac)++;//sayac� bir att�r
		
			if ( tempnode->sayac > T ){//Sayac e�ik de�erini ge�tiyse
				
				insertFront1( &head, &tempnode );//head node olarak atanmas�
			}		
		}
		else {//Girilen sayfa linkli listede yoksa
		
			maxsize++;//D���m say�n� bir att�r
			tempnode1 = createNode();//Girilen sayfa i�in d���m olu�turulmas�
			if( maxsize > L ){//D���m say�s� kapasiteyi a�arsa
				
				deleteEndNode( head );//Linkli listenin en son d���m�n�n silinmesi	
				maxsize--;//Bir d���m silindi�i i�in d���m say�s�n�n bir azalt�lmas�
			}
			
			strcpy( tempnode1->adr, temp );//D���m�n sayfan�n adresini tutmas�
			tempnode1->sayac = 1; 
			insertFront2( &head, &tempnode1 );//Yeni girilen sayfan�n d���m olarak head node �eklinde �ne al�nmas�
		}
		printlist(head);//Linkli listenin ekranda yazd�r�lmas�
		if (control){
		
			scanf( "%s", temp );
		}
		else{
		
			fscanf( fs, "%s", temp );
		}
	}
	
	printf( "Cache Buffer temizlensin mi?( evet ya da hayir? )\n" );
	scanf( "%s", &a );
	
	if ( !strcmp( a, "evet" ) )//Cache Buffer�n temizlenmesi
		freeNode(&head);
	if( head == NULL )
		printf( "Cache temizlendi" );
	else
		printlist(head);
	return 0;
}

BUFFER *createNode(){
	
	BUFFER *p;
	p = (BUFFER *)malloc(sizeof(BUFFER));
	
	if ( p == NULL ){
		
		printf( "Allocation error\n" );
		exit(0);
	}
	p->adr = (char*)malloc(MAX*sizeof(char));//D���mdeki adres �zerinden sayfa ismine eri�ilece�i i�in malloc() fonksiyonu ile yer a��lmas�
	return p;
}

void insertFront1( BUFFER **head, BUFFER **tempnode  ){
	
	if ( (*tempnode) == (*head) ){//E�er sayac� e�ik de�erini a�an d���m head ise hi� bir i�lem yapma 
		
		;
	}
	else if ( (*tempnode)->next == NULL ){//E�er sayac� e�ik de�erini a�an d���m Linkli listedeki son d���m ise
		
		(*tempnode)->prev->next = NULL;//Sondaki d���m en ba�a eklenece�i i�in �nceki d���m NULL g�sterir
		(*head)->prev = *tempnode;//Listenin en ba��na eklenecek d���m en ba�a eklenece�i i�in head d���m�n� previ yeni d���m olarak atan�r
		(*tempnode)->prev = NULL;//Listenin en ba��na eklenecek d���m en ba�a eklenece�i i�in prev NULL yap�l�r
		(*tempnode)->next = *head;//Listenin en ba��na eklenecek d���m�n head d���m�n� g�stermesi
		*head = *tempnode;//Listenin en ba��na eklenen d���m head d���m�ne atan�r
	}
	else{//E�er sayac� e�ik de�erini a�an d���m linkli listede ba�ta ve sonda de�ilse
		
		(*tempnode)->prev->next = (*tempnode)->next;//Listenin en ba��na eklenecek d���mden �nceki d���mden �nceki ve sonraki d���m�n birbirlerini g�stermesi
		(*tempnode)->next->prev = (*tempnode)->prev;
		(*head)->prev = *tempnode;//Listenin en ba��na eklenecek d���m en ba�a eklenece�i i�in head d���m�n� previ yeni d���m olarak atan�r
		(*tempnode)->prev = NULL;//Listenin en ba��na eklenecek d���m en ba�a eklenece�i i�in prev NULL yap�l�r
		(*tempnode)->next = *head;//Listenin en ba��na eklenecek d���m�n head d���m�n� g�stermesi
		*head = *tempnode;//Listenin en ba��na eklenen d���m head d���m�ne atan�r
	}	
}

void insertFront2( BUFFER **head, BUFFER **tempnode1 ){//Linkli listede olmayan sayfan�n d���m olarak head node yap�lmas�
	
	(*tempnode1)->prev = NULL;//Eklenecek yeni d���m en ba�a eklenece�i i�in prev NULL yap�l�r
	(*tempnode1)->next = *head;//Eklenecek yeni d���m�n head d���m�n� g�stermesi
	(*head)->prev = *tempnode1;//Eklenecek yeni d���m en ba�a eklenece�i i�in head d���m�n� previ yeni d���m olarak atan�r
	*head = *tempnode1;//Eklenen yeni d���m head d���m�ne atan�r
	
}

void deleteEndNode( BUFFER *head ){//Kapasite a��ld��� zaman son d���m�n silinmesi
	
	BUFFER *current = head;
	
	while( current->next != NULL )//Son d���me ula��ncaya kadar linkli listenin gezilmesi
		current = current->next;
	
	
	current->prev->next = NULL;//Son d���mden �ncekinin NULL g�stermesi
	
	free(current);//Son d���m�m silinmesi
	
}

void printlist( BUFFER *head ){//Linkli listenin ekrana yazd�r�lmas�
	
	BUFFER *p;
	
	for ( p = head; p->next != NULL; p = p->next )
		printf( "(%s,%d) - ", p->adr, p->sayac );
	
	printf( "(%s,%d)", p->adr, p->sayac );
	printf( "\n" );
}

void freeNode( BUFFER **head ){//Cache Buffer�n temizlenmesi
	
	BUFFER *current;
	BUFFER *temp;
	
	for( current = *head; current->next != NULL; ){//Linkli listenin silinmesi
		
		printlist(current);
		temp = current->next;//Silinecek d���m�n g�sterdi�i sonraki d���m�n kaybolmamas� i�in ge�ici olarak temp d���m�ne atanmas�
		free(current);//�lgili d���m�n silinmesi
		current = temp;//Saklanm�� olan sonraki d���m�n current d���m�ne atanmas�
	}
	printlist(current);
	*head = NULL;
}

