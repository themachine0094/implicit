#define ENT_TYPE_BOX 1
struct i_box
{
  FLT_TYPE bounds[6];
} PACKED;

#define ENT_TYPE_SPHERE 2
struct i_sphere
{
  FLT_TYPE center[3];
  FLT_TYPE radius;
} PACKED;

#define ENT_TYPE_GYROID 3
struct i_gyroid
{
  FLT_TYPE scale;
  FLT_TYPE thickness;
} PACKED;

union i_entity
{
  struct i_box box;
  struct i_sphere sphere;
  struct i_gyroid gyroid;
};

struct wrapper
{
  union i_entity entity;
  UINT_TYPE type;
} PACKED;