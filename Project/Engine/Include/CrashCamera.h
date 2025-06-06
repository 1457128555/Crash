#pragma once

#include "CrashEngineDefines.hpp"

namespace Crash
{
    class CRASH_ENGINE_API Camera
    {
    public:
        enum class ProjectionType : unsigned int
        {
            Orthogonal,
            Perspective,
        };

        void update(float deltaT);

        glm::mat4 getViewMat()         const;
        glm::mat4 getProjectionMat(float aspect) const;

        glm::vec3 getPosition() const { return mPos; }
        glm::vec3 getFront()    const { return mFront; }

    private:
        ProjectionType mType = ProjectionType::Perspective;

        float mFov = 45.f;
        float mMoveSpeed = 2.f;
        float mSensitivity = 0.1f;

        float mNearPlane = 1.f;
        float mFarPlane  = 100.f;

        float mYaw = -90.0f;   // 初始朝向 -Z
        float mPitch = 0.0f;   // 初始水平

        glm::vec3 mPos      = { 0.0f, 0.0f,  3.0f };
        glm::vec3 mFront    = { 0.0f, 0.0f, -1.0f };
        glm::vec3 mRight    = { 0.0f, 1.0f,  0.0f };
        glm::vec3 mUp       = { 0.0f, 1.0f,  0.0f };
    };
}
