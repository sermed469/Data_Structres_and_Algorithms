#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 10000

int Find( char*, char*, char*, int, int* );//Metinden ��kar�lacak kelimeyi Boyer-Moore Horspool Algoritmas�na g�re arayan fonksiyon
void Replace( char*, char*, char*, int );//Metinden eski kelimeyi ��kart�p yeni kelimeyi yerle�tiren fonksiyon
void shiftTable( char*, int*, int );//pattern'in ne kadar kayd�r�l�ca��n� belirleyen tabloyu olu�turan fonksiyon

int main(){

	struct timespec start, end;
	int i = 0, sayac, k;
	char find[20], replace[20], Text[MAX];
	const char filename[20];
	int table[200];
	FILE *fs;
	
	printf( "Degistirilecek kelimeyi giriniz: " );
	gets(find);
	printf( "Yeni kelimeyi giriniz: " );
	gets(replace);
	printf("Aramada case sensitive ozelligi olacak mi?(Evet ise 1'e degilse 0'a basiniz)");
	scanf("%d", &k );
	printf("Dosya adini giriniz(.txt uzantisi ile beraber) ");
	scanf( "%s", filename );

	fs = fopen( filename, "r" );
	fread(Text,sizeof(char),MAX,fs);//Dosyadaki metnin okunarak Text[] dizisine atanmas�
	fclose(fs);
	
	printf( "\nMetin:" );
	puts(Text);
	
	shiftTable(find,table,k);//Aranacak kelimedeki harflerin kelimede bulundu�u indise g�re tablo olu�turan fonksiyon
	
    clock_gettime(CLOCK_MONOTONIC, &start);
	sayac = Find(find,Text,replace,k,table);//Boyer-Moore Horspool Algoritmas� kullan�larak metinden ��kar�lacak kelimeyi bulan ve ka� de�i�im oldu�unu d�nd�ren fonksiyon
	clock_gettime(CLOCK_MONOTONIC, &end);
	
	printf( "\nYeni Metin: " );
	puts(Text);
	
	fs = fopen( filename, "w" );
	fprintf(fs, "%s", Text);//Find and Replace i�lemleri yap�ld�ktan sonra yeni olu�an metnin yine ayn� dosyaya yazd�r�lmas�
	fclose(fs);
	
	printf( "\nDegisim Sayisi: %d\n", sayac );//Ka� kez de�i�im yap�ld���n�n ekrana yazd�r�lmas�
	printf( "Gecen sure: %lf milisaniye\n", (double)((long int)(end.tv_sec-start.tv_sec)*1000000000 + (end.tv_nsec-start.tv_nsec)) / 1000000 );//Find and Replace i�lemininin �al��ma s�resinin ekrana yazd�r�lmas�
	return 0;	
}

int Find( char find[], char Text[], char replace[], int k, int table[] ){//Boyer-Moore Horspool Algoritmas� kullan�larak metinden ��kar�lacak kelimenin bulunmas�
	
	int N = strlen(Text), M = strlen(find), i = M-1, j, count = 0, shift = 0;
	
	while ( i  < strlen(Text) ){//Text[] dizisinin sonuna gelen kadar d�ng� devam eder
	
		j = M-1;
		
		while ( (j >= 0) && ( (find[j] == Text[i]) || ( !k && (fabs(find[j]-Text[i]) == fabs('A'-'a') ) ) ) ){//aranan kelimedeki harf ile metindeki ilgili harf ayn� oldu�u s�rece d�ng� devam eder
		//k de�eri e�er 0 ise yani case sensitive de�ilse or i�leminin sa� k�sm� aktif hale gelir. 
			j--;//aranan kelimedeki b�r �nceki harfe ge�ilmesi
			i--;//Metindeki bir �nceki harfe ge�ilmesi
		}
			
		if ( j == M - 1 ){//E�er aranan kelimenin son harfi metindeki harf ile e�le�mediyse

			shift = table[Text[i] - ' '];//metinde kontrol edilen harfin tablodaki de�eri kadar shift de�eri art�r�l�r
		}
		else if ( (j < M - 1) && (j >= 0) ){//E�er aranan kelimenin bir k�sm� e�le�tiyse
			
			if ( table[Text[i] - ' '] == M)//E�le�meyen harf aranan kelimede yoksa
				shift = M;//aranan kelimenin uzunlu�u kadar kayd�r
			else//E�le�meyen harf aranan kelimede varsa
				shift = table[find[M-1] - ' '];//kelimenin son harfinin tablodaki de�eri kadar kayd�r	
			
			shift += M - 1 - j;//kontrol s�ras�nda azalan i de�erinin eski haline getirilmesi
		}
		else{//E�er aranan kelime bulunduysa
		
			shift = M;//Kontrol s�ras�nda i indisi arananan kelimenin uzunlu�u kadar azald��� i�im M kadar art�r�l�r
			Replace(Text,find,replace,i+shift+1);//Yer de�i�tirme fonksiyonu �a�r�l�r	
			count++;//De�i�im say�s� bir art�r�l�r
			shift += strlen(replace) - strlen(find);//Eklenen ve ��kar�lan kelimelerin uzunluklar�na ba�l� olarak kayma miktar�n�n hesaplanmas�
			shift += M;
		}
		
		i += shift;
	}
	
	return count;
}

