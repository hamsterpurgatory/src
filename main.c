// License: AGPLv3 / https://www.gnu.org/licenses/agpl-3.0.en.html
// HamsterPurgatory.com / kick.com/HamsterPurgatory

#ifdef WEB
    #include <emscripten.h>
    #include <emscripten/html5.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <math.h>   // sqrtf fabsf cosf sinf
#include <string.h> // memset memcpy

#define uint GLuint
#define sint GLint
#define MAX_MODELS 28 // hard limit, be aware and increase if needed

#define PLAY_RANDOM

//*************************************
// rendering functions
//*************************************
#define PI   3.14159265359f // PI
#define x2PI 6.28318530718f // PI * 2
#define d2PI 1.57079632679f // PI / 2
GLint projection_id;
GLint modelview_id;
GLint opacity_id;
GLint position_id;
GLint color_id;
GLint ambient_id;
typedef struct {float x,y,z,w;} vec;
typedef struct {float m[4][4];} mat;
typedef struct {GLuint vid,cid,iid,itp,ni;} ESModel;
void esBind(const GLenum target, GLuint* buffer, const void* data, const GLsizeiptr datalen, const GLenum usage)
{
    glGenBuffers(1, buffer);
    glBindBuffer(target, *buffer);
    glBufferData(target, datalen, data, usage);
}
ESModel esModelArray[MAX_MODELS];
uint esModelArray_index = 0; // used in the asset files to track load index
uint esBoundModel = 0;
void esBindModel(const uint id)
{
    glBindBuffer(GL_ARRAY_BUFFER, esModelArray[id].vid);
    glVertexAttribPointer(position_id, 3, GL_SHORT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, esModelArray[id].cid);
    glVertexAttribPointer(color_id, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, esModelArray[id].iid);
    esBoundModel = id;
}
#define esRenderModel() glDrawElements(GL_TRIANGLES, esModelArray[esBoundModel].ni, esModelArray[esBoundModel].itp, 0)
void esBindRender(const uint id)
{
    glBindBuffer(GL_ARRAY_BUFFER, esModelArray[id].vid);
    glVertexAttribPointer(position_id, 3, GL_SHORT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, esModelArray[id].cid);
    glVertexAttribPointer(color_id, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, esModelArray[id].iid);
    glDrawElements(GL_TRIANGLES, esModelArray[id].ni, esModelArray[id].itp, 0);
}
void mIdent(mat *m)
{
    memset(m, 0x0, sizeof(mat));
    m->m[0][0] = 1.0f;
    m->m[1][1] = 1.0f;
    m->m[2][2] = 1.0f;
    m->m[3][3] = 1.0f;
}
void mMul(mat *r, const mat *a, const mat *b)
{
    mat tmp;
    for(int i = 0; i < 4; i++)
    {
        tmp.m[i][0] =	(a->m[i][0] * b->m[0][0]) +
                        (a->m[i][1] * b->m[1][0]) +
                        (a->m[i][2] * b->m[2][0]) +
                        (a->m[i][3] * b->m[3][0]) ;
        tmp.m[i][1] =	(a->m[i][0] * b->m[0][1]) + 
                        (a->m[i][1] * b->m[1][1]) +
                        (a->m[i][2] * b->m[2][1]) +
                        (a->m[i][3] * b->m[3][1]) ;
        tmp.m[i][2] =	(a->m[i][0] * b->m[0][2]) + 
                        (a->m[i][1] * b->m[1][2]) +
                        (a->m[i][2] * b->m[2][2]) +
                        (a->m[i][3] * b->m[3][2]) ;
        tmp.m[i][3] =	(a->m[i][0] * b->m[0][3]) + 
                        (a->m[i][1] * b->m[1][3]) +
                        (a->m[i][2] * b->m[2][3]) +
                        (a->m[i][3] * b->m[3][3]) ;
    }
    memcpy(r, &tmp, sizeof(mat));
}
void mFrustum(mat *r, const float left, const float right, const float bottom, const float top, const float nearZ, const float farZ)
{
    const float dX = right - left;
    const float dY = top - bottom;
    const float dZ = farZ - nearZ;
    const float rdX = 1.f/dX;
    const float rdY = 1.f/dY;
    const float rdZ = 1.f/dZ;
    mat frust;
    if(nearZ <= 0.0f || farZ <= 0.0f || dX <= 0.0f || dY <= 0.0f || dZ <= 0.0f){return;}
    frust.m[0][0] = 2.0f * nearZ * rdX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;
    frust.m[1][1] = 2.0f * nearZ * rdY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;
    frust.m[2][0] = (right + left) * rdX;
    frust.m[2][1] = (top + bottom) * rdY;
    frust.m[2][2] = -(nearZ + farZ) * rdZ;
    frust.m[2][3] = -1.0f;
    frust.m[3][2] = -2.0f * nearZ * farZ * rdZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;
    mMul(r, &frust, r);
}
void mPerspective(mat *r, const float fovy, const float aspect, const float nearZ, const float farZ)
{
    float frustumW, frustumH;
    frustumH = tanf(fovy * 0.00872664625f) * nearZ;
    frustumW = frustumH * aspect;
    mFrustum(r, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}
void mScale(mat *r, const float x, const float y, const float z)
{
    r->m[0][0] *= x;
    r->m[0][1] *= x;
    r->m[0][2] *= x;
    r->m[0][3] *= x;
    r->m[1][0] *= y;
    r->m[1][1] *= y;
    r->m[1][2] *= y;
    r->m[1][3] *= y;
    r->m[2][0] *= z;
    r->m[2][1] *= z;
    r->m[2][2] *= z;
    r->m[2][3] *= z;
}
void mScale1(mat *r, const float s){mScale(r, s, s, s);}
void mTranslate(mat *r, const float x, const float y, const float z)
{
    r->m[3][0] += (r->m[0][0] * x + r->m[1][0] * y + r->m[2][0] * z);
    r->m[3][1] += (r->m[0][1] * x + r->m[1][1] * y + r->m[2][1] * z);
    r->m[3][2] += (r->m[0][2] * x + r->m[1][2] * y + r->m[2][2] * z);
    r->m[3][3] += (r->m[0][3] * x + r->m[1][3] * y + r->m[2][3] * z);
}
void mSetPos(mat *r, const vec pos)
{
    r->m[3][0] = pos.x;
    r->m[3][1] = pos.y;
    r->m[3][2] = pos.z;
}
void mSetRotX(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);

    r->m[0][0] = c;
    r->m[0][1] = 0.f;
    r->m[0][2] = s;
    r->m[0][3] = 0.f;

    r->m[1][0] = 0.f;
    r->m[1][1] = 1.f;
    r->m[1][2] = 0.f;
    r->m[1][3] = 0.f;

    r->m[2][0] = -s;
    r->m[2][1] = 0.f;
    r->m[2][2] = c;
    r->m[2][3] = 0.f;
}
void mSetRotY(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);

    r->m[0][0] = 1.f;
    r->m[0][1] = 0.f;
    r->m[0][2] = 0.f;
    r->m[0][3] = 0.f;

    r->m[1][0] = 0.f;
    r->m[1][1] = c;
    r->m[1][2] = -s;
    r->m[1][3] = 0.f;

    r->m[2][0] = 0.f;
    r->m[2][1] = s;
    r->m[2][2] = c;
    r->m[2][3] = 0.f;
}
void mSetRotZ(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);

    r->m[0][0] = c;
    r->m[0][1] = -s;
    r->m[0][2] = 0.f;
    r->m[0][3] = 0.f;

    r->m[1][0] = s;
    r->m[1][1] = c;
    r->m[1][2] = 0.f;
    r->m[1][3] = 0.f;

    r->m[2][0] = 0.f;
    r->m[2][1] = 0.f;
    r->m[2][2] = 1.f;
    r->m[2][3] = 0.f;
}
void mRotX(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);
    const mat t = { c, 0.f, s, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    -s, 0.f, c, 0.f,
                    0.f, 0.f, 0.f, 1.f };
    mMul(r, &t, r);
}
void mRotY(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);
    const mat t = { 1.f, 0.f, 0.f, 0.f,
                    0.f, c, -s, 0.f,
                    0.f, s, c, 0.f,
                    0.f, 0.f, 0.f, 1.f };
    mMul(r, &t, r);
}
void mRotZ(mat *r, const float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);
    const mat t = { c, -s, 0.f, 0.f,
                    s, c, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f };
    mMul(r, &t, r);
}
void mGetDirY(vec *r, const mat matrix)
{
    r->x = matrix.m[1][0];
    r->y = matrix.m[1][1];
    r->z = matrix.m[1][2];
}
mat projection, view, model, modelview;
#define updateModelView() mMul(&modelview,&model,&view);glUniformMatrix4fv(modelview_id,1,GL_FALSE,(float*)&modelview.m[0][0])
#define toView() glUniformMatrix4fv(modelview_id,1,GL_FALSE,(float*)&view.m[0][0])
#define toModelView() glUniformMatrix4fv(modelview_id,1,GL_FALSE,(float*)&modelview.m[0][0])
const GLchar* v1 = // vertex shader
    "#version 100\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modelview;\n"
    "uniform float opacity;\n"
    "uniform float ambient;\n"
    "attribute vec4 position;\n"
    "attribute vec3 color;\n"
    "varying vec3 vertPos;\n"
    "varying vec3 vertCol;\n"
    "varying float vertOpa;\n"
    "varying float vertAmb;\n"
    "void main()\n"
    "{\n"
        "vec4 vertPos4 = modelview * position;\n"
        "vertPos = vertPos4.xyz / vertPos4.w;\n"
        "vertCol = color;\n"
        "vertOpa = opacity;\n"
        "vertAmb = ambient;\n"
        "gl_Position = projection * vertPos4;\n"
    "}\n";
