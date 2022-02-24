#ifndef MATH_MAT_H
#define MATH_MAT_H
#include "../core/log.h"
#include "vec.h"

namespace Math {
    class Mat4;

    class Mat3 {
    public:
        Mat3() {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] = 0.0f;
            }
            data[0] = 1.0f;
            data[4] = 1.0f;
            data[8] = 1.0f;
        }
        
        Mat3(const f32* data) {
            f32* data_ptr = (f32*)m_Data;
            for (int i = 0; i < 9; i++) {
                data_ptr[i] = data[i];
            }
        }

        explicit Mat3(const Mat4& mat);

        Mat3(const Math::Vec3& x, const Math::Vec3& y, const Math::Vec3& z) {
            m_Data[0] = x;
            m_Data[1] = y;
            m_Data[2] = z;
        }

        inline Mat3 operator+(const Mat3& rhs) {
            Mat3 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                result_data[i] = data[i] + rhs_data[i];
            }
            return result;
        }

        inline Mat3 operator-(const Mat3& rhs) const {
            Mat3 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                result_data[i] = data[i] - rhs_data[i];
            }
            return result;
        }

        /*
        inline Mat3 operator*(const Mat3& rhs) const {
            Mat3 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 3; i++) {
                f32 *c = result_data + i*3;
                for (i32 j = 0; j < 3; j++) {
                    c[j] = 0.0f;
                }
                for (i32 k = 0; k < 3; k++) {
                    const f32* b = rhs_data + k*3;
                    f32 a = data[i*3 + k];
                    for (i32 j = 0; j < 3; j++) {
                        c[j] += a*b[j];
                    }
                }
            }
            return result;
        }
        */

        inline Vec3 operator*(const Vec3& rhs) const {
            Vec3 result;
            for (i32 i = 0; i < 3; i++) {
                f32 sum = 0.0f;
                for (i32 j = 0; j < 3; j++) {
                    sum += m_Data[j][i]*rhs[j];
                }
                result[i] = sum;
            }
            return result;
        }

        inline Mat3 operator/(f32 rhs) const {
            Mat3 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            for (i32 i = 0; i < 9; i++) {
                result_data[i] = data[i]/rhs;
            }
            return result;
        }

        inline Mat3& operator+=(const Mat3& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] += data[i] + rhs_data[i];
            }
            return *this;
        }

        inline Mat3& operator-=(const Mat3& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] -= data[i] + rhs_data[i];
            }
            return *this;
        }

        /*
        inline Mat3& operator*=(const Mat3& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 3; i++) {
                float* c = data + i*3;
                for (i32 j = 0; j < 3; j++) {
                    c[j] = 0.0f;
                }
                for (i32 k = 0; k < 3; k++) {
                    const float* b = rhs_data + k*3;
                    float a = data[i*3 + k];
                    for (i32 j = 0; j < 3; j++) {
                        c[j] += a*b[j];
                    }
                }
                return *this;
            }
        }
        */

        inline Mat3& operator*=(f32 rhs) {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] *= rhs;
            }
            return *this;
        }

        inline Mat3& operator/=(f32 rhs) {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] /= rhs;
            }
            return *this;
        }

        inline Vec3& operator[](i32 i) {
            assert(i >= 0 && i < 3);
            return m_Data[i];
        }

        inline const Vec3& operator[](i32 i) const {
            assert(i >= 0 && i < 3);
            return m_Data[i];
        }

        inline const f32* data() const { return (const f32*)m_Data; }
        inline f32* data() { return (f32*)m_Data; }
        
        friend inline Mat3 operator*(f32 lhs, const Mat3& rhs) {
            Mat3 result;
            f32* data = (f32*)result.m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] = lhs*rhs_data[i];
            }
            return result;
        }

        friend inline Mat3 operator*(const Mat3& lhs, f32 rhs) {
            Mat3 result;
            f32* data = (f32*)result.m_Data;
            const f32* lhs_data = (const f32*)lhs.m_Data;
            for (i32 i = 0; i < 9; i++) {
                data[i] = lhs_data[i]*rhs;
            }
            return result;
        }
    private:
        Vec3 m_Data[3];
    };
    
    class Mat4 {
    public:
        Mat4() {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] = 0.0f;
            }
            data[0] = 1.0f;
            data[5] = 1.0f;
            data[10] = 1.0f;
            data[15] = 1.0f;
        }
        
        Mat4(const f32* data) {
            f32* data_ptr = (f32*)m_Data;
            for (int i = 0; i < 16; i++) {
                data_ptr[i] = data[i];
            }
        }

        explicit Mat4(const Mat3& m);

        Mat4(const Math::Vec4& x, const Math::Vec4& y, const Math::Vec4& z, const Math::Vec4& w) {
            m_Data[0] = x;
            m_Data[1] = y;
            m_Data[2] = z;
            m_Data[3] = w;
        }

        inline Mat4 operator+(const Mat4& rhs) {
            Mat4 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                result_data[i] = data[i] + rhs_data[i];
            }
            return result;
        }

        inline Mat4 operator-(const Mat4& rhs) const {
            Mat4 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                result_data[i] = data[i] - rhs_data[i];
            }
            return result;
        }

        inline Mat4 operator*(const Mat4& rhs) const {
            f32 res[16];
            res[0]  =  m_Data[0][0]*rhs.m_Data[0][0] + m_Data[1][0]*rhs.m_Data[0][1] + m_Data[2][0]*rhs.m_Data[0][2] + m_Data[3][0]*rhs.m_Data[0][3];
            res[1]  =  m_Data[0][1]*rhs.m_Data[0][0] + m_Data[1][1]*rhs.m_Data[0][1] + m_Data[2][1]*rhs.m_Data[0][2] + m_Data[3][1]*rhs.m_Data[0][3];
            res[2]  =  m_Data[0][2]*rhs.m_Data[0][0] + m_Data[1][2]*rhs.m_Data[0][1] + m_Data[2][2]*rhs.m_Data[0][2] + m_Data[3][2]*rhs.m_Data[0][3];
            res[3]  =  m_Data[0][3]*rhs.m_Data[0][0] + m_Data[1][3]*rhs.m_Data[0][1] + m_Data[2][3]*rhs.m_Data[0][2] + m_Data[3][3]*rhs.m_Data[0][3];
            res[4]  =  m_Data[0][0]*rhs.m_Data[1][0] + m_Data[1][0]*rhs.m_Data[1][1] + m_Data[2][0]*rhs.m_Data[1][2] + m_Data[3][0]*rhs.m_Data[1][3];
            res[5]  =  m_Data[0][1]*rhs.m_Data[1][0] + m_Data[1][1]*rhs.m_Data[1][1] + m_Data[2][1]*rhs.m_Data[1][2] + m_Data[3][1]*rhs.m_Data[1][3];
            res[6]  =  m_Data[0][2]*rhs.m_Data[1][0] + m_Data[1][2]*rhs.m_Data[1][1] + m_Data[2][2]*rhs.m_Data[1][2] + m_Data[3][2]*rhs.m_Data[1][3];
            res[7]  =  m_Data[0][3]*rhs.m_Data[1][0] + m_Data[1][3]*rhs.m_Data[1][1] + m_Data[2][3]*rhs.m_Data[1][2] + m_Data[3][3]*rhs.m_Data[1][3];
            res[8]  =  m_Data[0][0]*rhs.m_Data[2][0] + m_Data[1][0]*rhs.m_Data[2][1] + m_Data[2][0]*rhs.m_Data[2][2] + m_Data[3][0]*rhs.m_Data[2][3];
            res[9]  =  m_Data[0][1]*rhs.m_Data[2][0] + m_Data[1][1]*rhs.m_Data[2][1] + m_Data[2][1]*rhs.m_Data[2][2] + m_Data[3][1]*rhs.m_Data[2][3];
            res[10] =  m_Data[0][2]*rhs.m_Data[2][0] + m_Data[1][2]*rhs.m_Data[2][1] + m_Data[2][2]*rhs.m_Data[2][2] + m_Data[3][2]*rhs.m_Data[2][3];
            res[11] =  m_Data[0][3]*rhs.m_Data[2][0] + m_Data[1][3]*rhs.m_Data[2][1] + m_Data[2][3]*rhs.m_Data[2][2] + m_Data[3][3]*rhs.m_Data[2][3];
            res[12] =  m_Data[0][0]*rhs.m_Data[3][0] + m_Data[1][0]*rhs.m_Data[3][1] + m_Data[2][0]*rhs.m_Data[3][2] + m_Data[3][0]*rhs.m_Data[3][3];
            res[13] =  m_Data[0][1]*rhs.m_Data[3][0] + m_Data[1][1]*rhs.m_Data[3][1] + m_Data[2][1]*rhs.m_Data[3][2] + m_Data[3][1]*rhs.m_Data[3][3];
            res[14] =  m_Data[0][2]*rhs.m_Data[3][0] + m_Data[1][2]*rhs.m_Data[3][1] + m_Data[2][2]*rhs.m_Data[3][2] + m_Data[3][2]*rhs.m_Data[3][3];
            res[15] =  m_Data[0][3]*rhs.m_Data[3][0] + m_Data[1][3]*rhs.m_Data[3][1] + m_Data[2][3]*rhs.m_Data[3][2] + m_Data[3][3]*rhs.m_Data[3][3];

            return Mat4(res);
        }

        inline Vec4 operator*(const Vec4& rhs) const {
            Vec4 result;
            for (i32 i = 0; i < 4; i++) {
                f32 sum = 0.0f;
                for (i32 j = 0; j < 4; j++) {
                    sum += m_Data[j][i]*rhs[j];
                }
                result[i] = sum;
            }
            return result;
        }

        inline Mat4 operator/(f32 rhs) const {
            Mat4 result;
            f32* result_data = (f32*)result.m_Data;
            const f32* data = (const f32*)m_Data;
            for (i32 i = 0; i < 16; i++) {
                result_data[i] = data[i]/rhs;
            }
            return result;
        }

        inline Mat4& operator+=(const Mat4& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] += data[i] + rhs_data[i];
            }
            return *this;
        }

        inline Mat4& operator-=(const Mat4& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] -= data[i] + rhs_data[i];
            }
            return *this;
        }

        /*
        inline Mat4& operator*=(const Mat4& rhs) {
            f32* data = (f32*)m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 4; i++) {
                float* c = data + i*4;
                for (i32 j = 0; j < 4; j++) {
                    c[j] = 0.0f;
                }
                for (i32 k = 0; k < 4; k++) {
                    const float* b = rhs_data + k*4;
                    float a = data[i*4 + k];
                    for (i32 j = 0; j < 4; j++) {
                        c[j] += a*b[j];
                    }
                }
                return *this;
            }
        }
        */

        inline Mat4& operator*=(f32 rhs) {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] *= rhs;
            }
            return *this;
        }

        inline Mat4& operator/=(f32 rhs) {
            f32* data = (f32*)m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] /= rhs;
            }
            return *this;
        }

        inline Vec4& operator[](i32 i) {
            assert(i >= 0 && i < 4);
            return m_Data[i];
        }

        inline const Vec4& operator[](i32 i) const {
            assert(i >= 0 && i < 4);
            return m_Data[i];
        }

        inline const f32* data() const { return (const f32*)m_Data; }
        inline f32* data() { return (f32*)m_Data; }
        
        friend inline Mat4 operator*(f32 lhs, const Mat4& rhs) {
            Mat4 result;
            f32* data = (f32*)result.m_Data;
            const f32* rhs_data = (const f32*)rhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] = lhs*rhs_data[i];
            }
            return result;
        }

        friend inline Mat4 operator*(const Mat4& lhs, f32 rhs) {
            Mat4 result;
            f32* data = (f32*)result.m_Data;
            const f32* lhs_data = (const f32*)lhs.m_Data;
            for (i32 i = 0; i < 16; i++) {
                data[i] = lhs_data[i]*rhs;
            }
            return result;
        }
    private:
        Vec4 m_Data[4];
    };

    Mat4 Inverse(const Mat4& mat);
}

#endif /* End of MATH_MAT_H */
