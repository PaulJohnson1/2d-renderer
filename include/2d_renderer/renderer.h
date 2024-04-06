#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
        struct r2d_renderer_state
        {
                float matrix[6];
                float global_alpha;

#ifdef __cplusplus
                void *path;
                void *paint;
#endif
        };

        struct r2d_renderer
        {
                struct r2d_renderer_state state;
#ifdef __cplusplus
                void *surface;
                void *canvas;
#else
        uint32_t context_id;
#endif
                float width;
                float height;
        };

        void r2d_renderer_init_native(struct r2d_renderer *, void *glfw_window, void *data, void (*on_frame)(void *, float));
        void r2d_renderer_init(struct r2d_renderer *);

        void r2d_renderer_set_fill(struct r2d_renderer *, uint32_t);
        void r2d_renderer_set_stroke(struct r2d_renderer *, uint32_t);

        void r2d_renderer_save(struct r2d_renderer *);
        void r2d_renderer_restore(struct r2d_renderer *);

        void r2d_renderer_state_store(struct r2d_renderer *, struct r2d_renderer_state *);
        void r2d_renderer_state_restore(struct r2d_renderer *, struct r2d_renderer_state *);
        void r2d_renderer_set_transform(struct r2d_renderer *, float, float, float, float, float, float);
        void r2d_renderer_update_transform(struct r2d_renderer *);

        void r2d_renderer_rotate(struct r2d_renderer *, float);
        void r2d_renderer_scale(struct r2d_renderer *, float);
        void r2d_renderer_scale2(struct r2d_renderer *, float, float);

        void r2d_renderer_begin_path(struct r2d_renderer *);
        void r2d_renderer_move_to(struct r2d_renderer *, float, float);
        void r2d_renderer_line_to(struct r2d_renderer *, float, float);

        void r2d_renderer_fill(struct r2d_renderer *);
        void r2d_renderer_stroke(struct r2d_renderer *);

        void r2d_renderer_end_of_frame(struct r2d_renderer *);
#ifdef __cplusplus
}
#endif