const GLchar* f1 = // fragment shader
    "#version 100\n"
    "#extension GL_OES_standard_derivatives : enable\n"
    "precision highp float;\n"
    "varying vec3 vertPos;\n"
    "varying vec3 vertCol;\n"
    "varying float vertOpa;\n"
    "varying float vertAmb;\n"
    "void main()\n"
    "{\n"
        "vec3 normal = normalize(cross(dFdx(vertPos), dFdy(vertPos)));\n"
        "vec3 absN = abs(normal);\n"
        "vec3 axisColor = (vertCol*0.95) * absN.x + vertCol * absN.y + (vertCol*0.975) * absN.z;\n"
        "float lambertian = clamp(max(dot(normalize(-vertPos), normal), 0.0), 0.64, 1.0);\n"
        "gl_FragColor = vec4((lambertian*axisColor*vertAmb), vertOpa);\n"
    "}\n";
void makeShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v1, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f1, NULL);
    glCompileShader(fragmentShader);
    GLuint shader = glCreateProgram();
        glAttachShader(shader, vertexShader);
        glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    position_id   = glGetAttribLocation(shader,  "position");
    color_id      = glGetAttribLocation(shader,  "color");
    projection_id = glGetUniformLocation(shader, "projection");
    modelview_id  = glGetUniformLocation(shader, "modelview");
    opacity_id    = glGetUniformLocation(shader, "opacity");
    ambient_id    = glGetUniformLocation(shader, "ambient");
    glUseProgram(shader);
}

//*************************************
// load models
//*************************************
#include "mdl/scene.h"  //0
#include "mdl/glass.h"  //1
#include "mdl/water.h"  //2
#include "mdl/hamster.h"//3
#include "mdl/hamster_half1.h"//4
#include "mdl/hamster_half2.h"//5
#include "mdl/blender_rotor.h"//6
#include "mdl/blender_blood.h"//7
#include "mdl/microwave_plate.h"//8
#include "mdl/microwave_blood.h"//9
#include "mdl/chopping_blood.h" //10
#include "mdl/trash_blood.h"//11
#include "mdl/cat_arm.h"    //12
#include "mdl/cat_body.h"   //13
#include "mdl/cat_head.h"   //14
#include "mdl/cat_mouth.h"  //15
#include "mdl/sci_arm.h"    //16
#include "mdl/sci_body.h"   //17
#include "mdl/sci_head.h"   //18
#include "mdl/sci_mouth.h"  //19
#include "mdl/theo_arm.h"   //20
#include "mdl/theo_body.h"  //21
#include "mdl/theo_head.h"  //22
#include "mdl/theo_mouth.h" //23
#include "mdl/karen_arm.h"  //24
#include "mdl/karen_body.h" //25
#include "mdl/karen_head.h" //26
#include "mdl/karen_mouth.h"//27
void register_models()
{
    register_scene();register_glass();register_water();register_hamster();
    register_hamster_half1();register_hamster_half2();register_blender_rotor();
    register_blender_blood();register_microwave_plate();register_microwave_blood();
    register_chopping_blood();register_trash_blood();register_cat_arm();
    register_cat_body();register_cat_head();register_cat_mouth();register_sci_arm();
    register_sci_body();register_sci_head();register_sci_mouth();register_theo_arm();
    register_theo_body();register_theo_head();register_theo_mouth();register_karen_arm();
    register_karen_body();register_karen_head();register_karen_mouth();
}

