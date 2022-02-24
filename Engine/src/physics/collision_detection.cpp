#include <string>

#include "../core/log.h"
#include "collider.h"
#include "rigid_body.h"
#include "collision_detection.h"

namespace Fly {
    const f32 COLLISION_EPS = 0.0f;

    bool CollisionDetection::SphereSphere(const Collider& a, const Collider& b, ContactData& contactData) {
        const SphereCollider& sphereA = (const SphereCollider&) a;
        const SphereCollider& sphereB = (const SphereCollider&) b;

        if (contactData.contactsLeft <= 0) { return false; }

        Math::Vec3 posA = sphereA.transform.GetPosition();
        Math::Vec3 posB = sphereB.transform.GetPosition();

        Math::Vec3 diff = posA - posB;
        f32 d = Math::Length(diff);

        if (d >= sphereA.radius + sphereB.radius) {
            return false;
        }

        Contact& contact = contactData.contacts[FLY_MAX_CONTACTS - contactData.contactsLeft];
        contact.normal = diff/d;
        contact.point = posA + diff*0.5f;
        contact.penetration = sphereA.radius + sphereB.radius - d;
        contact.bodies[0] = sphereA.m_RigidBody;
        contact.bodies[1] = sphereB.m_RigidBody;

        return true;
    }



