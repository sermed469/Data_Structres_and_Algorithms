#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 10000

int Find( char*, char*, char*, int, int* );//Metinden çýkarýlacak kelimeyi Boyer-Moore Horspool Algoritmasýna göre arayan fonksiyon
void Replace( char*, char*, char*, int );//Metinden eski kelimeyi çýkartýp yeni kelimeyi yerleþtiren fonksiyon
void shiftTable( char*, int*, int );//pattern'in ne kadar kaydýrýlýcaðýný belirleyen tabloyu oluþturan fonksiyon

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
	fread(Text,sizeof(char),MAX,fs);//Dosyadaki metnin okunarak Text[] dizisine atanmasý
	fclose(fs);
	
	printf( "\nMetin:" );
	puts(Text);
	
	shiftTable(find,table,k);//Aranacak kelimedeki harflerin kelimede bulunduðu indise göre tablo oluþturan fonksiyon
	
    clock_gettime(CLOCK_MONOTONIC, &start);
	sayac = Find(find,Text,replace,k,table);//Boyer-Moore Horspool Algoritmasý kullanýlarak metinden çýkarýlacak kelimeyi bulan ve kaç deðiþim olduðunu döndüren fonksiyon
	clock_gettime(CLOCK_MONOTONIC, &end);
	
	printf( "\nYeni Metin: " );
	puts(Text);
	
	fs = fopen( filename, "w" );
	fprintf(fs, "%s", Text);//Find and Replace iþlemleri yapýldýktan sonra yeni oluþan metnin yine ayný dosyaya yazdýrýlmasý
	fclose(fs);
	
	printf( "\nDegisim Sayisi: %d\n", sayac );//Kaç kez deðiþim yapýldýðýnýn ekrana yazdýrýlmasý
	printf( "Gecen sure: %lf milisaniye\n", (double)((long int)(end.tv_sec-start.tv_sec)*1000000000 + (end.tv_nsec-start.tv_nsec)) / 1000000 );//Find and Replace iþlemininin çalýþma süresinin ekrana yazdýrýlmasý
	return 0;	
}

int Find( char find[], char Text[], char replace[], int k, int table[] ){//Boyer-Moore Horspool Algoritmasý kullanýlarak metinden çýkarýlacak kelimenin bulunmasý
	
	int N = strlen(Text), M = strlen(find), i = M-1, j, count = 0, shift = 0;
	
	while ( i  < strlen(Text) ){//Text[] dizisinin sonuna gelen kadar döngü devam eder
	
		j = M-1;
		
		while ( (j >= 0) && ( (find[j] == Text[i]) || ( !k && (fabs(find[j]-Text[i]) == fabs('A'-'a') ) ) ) ){//aranan kelimedeki harf ile metindeki ilgili harf ayný olduðu sürece döngü devam eder
		//k deðeri eðer 0 ise yani case sensitive deðilse or iþleminin sað kýsmý aktif hale gelir. 
			j--;//aranan kelimedeki bür önceki harfe geçilmesi
			i--;//Metindeki bir önceki harfe geçilmesi
		}
			
		if ( j == M - 1 ){//Eðer aranan kelimenin son harfi metindeki harf ile eþleþmediyse

			shift = table[Text[i] - ' '];//metinde kontrol edilen harfin tablodaki deðeri kadar shift deðeri artýrýlýr
		}
		else if ( (j < M - 1) && (j >= 0) ){//Eðer aranan kelimenin bir kýsmý eþleþtiyse
			
			if ( table[Text[i] - ' '] == M)//Eþleþmeyen harf aranan kelimede yoksa
				shift = M;//aranan kelimenin uzunluðu kadar kaydýr
			else//Eþleþmeyen harf aranan kelimede varsa
				shift = table[find[M-1] - ' '];//kelimenin son harfinin tablodaki deðeri kadar kaydýr	
			
			shift += M - 1 - j;//kontrol sýrasýnda azalan i deðerinin eski haline getirilmesi
		}
		else{//Eðer aranan kelime bulunduysa
		
			shift = M;//Kontrol sýrasýnda i indisi arananan kelimenin uzunluðu kadar azaldýðý içim M kadar artýrýlýr
			Replace(Text,find,replace,i+shift+1);//Yer deðiþtirme fonksiyonu çaðrýlýr	
			count++;//Deðiþim sayýsý bir artýrýlýr
			shift += strlen(replace) - strlen(find);//Eklenen ve çýkarýlan kelimelerin uzunluklarýna baðlý olarak kayma miktarýnýn hesaplanmasý
			shift += M;
		}
		
		i += shift;
	}
	
	return count;
}

void shiftTable( char find[], int table[], int k ){//Boþluk kaarakterinden baþlayýp 'z' karakterine kadar her bir harf için pattern kelimesinin kaydýrýlma miktarýnýn heasplanmasý 
	
	int i, M = strlen(find), size = 'z'-' '+1;//Oluþturulucak tabloda boþluk karakteri offset deðeri olarak ayarlanýr
	
	for ( i = 0; i < size; i++ )//Öncelikle tüm karakterler için aranan kelimenin uzunluðu kadar deðer atanýr
		table[i] = M;
 	
 	for ( i = 0; i < M-1; i++ ){//Aranan kelimedeki harflerin tablodaki deðerlerinin kelimede bulunduklarý indise göre ayarlanmasý
 		
		table[find[i] - ' '] = M - i - 1;
		
		if ( k == 0 ){//Eðer case sensitive özelliði devre dýþý ise aranan kelimedeki harfin kendisiyle denk olan büyük ya da küçük harfin de tablodaki deðeri almasý
			
			if ( find[i] > 'Z' )//Eðer küçük harf ise
				table['A' - 'a' + find[i] - ' '] = M - i - 1;
			else//Eðer büyük harf ise
				table['a' - 'A' + find[i] - ' '] = M - i - 1;
		}
	 }
}

void Replace( char Text[], char find[], char replace[], int x ){//Find() fonksiyonu ile bulunan kelimenin yeni kelime ile deðiþtirilmesi
	
	int fark = strlen(find) - strlen(replace), i = 0, j = 0;
	int length = strlen(Text), M = strlen(find);
	
	if ( fark < 0 )//Eðer çýkarýlýcak kelime yeni kelimeden daha kýsa ise
		for ( i = length - 1; i >= x; i-- )//Metnin son harfinden baþlayarak her bir harf uzun kelime ile kýsa kelimenin farký kadar saða kaydýrýlýr
			Text[i-fark] = Text[i];//Yeni kelimenin yerleþmesi için yer açýlmýþ olur
	else if( fark > 0 ){//Eðer çýkarýlacak kelime yeni kelimeden daha uzun ise 
	
		for ( i = x - M; i < x; i++ )//Çýkarýlacak olan kelimenin olduðu kýsým null karakter ile doldurulur
			Text[i] = '\0';//Çýkarýlacak kelime daha uzun olduðu için fazla harfler görünmemiþ olur
	
		for ( i = x; Text[i] != '\0'; i++ ){//Find() fonksiyonunu ile bulunan kelimeden bir sonraki harften metnin son harfine kadar tüm harflerin kelimeler arasýndaki fark kadar sola kaydýrýlmasý
			Text[i-fark] = Text[i];
			Text[i] = '\0';//Ýlgili harf sola kaydýrýldýðý zaman eski yerinde ayný harf deðerinin kalmamasý için null deðeri atanýr
		}
	}
		
	i = x - M;
	while( replace[j] != '\0' ){//Yeni kelimenin metinde yerine yazýlmasý
		
		Text[i] = replace[j];
		j++;
		i++;
	}
}

