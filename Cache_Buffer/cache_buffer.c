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
	
	if (control){//Consoledan ya da Dosyadan istek alýnýp alýnmayacaðýnýn kontrolü
		
		printf( "Sayac sinirini giriniz: " );
		scanf( "%d", &T );
	
		printf( "Cache Bufferin kapasitesini giriniz: " );
		scanf( "%d", &L );
	}
	else{//Dosyadan istek alýnýrsa
		
		fgets( &c[0], MAX, fs );//Sayacýn eþik deðerinin dosyadan alýnmasý
		fgets( &c[1], MAX, fs );//Bufferýn boyutunun dosyadan alýnmasý
		
		T = c[0] - '0';
		L = c[1] - '0';
	}
	
	head = createNode();//Ýlk düðümün oluþturulmasý
	head->sayac = 1;
	head->prev = NULL;
	head->next = NULL;;
	
	if (control){
		
		scanf( "%s", temp );
	}
	else{
		
		fscanf( fs, "%s", temp );
	}
	
	strcpy( head->adr, temp );//Sayfanýn adresinin düðüme atýlmasý
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
		
		while ( ( strcmp( tempnode->adr, temp ) ) && ( tempnode->next != NULL ) ){//Girilen sayfanýn linkli listede olup olmadýðýnýn kontrolü
			
			tempnode = tempnode->next;
		}

		if ( !strcmp( tempnode->adr, temp ) ){//Eðer girilen sayfa linkli listede varsa
			
			(tempnode->sayac)++;//sayacý bir attýr
		
			if ( tempnode->sayac > T ){//Sayac eþik deðerini geçtiyse
				
				insertFront1( &head, &tempnode );//head node olarak atanmasý
			}		
		}
		else {//Girilen sayfa linkli listede yoksa
		
			maxsize++;//Düðüm sayýný bir attýr
			tempnode1 = createNode();//Girilen sayfa için düðüm oluþturulmasý
			if( maxsize > L ){//Düðüm sayýsý kapasiteyi aþarsa
				
				deleteEndNode( head );//Linkli listenin en son düðümünün silinmesi	
				maxsize--;//Bir düðüm silindiði için düðüm sayýsýnýn bir azaltýlmasý
			}
			
			strcpy( tempnode1->adr, temp );//Düðümün sayfanýn adresini tutmasý
			tempnode1->sayac = 1; 
			insertFront2( &head, &tempnode1 );//Yeni girilen sayfanýn düðüm olarak head node þeklinde öne alýnmasý
		}
		printlist(head);//Linkli listenin ekranda yazdýrýlmasý
		if (control){
		
			scanf( "%s", temp );
		}
		else{
		
			fscanf( fs, "%s", temp );
		}
	}
	
	printf( "Cache Buffer temizlensin mi?( evet ya da hayir? )\n" );
	scanf( "%s", &a );
	
	if ( !strcmp( a, "evet" ) )//Cache Bufferýn temizlenmesi
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
	p->adr = (char*)malloc(MAX*sizeof(char));//Düðümdeki adres üzerinden sayfa ismine eriþileceði için malloc() fonksiyonu ile yer açýlmasý
	return p;
}

void insertFront1( BUFFER **head, BUFFER **tempnode  ){
	
	if ( (*tempnode) == (*head) ){//Eðer sayacý eþik deðerini aþan düðüm head ise hiç bir iþlem yapma 
		
		;
	}
	else if ( (*tempnode)->next == NULL ){//Eðer sayacý eþik deðerini aþan düðüm Linkli listedeki son düðüm ise
		
		(*tempnode)->prev->next = NULL;//Sondaki düðüm en baþa ekleneceði için önceki düðüm NULL gösterir
		(*head)->prev = *tempnode;//Listenin en baþýna eklenecek düðüm en baþa ekleneceði için head düðümünü previ yeni düðüm olarak atanýr
		(*tempnode)->prev = NULL;//Listenin en baþýna eklenecek düðüm en baþa ekleneceði için prev NULL yapýlýr
		(*tempnode)->next = *head;//Listenin en baþýna eklenecek düðümün head düðümünü göstermesi
		*head = *tempnode;//Listenin en baþýna eklenen düðüm head düðümüne atanýr
	}
	else{//Eðer sayacý eþik deðerini aþan düðüm linkli listede baþta ve sonda deðilse
		
		(*tempnode)->prev->next = (*tempnode)->next;//Listenin en baþýna eklenecek düðümden önceki düðümden önceki ve sonraki düðümün birbirlerini göstermesi
		(*tempnode)->next->prev = (*tempnode)->prev;
		(*head)->prev = *tempnode;//Listenin en baþýna eklenecek düðüm en baþa ekleneceði için head düðümünü previ yeni düðüm olarak atanýr
		(*tempnode)->prev = NULL;//Listenin en baþýna eklenecek düðüm en baþa ekleneceði için prev NULL yapýlýr
		(*tempnode)->next = *head;//Listenin en baþýna eklenecek düðümün head düðümünü göstermesi
		*head = *tempnode;//Listenin en baþýna eklenen düðüm head düðümüne atanýr
	}	
}

void insertFront2( BUFFER **head, BUFFER **tempnode1 ){//Linkli listede olmayan sayfanýn düðüm olarak head node yapýlmasý
	
	(*tempnode1)->prev = NULL;//Eklenecek yeni düðüm en baþa ekleneceði için prev NULL yapýlýr
	(*tempnode1)->next = *head;//Eklenecek yeni düðümün head düðümünü göstermesi
	(*head)->prev = *tempnode1;//Eklenecek yeni düðüm en baþa ekleneceði için head düðümünü previ yeni düðüm olarak atanýr
	*head = *tempnode1;//Eklenen yeni düðüm head düðümüne atanýr
	
}

void deleteEndNode( BUFFER *head ){//Kapasite aþýldýðý zaman son düðümün silinmesi
	
	BUFFER *current = head;
	
	while( current->next != NULL )//Son düðüme ulaþýncaya kadar linkli listenin gezilmesi
		current = current->next;
	
	
	current->prev->next = NULL;//Son düðümden öncekinin NULL göstermesi
	
	free(current);//Son düðümüm silinmesi
	
}

void printlist( BUFFER *head ){//Linkli listenin ekrana yazdýrýlmasý
	
	BUFFER *p;
	
	for ( p = head; p->next != NULL; p = p->next )
		printf( "(%s,%d) - ", p->adr, p->sayac );
	
	printf( "(%s,%d)", p->adr, p->sayac );
	printf( "\n" );
}

void freeNode( BUFFER **head ){//Cache Bufferýn temizlenmesi
	
	BUFFER *current;
	BUFFER *temp;
	
	for( current = *head; current->next != NULL; ){//Linkli listenin silinmesi
		
		printlist(current);
		temp = current->next;//Silinecek düðümün gösterdiði sonraki düðümün kaybolmamasý için geçici olarak temp düðümüne atanmasý
		free(current);//Ýlgili düðümün silinmesi
		current = temp;//Saklanmýþ olan sonraki düðümün current düðümüne atanmasý
	}
	printlist(current);
	*head = NULL;
}

