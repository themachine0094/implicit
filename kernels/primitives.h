#define REG_L -1
#define REG_R -2

#define ENT_TYPE_BOX 1
typedef struct PACKED
{
  FLT_TYPE bounds[6];
} i_box;

#define ENT_TYPE_SPHERE 2
typedef struct PACKED
{
  FLT_TYPE center[3];
  FLT_TYPE radius;
} i_sphere;

#define ENT_TYPE_GYROID 3
typedef struct PACKED
{
  FLT_TYPE scale;
  FLT_TYPE thickness;
} i_gyroid;

#define ENT_TYPE_CSG 0

typedef enum
{
    OP_NONE = 0,
    OP_BOOL_UNION = 1,
    OP_BOOL_INTERSECTION = 2,
} op_type;

typedef struct PACKED
{
    op_type type;
    UINT32_TYPE left_src;
    UINT32_TYPE right_src;
    UINT32_TYPE dest;
} op_step;