//*************************************
// globals
//*************************************
SDL_Window* wnd;
SDL_GLContext glc;
uint winw = 1024, winh = 768;
float aspect=0.f, t=0.f, lt=0.f, dt=0.f;

// archive head
uint head = 0;
#ifdef PLAY_RANDOM
uint max_head = 0;
#endif

// camera vars
int mx=0, my=0, lx=0, ly=0, md=0, md2=0, w2=0.f;
uint istouch = 0;
float sens = 0.003f;
float msens = 0.001f;
float xrot = 0.4f;
float yrot = 1.1f;
float dzoom = -2.f;
float zoom = -1.2f;
float cx=0.f, cy=0.f;

// actions
float blend = 0.f;
float micro = 0.f;
float sink = 0.f;
float chop = 0.f;
float toast = 0.f;
float wash = 0.f;
float trash = 0.f;

// sitting positions
const vec s1 = (vec){ 0.112f, 0.5f , -0.07f }; // catgirl
const vec s2 = (vec){ 0.71f, -0.1f , -0.067f}; // scientist
const vec s3 = (vec){ 0.71f,  0.35f, -0.07f }; // theorist
const vec s4 = (vec){-0.11f, -0.25f, -0.07f }; // karen

// action positions
const vec a1 = (vec){-0.6749094128608704f,  0.6767066717147827f , -0.07f}; // microwave
const vec a2 = (vec){-0.6432435512542725f,  0.413552463054657f  , -0.07f}; // sink
const vec a3 = (vec){-0.6432435512542725f,  0.1133602038025856f , -0.07f}; // chopping board
const vec a4 = (vec){-0.6610631942749023f, -0.08946600556373596f, -0.07f}; // blender
const vec a5 = (vec){-0.6736421585083008f, -0.21761448681354523f, -0.07f}; // toaster
const vec a6 = (vec){-0.721359f          , -0.5129505395889282f , -0.07f}; // washing machines
const vec a7 = (vec){-0.7842308878898621f, -0.7341938614845276f , -0.07f}; // trash can

// network data & emotes
uint vemo=0,vemoi=0,isact=0,msgskip=0;
float nwait = 0.f, nnt = 0.f;

// talking
uint t1=0, t2=0, t3=0, t4=0;
void clearTalking(){t1=0, t2=0, t3=0, t4=0;}
void setTalking(const uint u)
{
    clearTalking();
    /**/ if(u == 12){t1 = 1;}
    else if(u == 16){t2 = 1;}
    else if(u == 20){t3 = 1;}
    else if(u == 24){t4 = 1;}
}

// current action
uint cid = 0;
vec cs, ce;
void setCA(const uint u, const vec ep)
{
    cid = u;
    ce = ep;
    /**/ if(u == 12){cs = s1;}
    else if(u == 16){cs = s2;}
    else if(u == 20){cs = s3;}
    else if(u == 24){cs = s4;}
    isact = 1;
}
void doBlend(const uint c){blend=t+6.f;setCA(c, a4);}
void doMicro(const uint c){micro=t+6.f;setCA(c, a1);}
void doSink (const uint c){sink =t+6.f;setCA(c, a2);}
void doChop (const uint c){chop =t+6.f;setCA(c, a3);}
void doToast(const uint c){toast=t+6.f;setCA(c, a5);}
void doWash (const uint c){wash =t+6.f;setCA(c, a6);}
void doTrash(const uint c){trash=t+6.f;setCA(c, a7);}

