#pragma once
#include "Tup.h"

// u1 = ratio, u2 = (1-ratio)
inline float lerp(float start, float end, float ratio) {return start*ratio + end*(1-ratio);}


/*
# barycentric coordinates notes:

sys of equations:
(u1, u2, u3)

v1 = (x1, y1, z3)
v2 = (x2, y2, z3)
v3 = (x3, y3, z3)
p = point in triangle
p = (xp, yp, z3)

u1 + u2 + u3 = 1

dot product of xs:
    u1*x1 + u2*x2 + u3*x3 = xp
dot product of ys:
    u1*y1 + u2*y2 + u3*y3 = xp
dot product of ys:
    u1*z1 + u2*z2 + u3*z3 = xp

sys of equations:
| u1 + u2 + u3 = 1
| u1*x1 + u2*x2 + u3*x3 = xp
| u1*y1 + u2*y2 + u3*y3 = xp

u3 = (1 - u1 - u2)

u1 + u2 + (1 - u1 - u2) = 1

| u1*(x1 - x3) + u2*(x2 - x3) = xp - x3
| u1*(y1 - y3) + u2*(y2 - y3) = xp - y3

T = mat2x2(
    (x1 - x3), (x2 - x3),
    (y1 - y3), (y2 - y3),
)

U = mat2x1(
    u1,
    u2
)

R = mat2x1(
    xp - x3
    yp - y3
)

T*U = R

U = inv(T)*R

inv(T) = adj(T) / det(T)

determinate:
det(T) = (x1 - x3)*(y2 - y3) - (x2 - x3)*(y1 - y3)
adjoint:
adj(T) = mat2x2(
    (y2 - y3), (x3 - x2),
    (y3 - y1), (x1 - x3),
)

inv(T) = mat2x2(
    (y2 - y3)/det(T), (x3 - x2)/det(T),
    (y3 - y1)/det(T), (x1 - x3)/det(T),
)

inv(T)*R = mat2x1(
    ((y2 - y3)*(xp - x3) + (x3 - x2)*(yp - y3))/det(T),
    ((y3 - y1)*(xp - x3) + (x1 - x3)*(yp - y3))/det(T),
)

det = (x1 - x3)*(y2 - y3) - (x2 - x3)*(y1 - y3)
u1 = ((y2 - y3)*(xp - x3) + (x3 - x2)*(yp - y3))/det
u2 = ((y3 - y1)*(xp - x3) + (x1 - x3)*(yp - y3))/det
u3 = 1 - u1 - u2

adj(mat2x2([a,b][c,d])) = mat2x2(
    d, -b
    -c, a
)
    
*/
inline tup<float, 3> barycentric_coords(float x1, float y1, float x2, float y2, float x3, float y3,
float xp, float yp) {
    float exp1 = xp - x3;
    float exp2 = yp - y3;
    float det = fmaf((x1 - x3), (y2 - y3), -(x2 - x3)*(y1 - y3));
    float u1 = fmaf((y2 - y3), (exp1), (x3 - x2)*(exp2))/det;
    float u2 = fmaf((y3 - y1), (exp1), (x1 - x3)*(exp2))/det;
    float u3 = 1 - u1 - u2;
    return make_tup<float, 3>({u1, u2, u3});
}

template <typename T>
inline void vec_extend(vector<T>& v, vector<T>& v_prime) {
  v.reserve(v.size() + distance(v_prime.begin(),v_prime.end()));
  v.insert(v.end(),v_prime.begin(),v_prime.end());
}

template <typename T>
inline T clamp(const T& value, const T& low, const T& high) {
  return value < low ? low : (value > high ? high : value); 
}