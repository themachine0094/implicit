#define DX 0.0001f
#define AMB_STEP 0.05f
#define STEP_FOS 0.5f

#include "kernel_primitives.h"

/*Macro to numerically compute the gradient vector of a given
implicit function.*/
#define GRADIENT(func, pt, norm, v0){                \
    pt.x += DX; float vx = func; pt.x -= DX;         \
    pt.y += DX; float vy = func; pt.y -= DX;         \
    pt.z += DX; float vz = func; pt.z -= DX;         \
    norm = (float3)((vx - v0) / DX,                  \
                    (vy - v0) / DX,                  \
                    (vz - v0) / DX);                 \
}

uint colorToInt(float3 rgb)
{
  uint color = 0xff000000;
  color |= ((uint)(min(1.0f, max(0.0f, rgb.x)) * 255));
  color |= ((uint)(min(1.0f, max(0.0f, rgb.y)) * 255)) << 8;
  color |= ((uint)(min(1.0f, max(0.0f, rgb.z)) * 255)) << 16;
  return color;
}

uint sphere_trace(global uchar* packed,
                  global uint* offsets,
                  global uchar* types,
                  local float* valBuf,
                  local float* regBuf,
                  uint nEntities,
                  global op_step* steps,
                  uint nSteps,
                  float3 pt,
                  float3 dir,
                  int iters,
                  float tolerance,
                  float boundDist
#ifdef CLDEBUG
                  , uchar debugFlag
#endif
                  )
{
  if (nEntities == 0)
    return BACKGROUND_COLOR;
  
  dir = normalize(dir);
  float3 norm = (float3)(0.0f, 0.0f, 0.0f);
  bool found = false;
  float dTotal = 0.0f;
  float d;
  for (int i = 0; i < iters; i++){
    d = f_entity(packed, offsets, types, valBuf, regBuf,
                       nEntities, steps, nSteps, &pt);
    if (d < 0.0f) break;
    if (d < tolerance){
      GRADIENT(f_entity(packed, offsets, types, valBuf, regBuf,
                        nEntities, steps, nSteps, &pt),
               pt, norm, d);
      found = true;
      break;
    }
    pt += dir * (d * STEP_FOS);
    dTotal += d * STEP_FOS;
    if (i > 3 && dTotal > boundDist) break;
  }
  
  if (!found){
#ifdef CLDEBUG
    if (debugFlag)
      printf("Can't find intersection. Rendering background color\n");
#endif
    return BACKGROUND_COLOR;
  }

  pt -= dir * AMB_STEP;
  float amb = (f_entity(packed, offsets, types, valBuf, regBuf,
                        nEntities, steps, nSteps, &pt) - d) / AMB_STEP;
  norm = normalize(norm);
  d = dot(norm, -dir);
  float cd = 0.2f;
  float cl = 0.4f * amb + 0.6f;
  float3 color1 = (float3)(cd, cd, cd)*(1.0f-d) + (float3)(cl, cl, cl)*d;
#ifdef CLDEBUG
  if (debugFlag){
    printf("Floating point color: (%.2f, %.2f, %.2f)\n",
           color1.x, color1.y, color1.z);
  }
#endif
  return colorToInt(color1);
}