//*************************************
// utility functions
//*************************************
void doPerspective()
{
    glViewport(0, 0, winw, winh);
    aspect = (float)winw / (float)winh;
    w2 = winw/2;
    mIdent(&projection);
    mPerspective(&projection, 60.0f, aspect, 0.1f, 6.f);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (float*)&projection.m[0][0]);
}
#ifdef WEB
EM_BOOL emscripten_resize_event(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    winw = uiEvent->documentBodyClientWidth, winh = uiEvent->documentBodyClientHeight;
    doPerspective();
    emscripten_set_canvas_element_size("canvas", winw, winh);
    return EM_FALSE;
}
EM_BOOL emscripten_mousemove_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    mx = mouseEvent->clientX, my = mouseEvent->clientY;
    return EM_FALSE;
}
#endif
void rCharSit(const uint i, const vec pos, const float scale, const float mouth, const float mouth_dist, const float mouth_height, const float rot, const uint talking)
{
    mat gr;
    mIdent(&gr);
    mSetRotZ(&gr, rot);

    // body
    mIdent(&model);
    if(i == 16){mSetPos(&model, (vec){pos.x, pos.y, pos.z-0.035f});}
    /****/ else{mSetPos(&model, (vec){pos.x, pos.y, pos.z-0.041f});}
    mMul(&model, &gr, &model);
    mRotY(&model, 1.16f);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+1);

    // arms
    const vec ap = (vec){pos.x, pos.y, pos.z-0.007f};
    float ar = 0.3f;
    mat r1;
    if(vemoi == i && vemo == 2)
    {
        ar = d2PI+sinf(t*12.f)*0.16f;
        mIdent(&r1);
        mSetRotY(&r1, ar);
    }
    else
    {
        mIdent(&r1);
        mSetRotY(&r1, ar);
    }

    esBindModel(i);

    mIdent(&model);
    mSetPos(&model, ap);
    mMul(&model, &gr, &model);
    mMul(&model, &r1, &model);
    mScale1(&model, scale);
    updateModelView();
    esRenderModel();

    mIdent(&model);
    mSetPos(&model, ap);
    mMul(&model, &gr, &model);
    if(vemoi == i && vemo == 2){mMul(&model, &r1, &model);mRotY(&model, ar*0.3f);}else{mMul(&model, &r1, &model);}
    mScale(&model, -scale, scale, scale);
    updateModelView();
    esRenderModel();

    // head
    static float c1 = 0.f, c2 = 0.f, c3 = 0.f, c4 = 0.f;
    float cr;
    if(talking == 1)
    {
        /**/ if(i == 12){c1 += dt;}
        else if(i == 16){c2 += dt;}
        else if(i == 20){c3 += dt;}
        else if(i == 24){c4 += dt;}
    }
    /**/ if(i == 12){cr = sinf(c1)*0.3f;}
    else if(i == 16){cr = sinf(c2)*0.3f;}
    else if(i == 20){cr = sinf(c3)*0.3f;}
    else if(i == 24){cr = sinf(c4)*0.3f;}
    mIdent(&r1);
    mSetRotZ(&r1, cr);

    if(vemoi == i)
    {
        if(vemo == 1 || vemo == 2){mRotY(&r1, 0.4f+sinf(t*9.f)*0.16f);}
        /*****/ else if(vemo == 3){mRotY(&r1, sinf(t*7.f)*0.16f);}
        /*****/ else if(vemo == 4){mRotZ(&r1, sinf(t*7.f)*0.16f);}
    }
    
    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mMul(&model, &r1, &model);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+2); // head

    mTranslate(&model, 0.f, -mouth_dist, mouth_height);
    if(talking == 1)
    {
        if(vemoi == i && vemo == 1)
        {
            mScale(&model, 1.f, 1.f, (1.f-mouth)+(mouth*3.f*fabsf(sinf(t*5.f))));
        }
        else
        {
            mScale(&model, 1.f, 1.f, (1.f-mouth)+(mouth*fabsf(sinf(t*5.f))));
        }
    }
    updateModelView();
    esBindRender(i+3); // mouth
}
void rCharStand(const uint i, const vec pos, const float scale, const float mouth, const float mouth_dist, const float mouth_height, const float rot, const uint talking)
{
    mat gr;
    mIdent(&gr);
    mSetRotZ(&gr, rot);

    // body
    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+1);

    // arms
    static float c1 = 0.f, c2 = 0.f, c3 = 0.f, c4 = 0.f;
    float cr;
    if(talking == 1)
    {
        /**/ if(i == 12){c1 += dt;}
        else if(i == 16){c2 += dt;}
        else if(i == 20){c3 += dt;}
        else if(i == 24){c4 += dt;}
    }
    /**/ if(i == 12){cr = sinf(c1)*0.3f;}
    else if(i == 16){cr = sinf(c2)*0.3f;}
    else if(i == 20){cr = sinf(c3)*0.3f;}
    else if(i == 24){cr = sinf(c4)*0.3f;}

    const vec ap = (vec){pos.x, pos.y, pos.z-0.007f};
    float ar = 0.3f;
    mat r1;
    if(vemoi == i && vemo == 2)
    {
        ar = 1.1f+sinf(t*12.f)*0.16f;
        mIdent(&r1);
        mSetRotZ(&r1, cr);
        mRotY(&r1, ar);
        mRotX(&r1, 1.1f-ar);
    }
    
    esBindModel(i);
    mIdent(&model);
    if(vemoi == i && i == 20 && vemo == 2){mSetPos(&model, (vec){pos.x, pos.y, pos.z-0.03f});}else{mSetPos(&model, pos);}
    mMul(&model, &gr, &model);
    if(vemoi == i && vemo == 2){mMul(&model, &r1, &model);}
    mScale1(&model, scale);
    updateModelView();
    esRenderModel();

    mIdent(&model);
    if(vemoi == i && i == 20 && vemo == 2){mSetPos(&model, (vec){pos.x, pos.y, pos.z-0.03f});}else{mSetPos(&model, pos);}
    mMul(&model, &gr, &model);
    if(vemoi == i && vemo == 2){mMul(&model, &r1, &model);mRotY(&model, ar*0.3f);}
    mScale(&model, -scale, scale, scale);
    updateModelView();
    esRenderModel();

    // head
    mIdent(&r1);
    mSetRotZ(&r1, cr);

    if(vemoi == i)
    {
        if(vemo == 1 || vemo == 2){mRotY(&r1, 0.4f+sinf(t*9.f)*0.16f);}
        /*****/ else if(vemo == 3){mRotY(&r1, sinf(t*7.f)*0.16f);}
        /*****/ else if(vemo == 4){mRotZ(&r1, sinf(t*7.f)*0.16f);}
    }

    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mMul(&model, &r1, &model);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+2); // head

    mTranslate(&model, 0.f, -mouth_dist, mouth_height);
    if(talking == 1)
    {
        if(vemoi == i && vemo == 1)
        {
            mScale(&model, 1.f, 1.f, (1.f-mouth)+(mouth*3.f*fabsf(sinf(t*5.f))));
        }
        else
        {
            mScale(&model, 1.f, 1.f, (1.f-mouth)+(mouth*fabsf(sinf(t*5.f))));
        }
    }
    updateModelView();
    esBindRender(i+3); // mouth
}
void rCharJump(const uint i, const vec pos, const float scale, const float rot1, const float rot2)
{
    mat gr;
    mIdent(&gr);
    mSetRotZ(&gr, rot1);
    mRotY(&gr, -rot2);

    // body
    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+1);

    // arms
    esBindModel(i);
    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mScale1(&model, scale);
    updateModelView();
    esRenderModel();

    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mScale(&model, -scale, scale, scale);
    updateModelView();
    esRenderModel();

    // head
    mIdent(&model);
    mSetPos(&model, pos);
    mMul(&model, &gr, &model);
    mScale1(&model, scale);
    updateModelView();
    esBindRender(i+2);
}
void rStand(const uint c, const vec p)
{
    /**/ if(c == 12){rCharStand(12, p, 0.17f, 0.5f , 0.326f, 0.284f, d2PI, t1);}
    else if(c == 16){rCharStand(16, p, 0.11f, 0.4f , 0.45f , 0.46f , d2PI, t2);}
    else if(c == 20){rCharStand(20, p, 0.23f, 0.65f, 0.25f , 0.176f, d2PI, t3);}
    else if(c == 24){rCharStand(24, p, 0.11f, 0.45f, 0.42f , 0.39f , d2PI, t4);}
}
void rJump(const uint c, const vec p, const float rot2)
{
    /**/ if(c == 12){rCharJump(12, p, 0.17f, d2PI, rot2);}
    else if(c == 16){rCharJump(16, p, 0.11f, d2PI, rot2);}
    else if(c == 20){rCharJump(20, p, 0.23f, d2PI, rot2);}
    else if(c == 24){rCharJump(24, p, 0.11f, d2PI, rot2);}
}
void doJump(const float bd)
{
    if(bd < 0.3f)
    {
        const float nbd = bd*3.333333f;
        vec lp, pd;
        pd.x = ce.x - cs.x;
        pd.y = ce.y - cs.y;
        lp = cs;
        lp.x += pd.x * nbd;
        lp.y += pd.y * nbd;
        lp.z *= -4.f*fabsf(sinf(bd*20.93333333f));
        rJump(cid, lp, x2PI*nbd);
    }
    else if(bd < 5.7f){rStand(cid, ce);}
    else if(bd >= 5.7f && bd < 6.f)
    {
        const float nbd = (bd-5.7f)*3.333333f;
        vec lp, pd;
        pd.x = cs.x - ce.x;
        pd.y = cs.y - ce.y;
        lp = ce;
        lp.x += pd.x * nbd;
        lp.y += pd.y * nbd;
        lp.z *= -4.f*fabsf(sinf((bd-5.7f)*20.93333333f));
        rJump(cid, lp, -x2PI*nbd);
    }
    else{cid = 0;}
}

