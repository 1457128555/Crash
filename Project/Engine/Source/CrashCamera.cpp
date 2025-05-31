#include "CrashPCH.h"
#include "CrashCamera.h"

#include "CrashInputMgr.h"

namespace Crash
{
    glm::mat4 Camera::getViewMat() const
    {
        return glm::lookAt(mPos, mPos + mFront, mUp);
    }

    glm::mat4 Camera::getProjectionMat(float aspect) const
    {
        if(mType == ProjectionType::Perspective)
            return glm::perspective(glm::radians(mFov), aspect, mNearPlane, mFarPlane);
        
        assert(false);
        return glm::mat4(1.f);
    }

    void Camera::update(float deltaT)
    {
        //  update direction
        if(InputMgr::Instance()->isKeyDown(InputMgr::KeyCode::MOUSE_BUTTON_RIGHT))
        {
            double dx = 0.0, dy = 0.0;
            InputMgr::Instance()->getMouseDelta(dx, dy);
            dx *= mSensitivity;
            dy *= mSensitivity;

            // 更新欧拉角
            mYaw   += float(dx);
            mPitch -= float(dy); // 注意y轴通常反向

            // 限制俯仰角，防止翻转
            if (mPitch > 89.0f)  mPitch = 89.0f;
            if (mPitch < -89.0f) mPitch = -89.0f;

            // 计算新的前向量
            mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront.y = sin(glm::radians(mPitch));
            mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront = glm::normalize(mFront);

            // 重新计算右向量和上向量
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            mRight = glm::normalize(glm::cross(mFront, worldUp));
            mUp    = glm::normalize(glm::cross(mRight, mFront));
        }

        //  update move
        {
            glm::vec3 dir = {0.f, 0.f, 0.f};
        
            if(InputMgr::Instance()->isKeyDown(InputMgr::KeyCode::KEY_W))
                dir += mFront;
            if(InputMgr::Instance()->isKeyDown(InputMgr::KeyCode::KEY_S))
                dir -= mFront;
            if(InputMgr::Instance()->isKeyDown(InputMgr::KeyCode::KEY_D))
                dir += mRight;
            if(InputMgr::Instance()->isKeyDown(InputMgr::KeyCode::KEY_A))
                dir -= mRight;
        
            glm::normalize(dir);
            mPos += dir * mMoveSpeed * deltaT;
        }
    }
}