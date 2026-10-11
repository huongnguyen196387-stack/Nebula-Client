#include "opengl_test_harness.hpp"
#include "../include/nebulaclient.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <cmath>
#include <cstdlib>

namespace Nebula {
    bool OpenGlesTestHarness::RunTestContext() {
        LOGI("OpenGlesTestHarness (Standalone OpenGL ES Smoke Test): Bắt đầu kiểm tra EGL pbuffer và xác minh pixel.");

        EGLDisplay display = EGL_NO_DISPLAY;
        EGLSurface surface = EGL_NO_SURFACE;
        EGLContext context = EGL_NO_CONTEXT;
        bool eglInitialized = false;
        bool success = false;

        do {
            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            if (display == EGL_NO_DISPLAY) {
                LOGE("OpenGlesTestHarness: Không thể lấy EGLDisplay. Mã lỗi: %d", eglGetError());
                break;
            }

            EGLint major, minor;
            if (!eglInitialize(display, &major, &minor)) {
                LOGE("OpenGlesTestHarness: Khởi tạo EGL thất bại. Mã lỗi: %d", eglGetError());
                break;
            }
            eglInitialized = true;

            if (!eglBindAPI(EGL_OPENGL_ES_API)) {
                LOGE("OpenGlesTestHarness: eglBindAPI thất bại. Mã lỗi: %d", eglGetError());
                break;
            }

            const EGLint configAttribs[] = {
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs;
            if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0) {
                LOGE("OpenGlesTestHarness: Không chọn được EGLConfig phù hợp.");
                break;
            }

            const EGLint pbufferAttribs[] = {
                EGL_WIDTH, 64,
                EGL_HEIGHT, 64,
                EGL_NONE
            };

            surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
            if (surface == EGL_NO_SURFACE) {
                LOGE("OpenGlesTestHarness: Tạo pbuffer surface thất bại.");
                break;
            }

            const EGLint contextAttribs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
            };

            context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
            if (context == EGL_NO_CONTEXT) {
                LOGE("OpenGlesTestHarness: Tạo EGL context thất bại.");
                break;
            }

            if (!eglMakeCurrent(display, surface, surface, context)) {
                LOGE("OpenGlesTestHarness: eglMakeCurrent thất bại.");
                break;
            }

            glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                LOGE("OpenGlesTestHarness: Lỗi GLES render (glGetError): %d", glErr);
                break;
            }

            glFinish();

            unsigned char pixel[4] = {0, 0, 0, 0};
            glReadPixels(32, 32, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

            GLenum readErr = glGetError();
            if (readErr != GL_NO_ERROR) {
                LOGE("OpenGlesTestHarness: Lỗi glReadPixels: %d", readErr);
                break;
            }

            unsigned char expectedR = static_cast<unsigned char>(0.2f * 255.0f + 0.5f);
            unsigned char expectedG = static_cast<unsigned char>(0.3f * 255.0f + 0.5f);
            unsigned char expectedB = static_cast<unsigned char>(0.4f * 255.0f + 0.5f);
            unsigned char expectedA = static_cast<unsigned char>(1.0f * 255.0f + 0.5f);

            int tolerance = 3;
            bool rMatch = std::abs((int)pixel[0] - expectedR) <= tolerance;
            bool gMatch = std::abs((int)pixel[1] - expectedG) <= tolerance;
            bool bMatch = std::abs((int)pixel[2] - expectedB) <= tolerance;
            bool aMatch = std::abs((int)pixel[3] - expectedA) <= tolerance;

            if (!rMatch || !gMatch || !bMatch || !aMatch) {
                LOGE("OpenGlesTestHarness: Kiểm tra pixel thất bại! Đọc được: R=%d, G=%d, B=%d, A=%d",
                     pixel[0], pixel[1], pixel[2], pixel[3]);
                break;
            }

            LOGI("OpenGlesTestHarness: Smoke test pbuffer render và xác minh pixel thành công.");
            success = true;
        } while (0);

        if (display != EGL_NO_DISPLAY && eglInitialized) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (context != EGL_NO_CONTEXT) {
                eglDestroyContext(display, context);
            }
            if (surface != EGL_NO_SURFACE) {
                eglDestroySurface(display, surface);
            }
            eglTerminate(display);
        }

        return success;
    }
}
