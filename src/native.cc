#include <2d_renderer/renderer.h>

#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <GLFW/glfw3.h>

#define SK_GL
#define SkDebugf(...)

#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkPath.h>
#include <include/core/SkGraphics.h>
#include <include/core/SkImageEncoder.h>
#include <include/core/SkString.h>
#include <include/core/SkSurface.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkColorSpace.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/gl/GrGLInterface.h>

extern "C"
{
    void r2d_renderer_init_native(struct r2d_renderer *r, void *_window, void *data, void (*on_frame)(void *, float))
    {
        GLFWwindow *window = (GLFWwindow *)_window;
        memset((void *)r, 0, sizeof *r);
        r->state.matrix[0] = 1;
        r->state.matrix[4] = 1;

        sk_sp<GrGLInterface const> interface = GrGLMakeNativeInterface();

        GrDirectContext *context = GrDirectContext::MakeGL(interface).release();
        GrGLFramebufferInfo framebufferInfo;
        framebufferInfo.fFBOID = 0;
        framebufferInfo.fFormat = GL_RGBA8;
        SkColorType colorType = kRGBA_8888_SkColorType;
        GrBackendRenderTarget backendRenderTarget(800, 800,
                                                  0, // sample count
                                                  0, // stencil bits
                                                  framebufferInfo);

        SkSurface *surface = SkSurface::MakeFromBackendRenderTarget(context, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr).release();
        SkCanvas *canvas = surface->getCanvas();
        SkPath *path = new SkPath;
        r->state.path = path;
        SkPaint *paint = new SkPaint;
        paint->setAntiAlias(true);
        r->state.paint = paint;
        r->surface = (void *)surface;
        r->canvas = (void *)canvas;

        struct timeval start;
        struct timeval end;
        while (!glfwWindowShouldClose(window))
        {
            context->flush();
            canvas->clear(-1);
            gettimeofday(&start, NULL);
            on_frame(data, 0.606);
            gettimeofday(&end, NULL);
            uint64_t elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 +
                                    (end.tv_usec - start.tv_usec);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}

#define RETRIEVE_CANVAS                     \
    SkCanvas *c = (SkCanvas *)r->canvas;    \
    SkSurface *s = (SkSurface *)r->surface; \
    SkPath *p = (SkPath *)r->state.path;    \
    SkPaint *paint = (SkPaint *)r->state.paint;

void r2d_renderer_update_transform(struct r2d_renderer *r)
{
    RETRIEVE_CANVAS
    SkMatrix m;
    m.setAll(
        r->state.matrix[0],
        r->state.matrix[1],
        r->state.matrix[2],
        r->state.matrix[3],
        r->state.matrix[4],
        r->state.matrix[5], 0, 0, 1);
    c->setMatrix(m);
}

void r2d_renderer_save(struct r2d_renderer *r)
{
    RETRIEVE_CANVAS
    c->save();
}
void r2d_renderer_restore(struct r2d_renderer *r)
{
    RETRIEVE_CANVAS
    c->restore();
}

void r2d_renderer_set_fill(struct r2d_renderer *r, uint32_t x)
{
    RETRIEVE_CANVAS
    paint->setARGB((x >> 24) & 255,
                   (x >> 8) & 255,
                   (x >> 16) & 255,
                   x & 255);
}

void r2d_renderer_set_stroke(struct r2d_renderer *r, uint32_t x)
{
}

void r2d_renderer_begin_path(struct r2d_renderer *r)
{
    RETRIEVE_CANVAS
    p->~SkPath();
    new (p) SkPath();
}

void r2d_renderer_move_to(struct r2d_renderer *r, float x, float y)
{
    RETRIEVE_CANVAS
    p->moveTo(x, y);
}

void r2d_renderer_line_to(struct r2d_renderer *r, float x, float y)
{
    RETRIEVE_CANVAS
    p->lineTo(x, y);
}

void r2d_renderer_fill(struct r2d_renderer *r)
{
    RETRIEVE_CANVAS
    c->drawPath(*p, *paint);
}
