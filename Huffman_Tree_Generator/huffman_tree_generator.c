#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 1000

typedef struct node{
	
	char harf;
	int freq;
	struct node *right, *left, *next, *prev;
	
}NODE;

NODE *createNode(void);
NODE *HesaplaFreq(char *);
NODE *InsertSort(NODE *);
NODE *HuffmanTree(NODE *);
NODE *insert(NODE *, NODE *);
void printTree(NODE *);

int main(){
	
	char metin[MAX];
	NODE *head;
	
	head = createNode();
	
	printf( "Metni yaziniz: " );
	gets( metin );
	
	head = HesaplaFreq(metin);//yazýdaki harflerin kullaným sýklýðýna göre linkli listenin oluþturulmasý
	head = InsertSort(head);//linkli listeyi oluþturan düðümlerin içerdikleri harflerin sayýsýna baðlý olarak küçükten büyüðe sýralanmasý
	head = HuffmanTree(head);//Sýralanmýþ olan linkli listeden huffman aðacýnýn oluþturulmasý
	printTree(head);//huffman aðacýnýn her bir seviyesi bir satýrda olacak þekilde ekrana yazdýrýlmasý
	return 0;
}

NODE *createNode(){//yeni bir düðüm oluþturma fonksiyonu
	
	NODE *p;
	p = (NODE *)malloc(sizeof(NODE));//düðüm için hafýzada yer açýlmasý
	
	if ( p == NULL ){
		
		printf( "Allocation error\n" );
		exit(0);
	}
	
	p->freq = 0;
	p->right = NULL;
	p->left = NULL;
	p->next = NULL;
	p->prev = NULL;
	return p;
}

NODE *HesaplaFreq( char metin[] ){//yazýdaki harflerin kullaným sýklýðýna göre linkli liste oluþturan fonksiyon
	
	NODE *current, *temp1, *temp2;
	int i = 0;
	
	current = createNode();
	
	current->harf = metin[i];//yazýdaki ilk harf için düðüm oluþturulmasý
	current->freq++;//ilgili harfin tutulduðunda düðümde kullaným sayýsýnýn bir artýrýlmasý
	i++;//yazýdaki bir sonraki harfe geçilmesi
	
	while ( metin[i] != '\0' ){//yazýdaki tüm harflerin kontrol edilmesi için yazýnýn sonundaki null karakterini görene kadar döngü devam eder
		
		temp1 = current;//linkli listede yazýdaki ilgili harfin bulunup bulunmadýðýný kontrol etmek için geçici bir temp1 düðümü oluþturulur
		while ( (temp1->next != NULL) && (temp1->harf != metin[i]) )//linkli listede o harfin daha önceden bulunup bulunmadýðýnýn kontrolü				
			temp1 = temp1->next; 
			
		if ( temp1->harf == metin[i] ){//Eðer linkli listede bu harfi içeren bir düðüm varsa
				
			temp1->freq++;//Ýlgili düðümdeki harfin kullaným sayýsý bir artýtýlýr
		}
		else{//Eðer bu harf ile ilk kez karþýlaþýlýyorsa
				
			temp2 = createNode();//bu harfin bilgisini tutmak için yeni bir düðüm oluþturulur
			temp2->freq++;//harfin kullaným sayýsý bir artýlýr
			temp2->harf = metin[i];
			temp1->next = temp2;//yeni düðüm linkli listenin sonuna eklenir
			temp2->prev = temp1;
		}
	
		i++;
	}
	return current;
}

NODE *InsertSort( NODE *head ){//Linki listedeki düðümlerin harf sýklýðýna göre insertion sortla küçükten büyüðe sýralanmasý
	
	NODE *current, *temp;
	int tempfreq;
	char tempharf;
	
	for ( current = head->next; current != NULL; current = current->next ){//Bütün linkli listenin head düðümden sonraki düðümden baþlanarak dolaþýlmasý
		
		temp = current;//ilgili düðümün geçici olarak saklanmasý
		tempfreq = temp->freq;//sýrasý deðiþecek olan düðümdeki harfin kullaným sýklýðýnýn saklanmasý
		tempharf = temp->harf;//sýrasý deðiþecek olan düðümdeki harfin saklanmasý
		
		while ( (temp->prev != NULL) && (tempfreq < temp->prev->freq) ){//ilgili düðümden önceki düðümler dolaþýlarak daha büyük olmayan bir düðüm bulunana kadar döngü devam eder
			
			temp->freq = temp->prev->freq;//linkli listedeki temp düðümündeki kullaným sýklýðýnýn deðerine kendisinden bir önceki düðümün deðeri atanýr
			temp->harf = temp->prev->harf;//linkli listedeki temp düðümdeki harf kendisinden bir önceki düðümün harf deðeri ile deðiþtirilir
			temp = temp->prev;//linkli listede bir önceki düðüme geçilir 
		}
		temp->freq = tempfreq;//
		temp->harf = tempharf;
	}
	return head;
}