void shiftTable( char find[], int table[], int k ){//Bo�luk kaarakterinden ba�lay�p 'z' karakterine kadar her bir harf i�in pattern kelimesinin kayd�r�lma miktar�n�n heasplanmas� 
	
	int i, M = strlen(find), size = 'z'-' '+1;//Olu�turulucak tabloda bo�luk karakteri offset de�eri olarak ayarlan�r
	
	for ( i = 0; i < size; i++ )//�ncelikle t�m karakterler i�in aranan kelimenin uzunlu�u kadar de�er atan�r
		table[i] = M;
 	
 	for ( i = 0; i < M-1; i++ ){//Aranan kelimedeki harflerin tablodaki de�erlerinin kelimede bulunduklar� indise g�re ayarlanmas�
 		
		table[find[i] - ' '] = M - i - 1;
		
		if ( k == 0 ){//E�er case sensitive �zelli�i devre d��� ise aranan kelimedeki harfin kendisiyle denk olan b�y�k ya da k���k harfin de tablodaki de�eri almas�
			
			if ( find[i] > 'Z' )//E�er k���k harf ise
				table['A' - 'a' + find[i] - ' '] = M - i - 1;
			else//E�er b�y�k harf ise
				table['a' - 'A' + find[i] - ' '] = M - i - 1;
		}
	 }
}

void Replace( char Text[], char find[], char replace[], int x ){//Find() fonksiyonu ile bulunan kelimenin yeni kelime ile de�i�tirilmesi
	
	int fark = strlen(find) - strlen(replace), i = 0, j = 0;
	int length = strlen(Text), M = strlen(find);
	
	if ( fark < 0 )//E�er ��kar�l�cak kelime yeni kelimeden daha k�sa ise
		for ( i = length - 1; i >= x; i-- )//Metnin son harfinden ba�layarak her bir harf uzun kelime ile k�sa kelimenin fark� kadar sa�a kayd�r�l�r
			Text[i-fark] = Text[i];//Yeni kelimenin yerle�mesi i�in yer a��lm�� olur
	else if( fark > 0 ){//E�er ��kar�lacak kelime yeni kelimeden daha uzun ise 
	
		for ( i = x - M; i < x; i++ )//��kar�lacak olan kelimenin oldu�u k�s�m null karakter ile doldurulur
			Text[i] = '\0';//��kar�lacak kelime daha uzun oldu�u i�in fazla harfler g�r�nmemi� olur
	
		for ( i = x; Text[i] != '\0'; i++ ){//Find() fonksiyonunu ile bulunan kelimeden bir sonraki harften metnin son harfine kadar t�m harflerin kelimeler aras�ndaki fark kadar sola kayd�r�lmas�
			Text[i-fark] = Text[i];
			Text[i] = '\0';//�lgili harf sola kayd�r�ld��� zaman eski yerinde ayn� harf de�erinin kalmamas� i�in null de�eri atan�r
		}
	}
		
	i = x - M;
	while( replace[j] != '\0' ){//Yeni kelimenin metinde yerine yaz�lmas�
		
		Text[i] = replace[j];
		j++;
		i++;
	}
}

