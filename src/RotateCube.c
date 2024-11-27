#include <GL/freeglut.h>
#include "RotateCube.h"

// Roll, Pitch, Yaw açıları için yapı
CubeRotation cubeRotation;

// Küpü çizen fonksiyon
// Dikdörtgen prizmayı çizen fonksiyon
void drawCube() {
    glBegin(GL_QUADS);

    // Ön yüz (kırmızı)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.25f, -0.125f,  0.625f);
    glVertex3f( 0.25f, -0.125f,  0.625f);
    glVertex3f( 0.25f,  0.125f,  0.625f);
    glVertex3f(-0.25f,  0.125f,  0.625f);

    // Arka yüz (yeşil)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.25f, -0.125f, -0.625f);
    glVertex3f( 0.25f, -0.125f, -0.625f);
    glVertex3f( 0.25f,  0.125f, -0.625f);
    glVertex3f(-0.25f,  0.125f, -0.625f);

    // Sol yüz (mavi)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.25f, -0.125f, -0.625f);
    glVertex3f(-0.25f, -0.125f,  0.625f);
    glVertex3f(-0.25f,  0.125f,  0.625f);
    glVertex3f(-0.25f,  0.125f, -0.625f);

    // Sağ yüz (sarı)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 0.25f, -0.125f, -0.625f);
    glVertex3f( 0.25f, -0.125f,  0.625f);
    glVertex3f( 0.25f,  0.125f,  0.625f);
    glVertex3f( 0.25f,  0.125f, -0.625f);

    // Üst yüz (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.25f,  0.125f, -0.625f);
    glVertex3f( 0.25f,  0.125f, -0.625f);
    glVertex3f( 0.25f,  0.125f,  0.625f);
    glVertex3f(-0.25f,  0.125f,  0.625f);

    // Alt yüz (mor)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.25f, -0.125f, -0.625f);
    glVertex3f( 0.25f, -0.125f, -0.625f);
    glVertex3f( 0.25f, -0.125f,  0.625f);
    glVertex3f(-0.25f, -0.125f,  0.625f);

    glEnd();
}


// Roll, pitch ve yaw değerlerine göre küpü döndüren fonksiyon
void rotate_cube(CubeRotation rotation) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera ayarı (bakış yönü)
    gluLookAt(3.0, 3.0, 3.0, // Kamera konumu (sağ üst ve arka pozisyondan bakış)
    0.0, 0.0, 0.0,  // Bakılan nokta (cismin merkezi)
    0.0, 1.0, 0.0); // Yukarı yön (y ekseni pozitif yön)

    // Dönüş merkezini cismin merkezine getir
    glTranslatef(0.0f, 0.0f, 0.0f); // Cismin merkezini sıfır noktasına getir

    // Roll, pitch, yaw değerlerine göre cismin merkez etrafında döndür
    glRotatef(rotation.pitch, -1.0f, 0.0f, 0.0f);  // X ekseni etrafında (pitch)
    glRotatef(rotation.yaw, 0.0f, 1.0f, 0.0f); // Y ekseni etrafında (yaw)
    glRotatef(rotation.roll, 0.0f, 0.0f, 1.0f);   // Z ekseni etrafında (roll)

    // Cismi orijinal konumuna geri getir
    glTranslatef(0.0f, 0.0f, 0.0f);

    // Dikdörtgen prizmayı çiz
    drawCube();

    glutSwapBuffers(); // Çift buffer kullanımı
}


// Görüntüyü güncelleyen fonksiyon
void display() {
    rotate_cube(cubeRotation);
}

// Roll, pitch, yaw değerlerini güncelleyen fonksiyon
void update_rotation(CubeRotation rotation) {
    // Yapıyı güncelle
    cubeRotation = rotation;

    // Ekranı yeniden çiz
    glutPostRedisplay();
    glutMainLoopEvent();
}

// OpenGL ayarlarını yapan fonksiyon
void init() {
    glEnable(GL_DEPTH_TEST); // Derinlik testi (3D için gerekli)

    // Arka plan rengini gri olarak ayarla
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // Perspektif ayarı
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// OpenGL uygulamasını başlatan fonksiyon
void init_rotate_cube(int argc, char** argv, CubeRotation *rotation) {
    glutInit(&argc, argv); // GLUT'u başlatma
    cubeRotation = *rotation;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Rotating Cube");

    init();

    glutDisplayFunc(display);
}