NODE *HuffmanTree( NODE *head ){//huffman aðacýný oluþturan fonksiyon
	
	NODE *current = head, *root;
	
	while ( current->next != NULL ){
		
		root = createNode();//aðacýn kökünü oluþturacak root düðümünün oluþturulmasý
		root->freq = current->freq + current->next->freq;//linkli listenin ilk iki düðümündeki kullaným sýklýðý toplanýr
		root->left = current;//kökün sol düðümü listedeki ilk düðüm olarak atanýr
		root->right = current->next;//kökün sað düðümü listedeki ikinci düðüm olarak atanýr
		root->harf = '\0';//root düðümü herhangi bir harfi göstermediði için harf deðeri null yapýlýr
		
		current = insert(current, root);//yeni oluþturulan root düðümünün linkli listeye sýrayý bozmayacak þekilde eklenmesi
		
		current = current->next->next;//linkli listede aðaç yapýsýna katýlan ilk iki düðüm atlanarak sonraki düðüme geçilir
		current->prev = NULL;
	
	}
	
	return current;
}

NODE *insert( NODE *current, NODE *root ){//yeni bir düðümün linkli listeye sýrayý bozmayacak þekilde eklenmesi
	
	NODE *prev, *curr;
	
	if ( current == NULL ){//Eðer linkli listede herhangi bir düðüm yoksa yeni düðüm head olarak ayarlanýr
		
		current = root;
		current->next = NULL;
	}
	else{
		
		if ( root->freq < current->freq ){//Eðer yeni eklenecek düðümdeki kullaným sýklýðý linkli listedeki head düðümündeki kullaným sýklýðýndan azsa root düðümü head olarak ayarlanýr
			
			root->next = current;//root düðümünün next'i linki listeyi gösterecek þekilde ayarlanmasý
			current = root;
		}
		else{//yeni düðümün sýrayý bozmayacak þekilde listeye eklenmesi
			
			prev = current;//linkli listedeki önceki ve sonraki düðümlerin saklanmasý
			curr = current->next;//bu düðümler linkli listeye eklencek olan root düðümünden önceki ve sonraki düðümler olacaktýr
			
			while( (curr != NULL) && (curr->freq <= root->freq) ){//linki listenin gezilerek root düðümünün hangi iki düðüm arasýnda olmasý gerektiðinin bulunmasý
				
				prev = curr;//linkli listede bir sonraki düðüme geçilir
				curr = curr->next;
			}
			//root düðümü linkli listedeki prev ve curr düðümlerinin arasýna yerleþtirilir
			root->next = curr;//root düðümünün next'i curr olarak ayarlanýr
			prev->next = root;//prev düðümünün next'i root olarak ayarlanýr
		}
	}
	
	return current;
}

void printTree( NODE *head ){//huffman aðacýnýn her bir seviyesi bir satýrda olacak þekilde yazdýrýlmasý
	
	NODE *tmp;
	int id, sayac, control, level = 0, sayacnull = 1, start, end, finish;
	
	while( sayacnull != (int)pow(2,(level-1)) ){//huffman aðacýnýn sonuna ulaþýp ulaþýlmadýðýnýn kontrolü
		
		id = 0;//her seviyedeki düðümler 0'dan baþlanarak numaralandýrýlýr
		sayacnull = 0;//aðacýn sonuna ulaþýldýðýnýn kontrolünü saðlayan deðiþken
		while( id < (int)pow(2,level) ){//huffman aðacýnýn her seviyesinde 2^(aðac seviyesi) kadar düðüm bulunur
			
			tmp = head;
			sayac = 0;//kökten baþlayýp ekrana yazdýrýlacak düðüme gidene kadar 
			start = 0;
			end = (int)pow(2,level);
			finish = 0;
			while( (sayac < level) && (finish == 0) ){
				
				control = ( start + end ) / 2;
				if ( id < control ){
					tmp = tmp->left;//sol düðüme geçilir
					end = control;
				}
				else{
					tmp = tmp->right;//sað düðüme geçilir
					start = control;
				}	
				sayac++;
				if (tmp == NULL){//düðüm null ise döngüden çýkmak içim finish isimli kontrol deðiþkeni 1 yapýlýr
		
					finish = 1;
				}
			}
			if ( tmp != NULL ){//huffman aðacýnda sadece yaprak düðümleri harf içerir. Bu yüzden left ve right düðümleri null'dýr
				
				if ((tmp->left == NULL) && (tmp->right == NULL)){//tmp yaprak düðümü ise içerdiði harf ile beraber kullaným sýklýðý da ekrana yazdýrýlýr
		
				printf( "%d%c  ", tmp->freq, tmp->harf );
				}
				else{

					printf( "%d  ", tmp->freq );
				}
			}
			else{//yazdýrýlacak eleman null ise boþluk olarak yazdýrýlýr 
				
				printf( "   " );
				sayacnull++;//null sayacý bir artýlýr
			}
			id++;//aðacýn ilgili seviyesindeki bir sonraki düðüme geçilir
		}
		level++;//Aðacýn bir sonraki seviyesine geçilir
		printf( "\n" );
	}
}