//*************************************
// network data
//*************************************
#ifdef WEB
EMSCRIPTEN_KEEPALIVE void reloadSpeech()
{
    msgskip = 1;
    head--;
    nnt = 0.1f;
    nwait = 0.f;
}
void updateURL()
{
    EM_ASM({
        var v = $0;
        history.replaceState({}, "", "?" + v);
    }, head-1);
}
EM_JS(int, get_url_head, (), {
    try {
        var q = window.location.search.substring(1);
        var value = parseInt(q);
        return isNaN(value) ? -1 : Math.max(value, 0);
    }catch(e){return 0;}
});
// EM_ASYNC_JS(int, get_max_head, (), {
//     try {
//         let res = await fetch('/archive/head.txt');
//         let text = await res.text();
//         return Math.max(parseInt(text.trim()) || 0, 0);
//     }catch(e){return 0;}
// });
// EM_ASYNC_JS(int, get_head, (), {
//     try {
//         var q = window.location.search.substring(1);
//         var value = parseInt(q);
//         if(!isNaN(value)){return Math.max(value, 0);}
//         let res = await fetch('/archive/head.txt');
//         let text = await res.text();
//         value = Math.max(parseInt(text.trim()) || 0, 0);
//         return (value > 100) ? (value - 100) : 0;
//     }catch(e){return 0;}
// });
EM_JS(int, get_max_head, (), {
    try {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/archive/head.txt', false);
        xhr.send(null);
        if (xhr.status === 200) {
            var text = xhr.responseText;
            return Math.max(parseInt(text.trim()) || 0, 0);
        } else {
            return 0;
        }
    } catch(e) {
        return 0;
    }
});
EM_JS(int, get_head, (), {
    try {
        var q = window.location.search.substring(1);
        var value = parseInt(q);
        if (!isNaN(value)) return Math.max(value, 0);

        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/archive/head.txt', false);
        xhr.send(null);

        if (xhr.status === 200) {
            var text = xhr.responseText;
            value = Math.max(parseInt(text.trim()) || 0, 0);
            return (value > 100) ? (value - 100) : 0;
        } else {
            return 0;
        }
    } catch(e) {
        return 0;
    }
});
EM_JS(int, is_audio_playing, (), {if(window.voiceAudio && !window.voiceAudio.paused && !window.voiceAudio.ended){return 1;}else{return 0;}});
EM_JS(float, get_audiolen, (), {return window.audiolen;});
EM_JS(float, get_audio_timeleft, (), {if(window.voiceAudio && !window.voiceAudio.paused && !window.voiceAudio.ended){return window.voiceAudio.duration-window.voiceAudio.currentTime;}else{return 0;}});
EM_JS(uint32_t, secure_random_uint, (),
{
    var array = new Uint32Array(1);
    crypto.getRandomValues(array);
    return array[0];
});
GLfloat esRandFloat(const GLfloat min, const GLfloat max)
{
    static GLfloat rrndmax = 1.f/(GLfloat)RAND_MAX;
    return (((GLfloat)rand()) * rrndmax) * (max-min) + min;
}
GLuint esRand(const GLuint min, const GLuint max)
{
    return (GLuint)roundf(esRandFloat((GLfloat)min, (GLfloat)max));
}
void addChat(const uint i, const char* msg)
{
    if(msgskip == 1)
    {
        msgskip = 0;
        return;
    }
    char t[1024];
    /**/ if(i == 12){snprintf(t, sizeof(t), "addChat(\"<b style='color:#ff99ff'>&lt;Catgirl&gt;</b> %s\")", msg);}
    else if(i == 16){snprintf(t, sizeof(t), "addChat(\"<b style='color:#1cffff'>&lt;Scientist&gt;</b> %s\")", msg);}
    else if(i == 20){snprintf(t, sizeof(t), "addChat(\"<b style='color:#ffd381'>&lt;Theorist&gt;</b> %s\")", msg);}
    else if(i == 24){snprintf(t, sizeof(t), "addChat(\"<b style='color:#ff8ea1'>&lt;Karen&gt;</b> %s\")", msg);}
    emscripten_run_script(t);
}
void get_data_callback(void* user_data, void* buff, int size)
{
    // check for any errors in the data
    if(size < 8){return;}
    if(((char*)buff)[1] != '|'){return;}

    // let's go
    cid = 0; // new sequence so reset back to sitting position
    isact = 0; // reset action state
    nwait = t+3.f; // set how long we are willing to wait for audio to load

    // null terminate the buffer data for sscanf
    char temp[2048];
    if((size_t)size >= sizeof(temp)){size = sizeof(temp) - 1;}
    memcpy(temp, buff, size);
    temp[size] = '\0';

    // parsed strings (i should have passed vact and vemo as integer ids)
    uint vcid=0,vlen=0; // vlen no longer used
    char vmsg[512] = {0};
    char vact[32] = {0};
    char vemos[16] = {0};

    // parse data
    sscanf(temp, "%d|%d|%31[^|]|%15[^|]|%511[^|]", &vcid, &vlen, vact, vemos, vmsg);
    vcid = (vcid*4)+12; // convert 0-3 id to 12-24
    for(size_t i = 0; vmsg[i]; i++){if(vmsg[i] == '"' || vmsg[i] == '<' || vmsg[i] == '>'){vmsg[i] = '\'';}} // some basic escaping to prevent potential prompt injection

    // update the html chatlog
    /**/ if(vcid == 12){printf("Catgirl: %s\n"  , vmsg);addChat(vcid, vmsg);}
    else if(vcid == 16){printf("Scientist: %s\n", vmsg);addChat(vcid, vmsg);}
    else if(vcid == 20){printf("Theorist: %s\n" , vmsg);addChat(vcid, vmsg);}
    else if(vcid == 24){printf("Karen: %s\n"    , vmsg);addChat(vcid, vmsg);}

    // set emote
    /**/ if(strcmp(vemos, "laugh") == 0){vemo=1;vemoi=vcid;}
    else if(strcmp(vemos, "dance") == 0){vemo=2;vemoi=vcid;}
    else if(strcmp(vemos, "yes"  ) == 0){vemo=3;vemoi=vcid;}
    else if(strcmp(vemos, "no"   ) == 0){vemo=4;vemoi=vcid;}

    // do action
    /**/ if(strcmp(vact, "washingmachine") == 0){doWash (vcid);}
    else if(strcmp(vact, "microwave"     ) == 0){doMicro(vcid);}
    else if(strcmp(vact, "sink"          ) == 0){doSink (vcid);}
    else if(strcmp(vact, "choppingboard" ) == 0){doChop (vcid);}
    else if(strcmp(vact, "blender"       ) == 0){doBlend(vcid);}
    else if(strcmp(vact, "toaster"       ) == 0){doToast(vcid);}
    else if(strcmp(vact, "trashcan"      ) == 0){doTrash(vcid);}

    // play audio / do talking
    EM_ASM({
        var vhead = $0;
        if(!window.voiceAudio){window.voiceAudio = new Audio();}
        window.audiolen = undefined;
        var audio = window.voiceAudio;
        audio.src = '/archive/v' + vhead + '.mp3?' + Date.now();
        audio.load();
        audio.addEventListener('loadedmetadata', function(){window.audiolen = audio.duration;}, {once: true});
        audio.play().then(() => {document.getElementById('playContainer').style.display = 'none';}).catch(err => {document.getElementById('playContainer').style.display = 'block';});
    }, head);

    // start the character talking
    setTalking(vcid);

    // increment the head
    head++;
    if(head >= max_head){head = esRand(0, max_head-32);}
    updateURL();
}
#endif

