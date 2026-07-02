#pragma once

#include "camera.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int valid;
    float brightness;
    float confidence;
} VisionResult;

int vision_init(void);
VisionResult vision_analyze(const CameraFrame *frame);
void vision_cleanup(void);

#ifdef __cplusplus
}
#endif