    bool CollisionDetection::SphereCylinderBrick(const Collider& sphereA, const Collider& cylinderBrickB, ContactData& contactData) {
        Contact& contact = contactData.contacts[FLY_MAX_CONTACTS - contactData.contactsLeft];
        const SphereCollider& sphere = (const SphereCollider&)sphereA;
        const CylinderBrickCollider& brick = (const CylinderBrickCollider&)cylinderBrickB;

        contact.bodies[0] = sphere.m_RigidBody;
        contact.bodies[1] = brick.m_RigidBody;

        Math::Vec3 posSphereWS = sphere.transform.GetPosition();
        Math::Mat4 model = brick.transform.GetModelMatrix();
        Math::Mat4 inverse = Math::Inverse(model);
        Math::Vec3 posSphere = Math::Vec3(inverse*Math::Vec4(posSphereWS, 1.0f));

        Math::Vec3 velocityWS = sphereA.m_RigidBody->GetVelocity();
        Math::Vec3 velocity = Math::Vec3(inverse*Math::Vec4(velocity, 0.0f));
        Math::Vec3 posSphereBefore = posSphere - velocity;

        Math::Vec3 posCylinderWS = brick.transform.GetPosition() + 0.5f*brick.transform.GetForward()*(brick.largeCylinderRadius + brick.smallCylinderRadius);
        Math::Vec3 posCylinder = Math::Vec3(inverse*Math::Vec4(posCylinderWS, 1.0f));

        // Disable intersection with opposite ring area
        Math::Vec3 cNormal = Math::Normalize(-posCylinder);
        f32 d = -Math::Dot(cNormal, posCylinder);
        if (Math::Dot(cNormal, posSphere) - sphere.radius + d < 0.0f) {
            return false;
        }        

        if (posSphere.y > -0.5f*brick.height && posSphere.y < 0.5*brick.height) { 
            // Find left and right border planes
            f32 cosTheta = Math::Cos(brick.angle/2.0f); 
            f32 sinTheta = Math::Sin(brick.angle/2.0f);

            Math::Vec3 lv = Math::Vec3(-sinTheta, 0.0f, cosTheta);
            Math::Vec3 leftPoint = posCylinder + brick.smallCylinderRadius*lv;
            Math::Vec3 leftNormal = Math::Normalize(Math::Cross(lv, Math::Vec3(0.0f, 1.0f, 0.0f)));

            Math::Vec3 rv = Math::Vec3(sinTheta, 0.0f, cosTheta);
            Math::Vec3 rightPoint = posCylinder + brick.smallCylinderRadius*rv;
            Math::Vec3 rightNormal = Math::Normalize(Math::Cross(rv, Math::Vec3(0.0f, -1.0f, 0.0f)));
        
            f32 d1 = -Math::Dot(leftNormal, leftPoint);
            f32 d2 = -Math::Dot(rightNormal, rightPoint);

            Math::Vec2 diff = Math::Vec2(posSphere.x, posSphere.z) - Math::Vec2(posCylinder.x, posCylinder.z);          
            f32 dist = Math::LengthSqr(diff);
            Math::Vec2 diffBefore = Math::Vec2(posSphereBefore.x, posSphereBefore.z) - Math::Vec2(posCylinder.x, posCylinder.z);          
            f32 distBefore = Math::LengthSqr(diffBefore);

            f32 R = sphere.radius + brick.largeCylinderRadius;
            f32 r = brick.smallCylinderRadius - sphere.radius;

            // Left case
            if (Math::Dot(leftNormal, posSphere) + d1 > 0.0f) {     
                Math::Vec2 pa = diff - Math::Vec2(lv.x, lv.z)*brick.smallCylinderRadius;
                Math::Vec2 ba = Math::Vec2(lv.x, lv.z)*(brick.largeCylinderRadius - brick.smallCylinderRadius);
                float h = Math::Clamp(Math::Dot(pa, ba)/ Math::Dot(ba, ba), 0.0f, 1.0f);

                f32 d = Math::Length(pa - ba*h);
                if (d < sphere.radius + COLLISION_EPS) {
                    Math::Vec3 normalLS = Math::Vec3(pa.x - ba.x*h, 0.0f, pa.y - ba.y*h)/d;

                    contact.normal = Math::Normalize(Math::Vec3(model*Math::Vec4(normalLS, 0.0f)));
                    contact.penetration = sphere.radius - d;
                    return true;
                }
                return false;
            }
            // Right case
            else if (Math::Dot(rightNormal, posSphere) + d2 > 0.0f) {
                Math::Vec2 pa = diff - Math::Vec2(rv.x, rv.z)*brick.smallCylinderRadius;
                Math::Vec2 ba = Math::Vec2(rv.x, rv.z)*(brick.largeCylinderRadius - brick.smallCylinderRadius);
                float h = Math::Clamp(Math::Dot(pa, ba)/ Math::Dot(ba, ba), 0.0f, 1.0f);

                f32 d = Math::Length(pa - ba*h);
                if (d < sphere.radius + COLLISION_EPS) {
                    Math::Vec3 normalLS = Math::Vec3(pa.x - ba.x*h, 0.0f, pa.y - ba.y*h)/d;

                    contact.normal = Math::Normalize(Math::Vec3(model*Math::Vec4(normalLS, 0.0f)));
                    contact.penetration = sphere.radius - d;
                    return true;
                }
                return false;
            }
            else {
                if (dist < (R*R + COLLISION_EPS) && dist > (r*r - COLLISION_EPS)) {

                    Math::Vec3 normalLS = Math::Normalize(Math::Vec3(diff.x, 0.0f, diff.y));
                    f32 d = Math::Sqrt(dist);
                    if (d > 0.5f*(R+r)) {
                        //contact.point
                        contact.normal = Math::Normalize(Math::Vec3(model*Math::Vec4(normalLS, 0.0f)));
                        contact.penetration = R - d;
                    }
                    else {
                        //contact.point
                        contact.penetration = d - r;
                        contact.normal = -Math::Normalize(Math::Vec3(model*Math::Vec4(normalLS, 0.0f)));
                    }
                    return true;
                }
                return false;
            }
        }
        else {
            Math::Vec3 endPoint1 = posCylinder + Math::Vec3(0.0f,  0.5f*brick.height, 0.0f);
            Math::Vec3 endPoint2 = posCylinder + Math::Vec3(0.0f, -0.5f*brick.height, 0.0f);

            Math::Vec3 endPoint;
            if (Math::LengthSqr(posSphere - endPoint1) > Math::LengthSqr(posSphere - endPoint2)) {
                endPoint = endPoint2;
            }
            else {
                endPoint = endPoint1;
            }

            Math::Vec3 v = Math::Cross(Math::Cross(Math::Vec3(0.0f, 1.0f, 0.0f), posSphere - endPoint), Math::Vec3(0.0f, 1.0f, 0.0));
            v = Math::Normalize(v);

            f32 t1, t2;
            Math::Vec3 p0 = endPoint - brick.largeCylinderRadius*v;

            if (!Math::RaySphereIntersection({p0, v}, posSphere, sphere.radius, t1, t2)) {
                return false;
            }

            t1 /= 2*brick.largeCylinderRadius;
            t2 /= 2*brick.largeCylinderRadius;

            f32 s1 = (1.0f - brick.smallCylinderRadius/brick.largeCylinderRadius)*0.5f;
            f32 s2 = 1.0f - s1;

            if (t1 > s1 && t2 < s2 || (t1 > 1.0f && t2 > 1.0f) || (t1 < 0.0f && t2 < 0.0f)) {
                return false;
            }
        }
        return true;
    }
}