//*************************************
// update & render
//*************************************
void main_loop()
{
    // time delta
    t = ((float)SDL_GetTicks())*0.001f, dt = t-lt, lt = t;

    // input
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_MULTIGESTURE)
        {
            if(event.mgesture.numFingers == 2)
            {
                lx=mx,ly=my;
                md = 0, md2 = 0;
                /**/ if(event.mgesture.dDist < 0){zoom += 0.09f * zoom;}
                else if(event.mgesture.dDist > 0){zoom -= 0.09f * zoom;}
                if(zoom > -0.33f){zoom = -0.33f;}else if(zoom < -3.f){zoom = -3.f;}
            }
        }
        else if(event.type == SDL_FINGERDOWN)
        {
            if(istouch == 0){istouch = 1;}
            mx = event.tfinger.x*winw, my = event.tfinger.y*winh, lx=mx, ly=my;
            sens = 0.006f;
            md = 2;
        }
        else if(event.type == SDL_FINGERUP)
        {
            md = 0;
        }
        else if(event.type == SDL_FINGERMOTION)
        {
            if(md > 0){mx = event.tfinger.x*winw, my = event.tfinger.y*winh;}
        }
        else if(event.type == SDL_MOUSEWHEEL)
        {
            /**/ if(event.wheel.y < 0){zoom += 0.09f * zoom;}
            else if(event.wheel.y > 0){zoom -= 0.09f * zoom;}
            if(zoom > -0.33f){zoom = -0.33f;}else if(zoom < -3.f){zoom = -3.f;}
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            lx=mx,ly=my;
            /**/ if(event.button.button == SDL_BUTTON_LEFT  ){md2 = 1;}
            else if(event.button.button == SDL_BUTTON_RIGHT ){md  = 1;}
            else if(event.button.button == SDL_BUTTON_MIDDLE){md  = 1;}
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            /**/ if(event.button.button == SDL_BUTTON_LEFT  ){md2 = 0;}
            else if(event.button.button == SDL_BUTTON_RIGHT ){md  = 0;}
            else if(event.button.button == SDL_BUTTON_MIDDLE){md  = 0;}
        }
#ifndef WEB // no effect in browsers
        else if(event.type == SDL_KEYDOWN) // debug
        {
            const SDL_Keycode key = event.key.keysym.sym;
            const SDL_Keycode scan = event.key.keysym.scancode;
            if(     key == SDLK_1){doBlend(12);}
            else if(key == SDLK_2){doMicro(16);}
            else if(key == SDLK_3){doSink(20); }
            else if(key == SDLK_4){doChop(24); }
            else if(key == SDLK_5){doToast(12);}
            else if(key == SDLK_6){doWash(16); }
            else if(key == SDLK_7){doTrash(20);}
            else if(key == SDLK_8){vemo=2;vemoi=12;t1=1;}
            else if(key == SDLK_9){vemo=2;vemoi=20;t3=1;}
            else if(key == SDLK_0){vemo=3;vemoi=16;t2=1;}
        }
        else if(event.type == SDL_MOUSEMOTION)
        {
            mx = event.motion.x, my = event.motion.y;
        }
        else if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                winw = event.window.data1, winh = event.window.data2;
                doPerspective();
            }
        }
        else if(event.type == SDL_QUIT)
        {
            SDL_GL_DeleteContext(glc);
            SDL_DestroyWindow(wnd);
            SDL_Quit();
            exit(0);
        }
#endif
    }

#ifdef WEB
    // check for new speech
    if(t > nnt)
    {
        // not waiting on audio to load?
        if(t > nwait)
        {
            // reset
            clearTalking();
            if(vemo != 0){vemo = 0;} // reset emote; time is up

            // check for new lines if no audio is currently playing
            if(is_audio_playing() == 0)
            {
                char fn[256];
                snprintf(fn, sizeof(fn), "/archive/l%u.txt?t=%lld", head, time(0));
                emscripten_async_wget_data(fn, NULL, get_data_callback, NULL);
            }
            nnt = t+0.5f;
        }
        else
        {
            // this signals audio is loaded
            const float audio_len = get_audiolen();
            if(audio_len > 0.f)
            {
                // reset audio len (we know the audio is loaded now)
                EM_ASM({window.audiolen = undefined;});
                nwait = 0.f;

                // set a wait based on how much time is left
                const float audio_timeleft = get_audio_timeleft();
                if(audio_timeleft > 0.f)
                {
                    if(isact == 1 && audio_len < 6.f){nnt = t+(6.f-(audio_len-audio_timeleft));}else{nnt = t+audio_timeleft;}
                }
                else{nnt = t+audio_len;} // this ensures that if audio play in browser (security) is disabled it will still wait a fair amount of time between each line
            }
            else{nnt = t+0.1f;}
        }
    }
