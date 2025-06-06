#include "CrashPCH.h"
#include "CrashCamera.h"

#include "CrashInputMgr.h"

#include "CrashEngine.h"
namespace Crash
{
    glm::mat4 Camera::getViewMat() const
    {
        return glm::lookAt(mPos, mPos + mFront, mUp);
    }

    glm::mat4 Camera::getProjectionMat(float aspect) const
    {
        if(mType == ProjectionType::Perspective)
        {
            if(Engine::Instance()->isReverseZ())
            {
                // 反转Z轴
                return glm::perspective(glm::radians(mFov), aspect, mFarPlane, mNearPlane);
            }
            else
            {
                // 正常Z轴
                return glm::perspective(glm::radians(mFov), aspect, mNearPlane, mFarPlane);
            }
        }
        
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

            mYaw   += float(dx);
            mPitch -= float(dy); 

            if (mPitch > 89.0f)  mPitch = 89.0f;
            if (mPitch < -89.0f) mPitch = -89.0f;

            mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront.y = sin(glm::radians(mPitch));
            mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront = glm::normalize(mFront);

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
        
            //  glm::normalize(dir);
            mPos += dir * mMoveSpeed * deltaT;
        }
    }
}