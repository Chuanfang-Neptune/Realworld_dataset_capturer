<<<<<<< HEAD
#ifndef _OPENGL_MATH_H_
#define _OPENGL_MATH_H_
=======
#ifndef OPENGL_MATH_H
#define OPENGL_MATH_H
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339


#include <cmath>
//vecter
typedef float Vector3f[3];                

//Assign values to the vector
inline void LoadVector3(Vector3f v, const float x, const float y, const float z)
{
    v[0] = x; v[1] = y; v[2] = z;
}

typedef float Matrix44f[16];         
//4*4 Identity matrix
inline void LoadIdentity44(Matrix44f m)
{
    m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f;  m[12] = 0.0f;
    m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f;  m[13] = 0.0f;
    m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}
//4*4 vecter Multiply
inline void MatrixMultiply44(Matrix44f product, const Matrix44f a, const Matrix44f b)
{
    unsigned int j, k;
    for (unsigned int i = 0; i < 16; i++) {
        j = i % 4;
        k = i / 4 * 4;
        product[i] = a[k] * b[j] + a[k+1] * b[j+4] + a[k+2] * b[j+8] + a[k+3] * b[j+12];
    }
}
inline float GetVectorLengthSquared3(const Vector3f u)
{
    return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}
// get vecter's length
inline float GetVectorLength3(const Vector3f u)
{
    return sqrtf(GetVectorLengthSquared3(u));
}
//vecter scale
inline void ScaleVector3(Vector3f v, const float scale)
{
    v[0] *= scale; v[1] *= scale; v[2] *= scale;
}
//vecter normalize
inline void NormalizeVector3(Vector3f u)
{
    ScaleVector3(u, 1.0f / GetVectorLength3(u));
}
//cross product
inline void CrossProduct3(Vector3f result, const Vector3f u, const Vector3f v)
{
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = u[2] * v[0] - v[2] * u[0];
    result[2] = u[0] * v[1] - v[0] * u[1];
}
//calculate camera matrix
inline void CameraMatrix44(Matrix44f m,Vector3f Target, Vector3f Up)
{
    Vector3f N,U,V,temp;
    for (unsigned int i = 0; i < 3; i++) {
        N[i] = Target[i];
        temp[i] = Up[i];
    }
    NormalizeVector3(N);
    CrossProduct3(U, temp, N); //U= Up * N
    NormalizeVector3(U);
    CrossProduct3(V, N, U); //V= N * U

    m[0] = U[0];   m[1] = U[1];   m[2] = U[2];    m[3] = 0.0f;
    m[4] = V[0];   m[5] = V[1];   m[6] = V[2];    m[7] = 0.0f;
    m[8] = N[0];   m[9] = N[1];   m[10] = N[2];   m[11] = 0.0f;
    m[12] = 0.0f;   m[13] = 0.0f;   m[14] = 0.0f;   m[15] = 1.0f;
}

#endif