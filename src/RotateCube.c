#include <stdio.h>
#include <math.h>
#include "RotateCube.h"

// Konsolu temizleyen yardımcı fonksiyon
void clear_screen() {
    printf("\033[H\033[J"); // ANSI kaçış kodları ile ekran temizleme
}

// Roll, pitch ve yaw değerlerini ekrana yazdıran fonksiyon
void rotate_cube(float roll, float pitch, float yaw) {
    clear_screen(); // Konsolu temizle

    // Roll, pitch ve yaw başlıkları
    printf("  Roll      Pitch      Yaw\n");
    printf("----------------------------\n");

    // Derece cinsinden roll, pitch ve yaw değerlerini yazdır
    printf("%7.2f°   %7.2f°   %7.2f°\n", roll, pitch, yaw);
}
