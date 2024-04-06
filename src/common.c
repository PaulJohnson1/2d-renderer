#include <2d_renderer/renderer.h>

#include <string.h>
#include <math.h>

void r2d_renderer_state_store(struct r2d_renderer *r, struct r2d_renderer_state *s)
{
    memcpy(s, &r->state, sizeof *s);
    r2d_renderer_save(r);
}

void r2d_renderer_state_restore(struct r2d_renderer *r, struct r2d_renderer_state *s)
{
    memcpy(&r->state, s, sizeof *s);
    r2d_renderer_restore(r);
}

void r2d_renderer_set_transform(struct r2d_renderer *r, float a, float b, float c, float d, float e, float f)
{
    r->state.matrix[0] = a;
    r->state.matrix[1] = b;
    r->state.matrix[2] = c;
    r->state.matrix[3] = d;
    r->state.matrix[4] = e;
    r->state.matrix[5] = f;
    r2d_renderer_update_transform(r);
}

void r2d_renderer_translate(struct r2d_renderer *r, float x, float y)
{
    r->state.matrix[2] += x * r->state.matrix[0] + y * r->state.matrix[3];
    r->state.matrix[5] += x * r->state.matrix[1] + y * r->state.matrix[4];
    r2d_renderer_update_transform(r);
}

void r2d_renderer_rotate(struct r2d_renderer *r, float a)
{
    float cos_a = cosf(a);
    float sin_a = sinf(a);
    float original0 = r->state.matrix[0];
    float original1 = r->state.matrix[1];
    float original3 = r->state.matrix[3];
    float original4 = r->state.matrix[4];
    r->state.matrix[0] = original0 * cos_a + original1 * -sin_a;
    r->state.matrix[1] = original0 * sin_a + original1 * cos_a;
    r->state.matrix[3] = original3 * cos_a + original4 * -sin_a;
    r->state.matrix[4] = original3 * sin_a + original4 * cos_a;
    r2d_renderer_update_transform(r);
}

void r2d_renderer_scale2(struct r2d_renderer *r, float x, float y)
{
    r->state.matrix[0] *= x;
    r->state.matrix[1] *= x;
    r->state.matrix[3] *= y;
    r->state.matrix[4] *= y;
    r2d_renderer_update_transform(r);
}

void r2d_renderer_scale(struct r2d_renderer *r, float s)
{
    r2d_renderer_scale2(r, s, s);
}
