#pragma once

#ifdef _cplusplus
extern "C" {
#endif

typedef struct {
    int width;
    int height;
    int channels;
    uint8_t *data;
} CameraFrame;

int camera_init(int width, int height);
int camera_capture(CameraFrame *frame);
void camera_release_frame(CameraFrame *frame);
void camera_cleanup();


#ifdef _cplusplus
}
#endif
