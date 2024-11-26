#ifndef ROTATECUBE_H
#define ROTATECUBE_H

// Roll, pitch ve yaw değerlerine göre küpü döndüren ve OpenGL ayarlarını başlatan fonksiyonlar
typedef struct {
    float roll;
    float pitch;
    float yaw;
} CubeRotation;

void init_rotate_cube(int argc, char** argv, CubeRotation *rotation);
void update_rotation(CubeRotation rotation);
void rotate_cube(CubeRotation rotation);

#endif // ROTATECUBE_H
