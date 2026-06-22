#include "camera.h"

#ifdef _cplusplus
extern "C" {
#endif
int camera_init(int width, int height){

}
#ifdef _cplusplus
}
#endif

#ifdef _cplusplus
extern "C" {
#endif
int camera_capture(CameraFrame *frame);
#ifdef _cplusplus
}
#endif

#ifdef _cplusplus
extern "C" {
#endif
void camera_release_frame(CameraFrame *frame);
#ifdef _cplusplus
}
#endif

#ifdef _cplusplus
extern "C" {
#endif
void camera_cleanup();
#ifdef _cplusplus
}
#endif