#endif
    
    // camera
    mIdent(&view);
    if(dt < 0.1f) // avoids big dt bug on game start (also means if you get 10 FPS or less = no camera)
    {
        dzoom += (zoom-dzoom)*3.f*dt;
        if(istouch == 1)
        {
            if(mx > w2)
            {
                xrot += (lx-mx)*sens;
                yrot += (ly-my)*sens;
                if(yrot > 1.6f){yrot = 1.6f;}else if(yrot < 0.f){yrot = 0.f;}
                lx = mx, ly = my;
            }
            mSetPos(&view, (vec){0.f, 0.f, dzoom});
            mSetRotY(&view, yrot);
            mRotZ(&view, xrot);
            if(mx < w2)
            {
                const float cos1 = cosf(xrot), sin1 = sinf(xrot);
                const float dx = (lx-mx)*sens, dy = -(ly-my)*sens;
                cx += dx*cos1 - dy*sin1;
                cy += dx*sin1 + dy*cos1;
                if(cx < -0.9f){cx = -0.9f;}else if(cx > 0.9f){cx = 0.9f;}
                if(cy < -0.8f){cy = -0.8f;}else if(cy > 0.8f){cy = 0.8f;}
                lx = mx, ly = my;
            }
            mTranslate(&view, cx, cy, 0.f);
        }
        else
        {
            if(md == 1)
            {
                xrot += (lx-mx)*sens;
                yrot += (ly-my)*sens;
                if(yrot > 1.6f){yrot = 1.6f;}else if(yrot < 0.f){yrot = 0.f;}
            }
            mSetPos(&view, (vec){0.f, 0.f, dzoom});
            mSetRotY(&view, yrot);
            mRotZ(&view, xrot);
            if(md2 == 1)
            {
                const float cos1 = cosf(xrot), sin1 = sinf(xrot);
                const float dx = (lx-mx)*msens, dy = -(ly-my)*msens;
                cx += dx*cos1 - dy*sin1;
                cy += dx*sin1 + dy*cos1;
                if(cx < -0.9f){cx = -0.9f;}else if(cx > 0.9f){cx = 0.9f;}
                if(cy < -0.8f){cy = -0.8f;}else if(cy > 0.8f){cy = 0.8f;}
            }
            mTranslate(&view, cx, cy, 0.f);
            if(md == 1 || md2 == 1){lx = mx, ly = my;}
        }
    }

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render world
    toView();
    esBindRender(0); // scene

    uint actions_running = 7;

    // microwave
    if(micro > t)
    {
        const float bd = 6.f-(micro-t);
        toModelView();
        doJump(bd);

        if(bd >= 3.f)
        {
            toView();

            esBindRender(9); // microwave blood
            
            toModelView();

            // spining plate
            mIdent(&model);
            mSetPos(&model, (vec){-0.793158f, 0.681474f, -0.08111f});
            updateModelView();
            esBindRender(8);
        }
        else
        {
            toModelView();

            // spining plate
            mIdent(&model);
            mSetPos(&model, (vec){-0.793158f, 0.681474f, -0.08111f});
            mSetRotZ(&model, t*3.f);
            updateModelView();
            esBindRender(8);
            
            // hamster
            mIdent(&model);
            mSetPos(&model, (vec){-0.793158f, 0.681474f, -0.06f});
            mSetRotZ(&model, t*3.f);
            mScale1(&model, 0.02f);
            updateModelView();
            esBindRender(3);
        }
    }
    else
    {
        toModelView();

        // static plate
        mIdent(&model);
        mSetPos(&model, (vec){-0.793158f, 0.681474f, -0.08111f});
        updateModelView();
        esBindRender(8);

        actions_running--;
    }

    // characters
    toModelView();

    glUniform1f(ambient_id, 0.8f);
    if(cid != 12){rCharSit  (12, s1, 0.17f, 0.5f , 0.326f, 0.284f,  0.f , t1);}
    if(cid != 16){rCharSit  (16, s2, 0.11f, 0.4f , 0.45f , 0.46f ,  d2PI, t2);}
    if(cid != 20){rCharStand(20, s3, 0.23f, 0.65f, 0.25f , 0.176f,  0.9f, t3);}
    if(cid != 24){rCharStand(24, s4, 0.11f, 0.45f, 0.42f , 0.39f , -2.3f, t4);}
    glUniform1f(ambient_id, 1.f);

    // blender
    if(blend > t)
    {
        float bd = 6.f-(blend-t);
        doJump(bd);
        
        if(bd >= 1.f)
        {
            bd -= 1.f;

            // spining rotor
            mIdent(&model);
            mSetPos(&model, (vec){-0.777437f, -0.088362f, -0.077077f});
            mSetRotZ(&model, t*19.f);
            updateModelView();
            esBindRender(6);

            // hamster
            mIdent(&model);
            mSetPos(&model, (vec){-0.777437f, -0.088362f, -0.057f-(bd*0.01f)});
            mSetRotZ(&model, t*9.f);
            mScale1(&model, 0.01f);
            updateModelView();
            esBindRender(3);
        }
        else
        {
            // static rotor
            mIdent(&model);
            mSetPos(&model, (vec){-0.777437f, -0.088362f, -0.077077f});
            updateModelView();
            esBindRender(6);
        }
    }
    else
    {
        // static rotor
        mIdent(&model);
        mSetPos(&model, (vec){-0.777437f, -0.088362f, -0.077077f});
        updateModelView();
        esBindRender(6);

        actions_running--;
    }

    // sink
    if(sink > t)
    {
        static float d = 0.f;
        const float bd = 6.f-(sink-t);
        doJump(bd);

        if(bd >= 3.f)
        {
            mIdent(&model);
            d += dt;
            mSetPos(&model, (vec){-0.817487f, 0.37f, -0.097172f-((3.f-(sink-t))*0.007f)});
            mSetRotZ(&model, d*3.f);
            mScale1(&model, 0.02f);
            updateModelView();
            esBindRender(3);
        }
        else
        {
            d = 0.f;
            mIdent(&model);
            mSetPos(&model, (vec){-0.817487f, 0.46f-(bd*0.03f), -0.097172f});
            mScale1(&model, 0.02f);
            updateModelView();
            esBindRender(3);
        }
    }
    else{actions_running--;}

    // chop
    if(chop > t)
    {
        static float d = 0.f;
        const float bd = 6.f-(chop-t);
        doJump(bd);

        if(bd >= 3.f)
        {
            esBindModel(5);

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.09f, -0.08f});
            mSetRotX(&model, -d2PI);
            mRotY(&model, -PI);
            mScale1(&model, 0.02f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.13f, -0.08f});
            mSetRotX(&model, -d2PI);
            mRotY(&model, -PI);
            mRotZ(&model, PI);
            mScale(&model, -0.02f, 0.02f, 0.02f);
            updateModelView();
            esRenderModel();
        }
        else if(bd >= 2.f)
        {
            esBindModel(5);

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.10f, -0.08f});
            mSetRotX(&model, d2PI);
            mScale1(&model, 0.02f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.12f, -0.08f});
            mSetRotX(&model, d2PI);
            mRotZ(&model, PI);
            mScale(&model, -0.02f, 0.02f, 0.02f);
            updateModelView();
            esRenderModel();
        }
        else if(bd >= 1.f && bd < 2.f)
        {
            esBindModel(4);

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.103f, -0.063f});
            mSetRotZ(&model, -d2PI);
            mScale1(&model, 0.02f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.117f, -0.063f});
            mSetRotZ(&model, -d2PI);
            mScale(&model, -0.02f, 0.02f, 0.02f);
            updateModelView();
            esRenderModel();
        }
        else
        {
            d = 0.f;
            mIdent(&model);
            mSetPos(&model, (vec){-0.784052f, 0.11f, -0.063f});
            mSetRotZ(&model, -d2PI);
            mScale1(&model, 0.02f);
            updateModelView();
            esBindRender(3);
        }
    }
    else{actions_running--;}

    // toast
    if(toast > t)
    {
        const float bd = 6.f-(toast-t);
        doJump(bd);

        if(bd >= 3.f)
        {
            mIdent(&model);
            mSetPos(&model, (vec){-0.785213f, -0.217887f, -0.066854f+(sinf(3.f-(toast-t))*0.08f)});
            mSetRotX(&model, PI);
            mRotY(&model, -t*1.2f);
            mScale1(&model, 0.01f);
            updateModelView();
            esBindRender(3);
        }
        else if(bd >= 0.3f && bd < 3.f)
        {
            mIdent(&model);
            mSetPos(&model, (vec){-0.767367f, -0.217887f, -0.066854f+(sinf(bd)*0.08f)});
            mSetRotY(&model, -t*1.2f);
            mScale1(&model, 0.01f);
            updateModelView();
            esBindRender(3);
        }
    }
    else{actions_running--;}

    // wash
    if(wash > t)
    {
        const float bd = 6.f-(wash-t);
        doJump(bd);

        if(bd > 0.3f && bd < 5.7)
        {
            esBindModel(3);

            mIdent(&model);
            mSetPos(&model, (vec){-0.862915f, -0.437543f, 0.007f});
            mSetRotY(&model, -t*4.4f);
            mScale1(&model, 0.03f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.862915f, -0.58373f, 0.007f});
            mSetRotY(&model, t*4.4f);
            mScale1(&model, 0.03f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.862915f, -0.437543f, -0.14f});
            mSetRotX(&model, PI);
            mRotY(&model, -t*4.4f);
            mScale1(&model, 0.03f);
            updateModelView();
            esRenderModel();

            mIdent(&model);
            mSetPos(&model, (vec){-0.862915f, -0.58373f, -0.14f});
            mSetRotX(&model, PI);
            mRotY(&model, t*4.4f);
            mScale1(&model, 0.03f);
            updateModelView();
            esRenderModel();
        }
    }
    else{actions_running--;}

    // trash
    if(trash > t)
    {
        const float bd = 6.f-(trash-t);
        doJump(bd);
        
        if(bd >= 2.f && bd < 4.6f)
        {
            mIdent(&model);
            mSetPos(&model, (vec){-0.878552f, -0.734222f, -0.113925f});
            mSetRotZ(&model, -t*4.4f);
            mScale(&model, 1.f, 1.f, 1.f+fabsf(sinf(d2PI+bd)*9.f));
            updateModelView();
            esBindRender(11);
        }
        else if(bd >= 0.3f && bd < 2.f)
        {
            mIdent(&model);
            mSetPos(&model, (vec){-0.878552f, -0.734222f, -0.03252f-(bd*0.08f)});
            mSetRotZ(&model, d2PI);
            mRotY(&model, -t*4.4f);
            mScale1(&model, 0.02f);
            updateModelView();
            esBindRender(3);
        }
    }
    else{actions_running--;}

    if(actions_running == 0){cid=0;}

    // table hamsters
    mIdent(&model);
    mSetPos(&model, (vec){0.170532f, 0.018884f, -0.10102f});
    mScale1(&model, 0.017f);
    updateModelView();
    esBindRender(3);
    mIdent(&model);
    mSetPos(&model, (vec){0.256f, 0.04f, -0.10102f});
    mSetRotX(&model, 1.3);
    mScale1(&model, 0.013f);
    updateModelView();
    esBindRender(5);

    // transparent objects
    glEnable(GL_BLEND);

        toView();
        
        // blender
        if(blend > t)
        {
            const float bd = 6.f-(blend-t);
            if(bd > 1.f)
            {
                glUniform1f(opacity_id, bd-1.f);
                esBindRender(7); // blender blood
            }
        }

        // chop
        if(chop > t)
        {
            const float b = chop-t;
            const float bd = 6.f-b;
            if(bd > 1.f)
            {
                glUniform1f(opacity_id, (bd-1.f)*0.333f);
                esBindRender(10); // chopping blood
            }
        }

        // glass
        glUniform1f(opacity_id, 0.75f);
        esBindRender(1); // glass
        
        // sink water
        toModelView();
        mIdent(&model);
        mSetPos(&model, (vec){-0.817487f, 0.412384f, -0.097172f});
        mScale(&model, 1.f, 1.f, sinf(t*0.84f));
        updateModelView();
        esBindRender(2);

    glDisable(GL_BLEND);

    // swap buffer
    SDL_GL_SwapWindow(wnd);
}

