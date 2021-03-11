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
	
	head = HesaplaFreq(metin);//yaz�daki harflerin kullan�m s�kl���na g�re linkli listenin olu�turulmas�
	head = InsertSort(head);//linkli listeyi olu�turan d���mlerin i�erdikleri harflerin say�s�na ba�l� olarak k���kten b�y��e s�ralanmas�
	head = HuffmanTree(head);//S�ralanm�� olan linkli listeden huffman a�ac�n�n olu�turulmas�
	printTree(head);//huffman a�ac�n�n her bir seviyesi bir sat�rda olacak �ekilde ekrana yazd�r�lmas�
	return 0;
}

NODE *createNode(){//yeni bir d���m olu�turma fonksiyonu
	
	NODE *p;
	p = (NODE *)malloc(sizeof(NODE));//d���m i�in haf�zada yer a��lmas�
	
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

NODE *HesaplaFreq( char metin[] ){//yaz�daki harflerin kullan�m s�kl���na g�re linkli liste olu�turan fonksiyon
	
	NODE *current, *temp1, *temp2;
	int i = 0;
	
	current = createNode();
	
	current->harf = metin[i];//yaz�daki ilk harf i�in d���m olu�turulmas�
	current->freq++;//ilgili harfin tutuldu�unda d���mde kullan�m say�s�n�n bir art�r�lmas�
	i++;//yaz�daki bir sonraki harfe ge�ilmesi
	
	while ( metin[i] != '\0' ){//yaz�daki t�m harflerin kontrol edilmesi i�in yaz�n�n sonundaki null karakterini g�rene kadar d�ng� devam eder
		
		temp1 = current;//linkli listede yaz�daki ilgili harfin bulunup bulunmad���n� kontrol etmek i�in ge�ici bir temp1 d���m� olu�turulur
		while ( (temp1->next != NULL) && (temp1->harf != metin[i]) )//linkli listede o harfin daha �nceden bulunup bulunmad���n�n kontrol�				
			temp1 = temp1->next; 
			
		if ( temp1->harf == metin[i] ){//E�er linkli listede bu harfi i�eren bir d���m varsa
				
			temp1->freq++;//�lgili d���mdeki harfin kullan�m say�s� bir art�t�l�r
		}
		else{//E�er bu harf ile ilk kez kar��la��l�yorsa
				
			temp2 = createNode();//bu harfin bilgisini tutmak i�in yeni bir d���m olu�turulur
			temp2->freq++;//harfin kullan�m say�s� bir art�l�r
			temp2->harf = metin[i];
			temp1->next = temp2;//yeni d���m linkli listenin sonuna eklenir
			temp2->prev = temp1;
		}
	
		i++;
	}
	return current;
}

NODE *InsertSort( NODE *head ){//Linki listedeki d���mlerin harf s�kl���na g�re insertion sortla k���kten b�y��e s�ralanmas�
	
	NODE *current, *temp;
	int tempfreq;
	char tempharf;
	
	for ( current = head->next; current != NULL; current = current->next ){//B�t�n linkli listenin head d���mden sonraki d���mden ba�lanarak dola��lmas�
		
		temp = current;//ilgili d���m�n ge�ici olarak saklanmas�
		tempfreq = temp->freq;//s�ras� de�i�ecek olan d���mdeki harfin kullan�m s�kl���n�n saklanmas�
		tempharf = temp->harf;//s�ras� de�i�ecek olan d���mdeki harfin saklanmas�
		
		while ( (temp->prev != NULL) && (tempfreq < temp->prev->freq) ){//ilgili d���mden �nceki d���mler dola��larak daha b�y�k olmayan bir d���m bulunana kadar d�ng� devam eder
			
			temp->freq = temp->prev->freq;//linkli listedeki temp d���m�ndeki kullan�m s�kl���n�n de�erine kendisinden bir �nceki d���m�n de�eri atan�r
			temp->harf = temp->prev->harf;//linkli listedeki temp d���mdeki harf kendisinden bir �nceki d���m�n harf de�eri ile de�i�tirilir
			temp = temp->prev;//linkli listede bir �nceki d���me ge�ilir 
		}
		temp->freq = tempfreq;//
		temp->harf = tempharf;
	}
	return head;
}

NODE *HuffmanTree( NODE *head ){//huffman a�ac�n� olu�turan fonksiyon
	
	NODE *current = head, *root;
	
	while ( current->next != NULL ){
		
		root = createNode();//a�ac�n k�k�n� olu�turacak root d���m�n�n olu�turulmas�
		root->freq = current->freq + current->next->freq;//linkli listenin ilk iki d���m�ndeki kullan�m s�kl��� toplan�r
		root->left = current;//k�k�n sol d���m� listedeki ilk d���m olarak atan�r
		root->right = current->next;//k�k�n sa� d���m� listedeki ikinci d���m olarak atan�r
		root->harf = '\0';//root d���m� herhangi bir harfi g�stermedi�i i�in harf de�eri null yap�l�r
		
		current = insert(current, root);//yeni olu�turulan root d���m�n�n linkli listeye s�ray� bozmayacak �ekilde eklenmesi
		
		current = current->next->next;//linkli listede a�a� yap�s�na kat�lan ilk iki d���m atlanarak sonraki d���me ge�ilir
		current->prev = NULL;
	
	}
	
	return current;
}

NODE *insert( NODE *current, NODE *root ){//yeni bir d���m�n linkli listeye s�ray� bozmayacak �ekilde eklenmesi
	
	NODE *prev, *curr;
	
	if ( current == NULL ){//E�er linkli listede herhangi bir d���m yoksa yeni d���m head olarak ayarlan�r
		
		current = root;
		current->next = NULL;
	}
	else{
		
		if ( root->freq < current->freq ){//E�er yeni eklenecek d���mdeki kullan�m s�kl��� linkli listedeki head d���m�ndeki kullan�m s�kl���ndan azsa root d���m� head olarak ayarlan�r
			
			root->next = current;//root d���m�n�n next'i linki listeyi g�sterecek �ekilde ayarlanmas�
			current = root;
		}
		else{//yeni d���m�n s�ray� bozmayacak �ekilde listeye eklenmesi
			
			prev = current;//linkli listedeki �nceki ve sonraki d���mlerin saklanmas�
			curr = current->next;//bu d���mler linkli listeye eklencek olan root d���m�nden �nceki ve sonraki d���mler olacakt�r
			
			while( (curr != NULL) && (curr->freq <= root->freq) ){//linki listenin gezilerek root d���m�n�n hangi iki d���m aras�nda olmas� gerekti�inin bulunmas�
				
				prev = curr;//linkli listede bir sonraki d���me ge�ilir
				curr = curr->next;
			}
			//root d���m� linkli listedeki prev ve curr d���mlerinin aras�na yerle�tirilir
			root->next = curr;//root d���m�n�n next'i curr olarak ayarlan�r
			prev->next = root;//prev d���m�n�n next'i root olarak ayarlan�r
		}
	}
	
	return current;
}

void printTree( NODE *head ){//huffman a�ac�n�n her bir seviyesi bir sat�rda olacak �ekilde yazd�r�lmas�
	
	NODE *tmp;
	int id, sayac, control, level = 0, sayacnull = 1, start, end, finish;
	
	while( sayacnull != (int)pow(2,(level-1)) ){//huffman a�ac�n�n sonuna ula��p ula��lmad���n�n kontrol�
		
		id = 0;//her seviyedeki d���mler 0'dan ba�lanarak numaraland�r�l�r
		sayacnull = 0;//a�ac�n sonuna ula��ld���n�n kontrol�n� sa�layan de�i�ken
		while( id < (int)pow(2,level) ){//huffman a�ac�n�n her seviyesinde 2^(a�ac seviyesi) kadar d���m bulunur
			
			tmp = head;
			sayac = 0;//k�kten ba�lay�p ekrana yazd�r�lacak d���me gidene kadar 
			start = 0;
			end = (int)pow(2,level);
			finish = 0;
			while( (sayac < level) && (finish == 0) ){
				
				control = ( start + end ) / 2;
				if ( id < control ){
					tmp = tmp->left;//sol d���me ge�ilir
					end = control;
				}
				else{
					tmp = tmp->right;//sa� d���me ge�ilir
					start = control;
				}	
				sayac++;
				if (tmp == NULL){//d���m null ise d�ng�den ��kmak i�im finish isimli kontrol de�i�keni 1 yap�l�r
		
					finish = 1;
				}
			}
			if ( tmp != NULL ){//huffman a�ac�nda sadece yaprak d���mleri harf i�erir. Bu y�zden left ve right d���mleri null'd�r
				
				if ((tmp->left == NULL) && (tmp->right == NULL)){//tmp yaprak d���m� ise i�erdi�i harf ile beraber kullan�m s�kl��� da ekrana yazd�r�l�r
		
				printf( "%d%c  ", tmp->freq, tmp->harf );
				}
				else{

					printf( "%d  ", tmp->freq );
				}
			}
			else{//yazd�r�lacak eleman null ise bo�luk olarak yazd�r�l�r 
				
				printf( "   " );
				sayacnull++;//null sayac� bir art�l�r
			}
			id++;//a�ac�n ilgili seviyesindeki bir sonraki d���me ge�ilir
		}
		level++;//A�ac�n bir sonraki seviyesine ge�ilir
		printf( "\n" );
	}
}

