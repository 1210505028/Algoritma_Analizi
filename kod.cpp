#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define A_SIZE 20
#define G_SIZE 10
#define MAX_W 10

// Sonsuz sayı için maksimum değer
#define INF INT_MAX

// Rastgele sayıları üretip diziyi dolduran fonksiyon
void generate(int a[], int size);

// Diziyi artan sırada sıralamak için kullanılan quicksort algoritması
void quicksort(int a[], int low, int high);

// Quicksort'ta kullanılan partition fonksiyonu
int partition(int a[], int low, int high);

// İki elemanı birbirleriyle değiştiren fonksiyon
void swap(int* a, int* b);

// Maksimum alt dizi toplamının ortalamasını hesaplayan fonksiyon
int calculate_average_max_sum(int a[], int size);

// Floyd-Warshall algoritmasını kullanarak grafikteki en kısa yolları hesaplayan fonksiyon
void calculate_shortest_paths(int g[][G_SIZE], int d[][G_SIZE], int size);

// Bir diziyi ekrana yazdıran fonksiyon
void print_array(int a[], int size);

// Bir grafik için komşuluk matrisi temsili olanı ekrana yazdıran fonksiyon
void print_graph(int g[][G_SIZE], int size);

// Grafikteki en kısa yolları ekrana yazdıran fonksiyon
void print_shortest_paths(int d[][G_SIZE], int size, int t);

int main() {
    int a[A_SIZE]; // Rastgele sayılardan oluşan dizi
    int g[G_SIZE][G_SIZE] = {
        // Bir grafik temsil eden komşuluk matrisi
        { 0, 10,  3,  0,  0,  5,  0, 17,  0, 22},
        {10,  0,  5,  0,  2,  0, 13,  0,  0,  0},
        { 3,  5,  0,  2,  0,  4,  0, 21,  0, 11},
        { 0,  0,  2,  0,  7,  0,  6,  0,  0,  0},
        { 0,  2,  0,  7,  0,  6,  0,  0, 19,  0},
        { 5,  0,  4,  0,  6,  0,  9,  3,  0,  0},
        { 0, 13,  0,  6,  0,  9,  0,  4,  0,  0},
        {17,  0, 21,  0,  0,  3,  4,  0,  8,  0},
        { 0,  0,  0,  0, 19,  0,  0,  8,  0,  5},
        {22,  0, 11,  0,  0,  0,  0,  0,  5,  0}
    };
    int d[G_SIZE][G_SIZE]; // En kısa yol matrisi
    int t; // Ortalama maksimum toplam değeri

    // Rastgele sayı üretimi için seed değeri
    srand(time(NULL));

    // Çalışma süresini hesaplamak için clock kullanılıyoruz
    clock_t start, end;
    double time;

    start = clock();

    generate(a, A_SIZE); // Diziyi rastgele sayılarla dolduruyoruz
    printf("A:\n");
    print_array(a, A_SIZE); // Diziyi ekrana yazdırıyoruz

    quicksort(a, 0, A_SIZE - 1); // Diziyi sıralıyoruz

    printf("\nSirali Dizi:\n");
    print_array(a, A_SIZE); // Sıralı diziyi ekrana yazdırıyoruz

    t = calculate_average_max_sum(a, A_SIZE); // Ortalama maksimum toplamı hesaplıyoruz
    printf("\nOrtalama Maksimum Toplam: %d\n", t);

    printf("\nGraf:\n");
    print_graph(g, G_SIZE); // Grafik komşuluk matrisini ekrana yazdırıyoruz

    calculate_shortest_paths(g, d, G_SIZE); // En kısa yolları hesaplıyoruz
    printf("\nEn Kisa Yollar:\n");
    print_graph(d, G_SIZE); // En kısa yollar matrisini ekrana yazdırıyoruz

    printf("\n%d'den Daha Kisa Yollar:\n", t);
    print_shortest_paths(d, G_SIZE, t); // Belirli bir değerden daha kısa yolları ekrana yazdırıyoruz

    end = clock();
    time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nCalisma suresi: %f saniye\n", time);

    return 0;
}

// Diziyi rastgele sayılarla dolduran fonksiyon
void generate(int a[], int size) {
    if (a == NULL) {
        printf("Dizi bellek hatası: Geçersiz dizi.\n");
        return;  // Hata kontrolü
    }

    for (int i = 0; i < size; i++) {
        a[i] = rand() % (2 * MAX_W) - MAX_W;
    }
}

// Quicksort'ta kullanılan partition fonksiyonu
int partition(int a[], int low, int high) {
    int pivot = a[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (a[j] < pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[high]);
    return (i + 1);
}

// İki değeri takas eden fonksiyon
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Diziyi quicksort algoritması ile sıralayan fonksiyon
void quicksort(int a[], int low, int high) {
    if (a == NULL) {
        printf("Dizi bellek hatası: Geçersiz dizi.\n");
        return;  // Hata kontrolü
    }

    if (low < high) {
        int pi = partition(a, low, high);
        quicksort(a, low, pi - 1);
        quicksort(a, pi + 1, high);
    }
}

// Dizideki ortalama maksimum toplamı hesaplayan fonksiyon
int calculate_average_max_sum(int a[], int size) {
    int t = 0, current_sum = 0, count = 0;
    for (int i = 0; i < size; i++) {
        if (current_sum + a[i] > 0) {
            current_sum = current_sum + a[i];
        }
        else {
            current_sum = 0;
        }
        if (current_sum > t) { // hesaplanan daha büyükse maksimumdan maksimumu güncelliyoruz ve count sayisini arttırıyoruz
            t = current_sum;
            count++;
        }
    }
    return t / count;
}


// Grafikteki en kısa yolları hesaplayan fonksiyon
void calculate_shortest_paths(int g[][G_SIZE], int d[][G_SIZE], int size) {
    // İlk olarak d matrisini g matrisine eşitliyoruz
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                d[i][j] = 0;
            }
            else if (g[i][j] != 0) {
                d[i][j] = g[i][j];
            }
            else {
                d[i][j] = INF;
            }
        }
    }

    // Floyd-Warshall algoritması ile en kısa yolları hesaplıyoruz
    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (d[i][k] != INF && d[k][j] != INF &&
                    d[i][k] + d[k][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }
}

// Diziyi ekrana yazdıran fonksiyon
void print_array(int a[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", a[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
}

// Grafik matrisini ekrana yazdıran fonksiyon
void print_graph(int g[][G_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (g[i][j] == INF) {
                printf("INF ");
            }
            else {
                printf("%3d ", g[i][j]);
            }
        }
        printf("\n");
    }
}


// Belirli bir değerden daha kısa yolları ekrana yazdıran fonksiyon
void print_shortest_paths(int d[][G_SIZE], int size, int t) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (d[i][j] < t) {
                printf("(%d, %d): %d\n", i, j, d[i][j]);
            }
   }
 }
}