//*************************************
// process entry point
//*************************************
int main(int argc, char** argv)
{
//*************************************
// setup render context / window
//*************************************
#ifdef WEB
    int msaa = 16;
#else
    int msaa = 0;
#endif
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0)
    {
        printf("ERROR: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }
#ifdef WEB
    double width, height;
    emscripten_get_element_css_size("body", &width, &height);
    winw = (Uint32)width, winh = (Uint32)height;
#endif
    if(msaa > 0)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    wnd = SDL_CreateWindow("Hamster Purgatory", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winw, winh, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    while(wnd == NULL)
    {
        msaa--;
        if(msaa == 0)
        {
            printf("ERROR: SDL_CreateWindow(): %s\n", SDL_GetError());
            return 1;
        }
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
        wnd = SDL_CreateWindow("Hamster Purgatory", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winw, winh, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    }
    SDL_GL_SetSwapInterval(1); // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync
    glc = SDL_GL_CreateContext(wnd);
    if(glc == NULL)
    {
        printf("ERROR: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

//*************************************
// bind vertex and index buffers
//*************************************
    register_models();

//*************************************
// configure render options
//*************************************
    makeShader();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.745f, 0.8863f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glUniform1f(opacity_id, 0.75f);
    glUniform1f(ambient_id, 1.f);
    doPerspective();

//*************************************
// execute update / render loop
//*************************************
#ifdef WEB
#ifdef PLAY_RANDOM
    srand(secure_random_uint());
    max_head = get_max_head();
    const int th = get_url_head();
    if(th == -1 || th >= max_head){head = esRand(0, max_head-32);}else{head=th;}
#else
    head = get_head();
#endif
    emscripten_run_script("document.getElementById('load').style.display='none'");
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, emscripten_resize_event);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, emscripten_mousemove_event);
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while(1){main_loop();}
#endif
    return 0;
}

