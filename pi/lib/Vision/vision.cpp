#include "vision.h"

#include <stddef.h>

int vision_init(void){
    return 0;
}

VisionResult vision_analyze(const CameraFrame *frame){
    VisionResult result;

    result.valid = 0;
    result.brightness = 0.0f;
    result.confidence = 0.0f;

    if(frame == nullptr || frame->data == nullptr){
        return result;
    }

    if(frame->width <= 0 || frame->height <= 0 || frame->channels != 3){
        return result;
    }

    const int pixel_count = frame->width * frame->height;
    const int sample_step = 16;

    unsigned long long brightness_sum = 0;
    int samples = 0;

    for(int i = 0; i < pixel_count; i += sample_step){
        int offset = i * frame->channels;

        unsigned int r = frame->data[offset + 0];
        unsigned int g = frame->data[offset + 1];
        unsigned int b = frame->data[offset + 2];

        brightness_sum += (r + g + b) / 3;
        samples++;
    }

    if(samples == 0){
        return result;
    }

    result.brightness = (float)brightness_sum / (float)samples;
    result.confidence = result.brightness / 255.0f;

    if(result.brightness > 20.0f && result.brightness < 245.0f){
        result.valid = 1;
    }

    return result;
}

void vision_cleanup(void){
    return;
}

