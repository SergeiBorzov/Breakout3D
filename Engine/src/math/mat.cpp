#include "mat.h"

namespace Math {
    Mat3::Mat3(const Mat4& m) {
        m_Data[0] = Math::Vec3(m[0]);
        m_Data[1] = Math::Vec3(m[1]);
        m_Data[2] = Math::Vec3(m[2]);
    }

    Mat4::Mat4(const Mat3& m) {
        m_Data[0] = Math::Vec4(m[0], 0.0f);
        m_Data[1] = Math::Vec4(m[1], 0.0f);
        m_Data[2] = Math::Vec4(m[2], 0.0f);
        m_Data[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    }

    Mat4 Inverse(const Mat4& mat) {
        f32 inv[16];
        const f32* m = mat.data();

        inv[0] =   m[5]  * m[10] * m[15] - 
                   m[5]  * m[11] * m[14] - 
                   m[9]  * m[6]  * m[15] + 
                   m[9]  * m[7]  * m[14] +
                   m[13] * m[6]  * m[11] - 
                   m[13] * m[7]  * m[10];

        inv[4] =  -m[4]  * m[10] * m[15] + 
                   m[4]  * m[11] * m[14] + 
                   m[8]  * m[6]  * m[15] - 
                   m[8]  * m[7]  * m[14] - 
                   m[12] * m[6]  * m[11] + 
                   m[12] * m[7]  * m[10];

        inv[8] =   m[4]  * m[9]  * m[15] - 
                   m[4]  * m[11] * m[13] - 
                   m[8]  * m[5]  * m[15] + 
                   m[8]  * m[7]  * m[13] + 
                   m[12] * m[5]  * m[11] - 
                   m[12] * m[7]  * m[9];

        inv[12] = -m[4]  * m[9]  * m[14] + 
                   m[4]  * m[10] * m[13] +
                   m[8]  * m[5]  * m[14] - 
                   m[8]  * m[6]  * m[13] - 
                   m[12] * m[5]  * m[10] + 
                   m[12] * m[6]  * m[9];

        inv[1] =  -m[1]  * m[10] * m[15] + 
                   m[1]  * m[11] * m[14] + 
                   m[9]  * m[2]  * m[15] - 
                   m[9]  * m[3]  * m[14] - 
                   m[13] * m[2]  * m[11] + 
                   m[13] * m[3]  * m[10];

        inv[5] =   m[0]  * m[10] * m[15] - 
                   m[0]  * m[11] * m[14] - 
                   m[8]  * m[2]  * m[15] + 
                   m[8]  * m[3]  * m[14] + 
                   m[12] * m[2]  * m[11] - 
                   m[12] * m[3]  * m[10];

        inv[9] =  -m[0]  * m[9]  * m[15] + 
                   m[0]  * m[11] * m[13] + 
                   m[8]  * m[1]  * m[15] - 
                   m[8]  * m[3]  * m[13] - 
                   m[12] * m[1]  * m[11] + 
                   m[12] * m[3]  * m[9];

        inv[13] =  m[0]  * m[9]  * m[14] - 
                   m[0]  * m[10] * m[13] - 
                   m[8]  * m[1]  * m[14] + 
                   m[8]  * m[2]  * m[13] + 
                   m[12] * m[1]  * m[10] - 
                   m[12] * m[2]  * m[9];

        inv[2] =   m[1]  * m[6] * m[15] - 
                   m[1]  * m[7] * m[14] - 
                   m[5]  * m[2] * m[15] + 
                   m[5]  * m[3] * m[14] + 
                   m[13] * m[2] * m[7]  - 
                   m[13] * m[3] * m[6];

        inv[6] =  -m[0]  * m[6] * m[15] + 
                   m[0]  * m[7] * m[14] + 
                   m[4]  * m[2] * m[15] - 
                   m[4]  * m[3] * m[14] - 
                   m[12] * m[2] * m[7]  + 
                   m[12] * m[3] * m[6];

        inv[10] =  m[0]  * m[5] * m[15] - 
                   m[0]  * m[7] * m[13] - 
                   m[4]  * m[1] * m[15] + 
                   m[4]  * m[3] * m[13] + 
                   m[12] * m[1] * m[7]  - 
                   m[12] * m[3] * m[5];

        inv[14] = -m[0]  * m[5] * m[14] + 
                   m[0]  * m[6] * m[13] + 
                   m[4]  * m[1] * m[14] - 
                   m[4]  * m[2] * m[13] - 
                   m[12] * m[1] * m[6]  + 
                   m[12] * m[2] * m[5];

        inv[3] =  -m[1] * m[6] * m[11] + 
                   m[1] * m[7] * m[10] + 
                   m[5] * m[2] * m[11] - 
                   m[5] * m[3] * m[10] - 
                   m[9] * m[2] * m[7]  + 
                   m[9] * m[3] * m[6];

        inv[7] =   m[0] * m[6] * m[11] - 
                   m[0] * m[7] * m[10] - 
                   m[4] * m[2] * m[11] + 
                   m[4] * m[3] * m[10] + 
                   m[8] * m[2] * m[7]  - 
                   m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] + 
                   m[0] * m[7] * m[9]  + 
                   m[4] * m[1] * m[11] - 
                   m[4] * m[3] * m[9]  - 
                   m[8] * m[1] * m[7]  + 
                   m[8] * m[3] * m[5];

        inv[15] =  m[0] * m[5] * m[10] - 
                   m[0] * m[6] * m[9]  - 
                   m[4] * m[1] * m[10] + 
                   m[4] * m[2] * m[9]  + 
                   m[8] * m[1] * m[6]  - 
                   m[8] * m[2] * m[5];

        f32 det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];

        if (det == 0)
            return false;

        det = 1.0f / det;

        for (int i = 0; i < 16; i++)
            inv[i] *= det;

        return Mat4(inv);
    }
}