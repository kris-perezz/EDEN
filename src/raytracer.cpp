#include "raytracer.h"
#include "color.h"     // Contains unpackRGB(), writeColour() if needed
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>     // For FLT_MAX

// Create an Image struct and allocate its data buffer.
Image *createImage(int width, int height) {
    Image *img = malloc(sizeof(Image));
    if (!img) exit(1);
    img->width = width;
    img->height = height;
    img->data = malloc(width * height * 3);
    if (!img->data) {
        free(img);
        exit(1);
    }
    return img;
}

// Free the Image structure.
void freeImage(Image *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

// Save image to file in binary PPM (P6) format.
int saveImageAsPPM(const char *filename, const Image *img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fprintf(fp, "P6\n%d %d\n255\n", img->width, img->height);
    fwrite(img->data, 1, img->width * img->height * 3, fp);
    fclose(fp);
    return 1;
}

// Render a ray traced scene into the provided image.
// This code is based on your assg.c logic (with anti-aliasing)
void renderRayTracedScene(const World *world, 
                          const Vec3 lightPos, 
                          float lightBrightness, 
                          const Vec3 backgroundColor, 
                          int imageWidth, int imageHeight, 
                          float viewportHeight, float focalLength, 
                          Image *img) {
    // Compute viewport width based on image aspect ratio.
    float viewportWidth = viewportHeight * ((float)imageWidth / imageHeight);
    // Camera positioned at origin.
    Vec3 cameraPos = {0.0f, 0.0f, 0.0f};
    // Anti-aliasing factor: 9 sub-pixels.
    const float AAFRACTION = 1.0f / 9.0f;

    // Precompute 3x3 anti-aliasing offsets.
    Vec3 rayOffsets[9];
    int offsetIndex = 0;
    float offsets[3] = {-1.0f / 3.0f, 0.0f, 1.0f / 3.0f};
    for (int oy = 0; oy < 3; oy++) {
        for (int ox = 0; ox < 3; ox++) {
            rayOffsets[offsetIndex++] = (Vec3){ offsets[ox], offsets[oy], -focalLength };
        }
    }
    
    // Pixel scale factors.
    float pxScale = viewportWidth / imageWidth;
    float pyScale = viewportHeight / imageHeight;
    
    // Loop over each pixel.
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            Vec3 accumulatedColor = {0.0f, 0.0f, 0.0f};
            
            float pxBase = (x - imageWidth / 2.0f) * pxScale;
            float pyBase = ((imageHeight - y - 1) - imageHeight / 2.0f) * pyScale;
            
            // Anti-aliasing: 9 samples per pixel.
            for (int offsetIdx = 0; offsetIdx < 9; offsetIdx++) {
                Vec3 rayDir = normalize((Vec3){
                    pxBase + rayOffsets[offsetIdx].x * pxScale,
                    pyBase + rayOffsets[offsetIdx].y * pyScale,
                    rayOffsets[offsetIdx].z
                });
                
                float closestT = FLT_MAX;
                Vec3 pixelColor = backgroundColor;
                float intensity = 0.0f;
                
                // Check intersection with each sphere in the world.
                for (int i = 0; i < world->size; i++) {
                    float t;
                    if (doesIntersect(world->spheres[i], cameraPos, rayDir, &t) && t < closestT) {
                        closestT = t;
                        Vec3 intersectionPoint = add(cameraPos, scalarMultiply(t, rayDir));
                        Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[i]->pos));
                        Vec3 lightDir = subtract(lightPos, intersectionPoint);
                        float lightDistanceSquared = dot(lightDir, lightDir);
                        lightDir = scalarMultiply(1.0f / sqrtf(lightDistanceSquared), lightDir);
                        intensity = lightBrightness * fmaxf(0.0f, dot(lightDir, normal)) / lightDistanceSquared;
                        
                        // Shadow check.
                        Vec3 shadowRayOrigin = add(intersectionPoint, scalarMultiply(0.001f, normal));
                        Vec3 shadowRayDir = normalize(subtract(lightPos, shadowRayOrigin));
                        int inShadow = 0;
                        for (int j = 0; j < world->size; j++) {
                            if (i == j) continue;
                            if (distance2(intersectionPoint, world->spheres[j]->pos) > lightDistanceSquared)
                                continue;
                            float tShadow;
                            if (doesIntersect(world->spheres[j], shadowRayOrigin, shadowRayDir, &tShadow)) {
                                inShadow = 1;
                                break;
                            }
                        }
                        if (inShadow) {
                            intensity *= 0.1f;  // Dim light in shadow.
                        }
                        pixelColor = scalarMultiply(intensity, world->spheres[i]->color);
                    }
                }
                if (closestT == FLT_MAX) {
                    pixelColor = backgroundColor;
                }
                accumulatedColor = add(accumulatedColor, pixelColor);
            }
            
            // Average anti-aliased samples.
            Vec3 finalColor = scalarMultiply(AAFRACTION, accumulatedColor);
            
            // Write finalColor (clamped to [0,1]) to the image buffer.
            int index = (y * imageWidth + x) * 3;
            int r = (int)(255 * fminf(1.0f, fmaxf(0.0f, finalColor.x)));
            int g = (int)(255 * fminf(1.0f, fmaxf(0.0f, finalColor.y)));
            int b = (int)(255 * fminf(1.0f, fmaxf(0.0f, finalColor.z)));
            img->data[index] = (unsigned char) r;
            img->data[index + 1] = (unsigned char) g;
            img->data[index + 2] = (unsigned char) b;
        }
    }
}
