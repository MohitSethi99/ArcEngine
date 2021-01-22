#pragma once

#include "Arc/Core/Base.h"

#include <box2d/b2_body.h>

#include <glm/glm.hpp>

#include "box2d/b2_fixture.h"

namespace ArcEngine
{
	enum class Rigidbody2dType
	{
		Static = b2BodyType::b2_staticBody,
		Kinematic = b2BodyType::b2_kinematicBody,
		Dynamic = b2BodyType::b2_dynamicBody
	};

	class Rigidbody2D
	{
	public:
		enum class CollisionDetectionType { Discrete = 0, Continuous };
		enum class SleepType { NeverSleep = 0, StartAwake, StartAsleep };

		struct Rigidbody2DSpecification
		{
			Rigidbody2dType Type = Rigidbody2dType::Dynamic;
			float LinearDamping = 0.0f;
			float AngularDamping = 0.0f;
			float GravityScale = 1.0f;

			CollisionDetectionType CollisionDetection = CollisionDetectionType::Discrete;
			SleepType SleepingMode = SleepType::StartAwake;

			bool FreezeRotationZ = false;
		};

	public:
		Rigidbody2D(const glm::vec2& position, float rotation, const Rigidbody2DSpecification& specification);
		~Rigidbody2D() = default;

		inline Ref<Rigidbody2DSpecification> GetSpecification() const { return m_Specification; }
		void SetSpecification(const Rigidbody2DSpecification& specification);

	public:
		inline glm::vec2& GetPosition() const { return (glm::vec2&)m_Body2D->GetPosition(); }
		inline float GetRotation() const { return m_Body2D->GetAngle(); }
		inline float GetMass() const { return m_Body2D->GetMass();	}
		inline glm::vec2& GetVelocity() const { return (glm::vec2&)m_Body2D->GetLinearVelocity(); }
		inline float GetAngularVelocity() const { return m_Body2D->GetAngularVelocity(); }
		inline float GetInertia() const { return m_Body2D->GetInertia(); }
		inline glm::vec2& GetLocalCenterOfMass() const { return (glm::vec2&)m_Body2D->GetLocalCenter(); }
		inline glm::vec2& GetWorldCenterOfMass() const { return (glm::vec2&)m_Body2D->GetWorldCenter(); }
		inline bool IsAwake() const { return m_Body2D->IsAwake(); }

		void SetTransform(const glm::vec2& position, const float rotation) const { m_Body2D->SetTransform((b2Vec2&)position, rotation); }
		void SetType(const Rigidbody2dType type) const;
		void SetMass(const float value) const;
		inline void SetLinearDamping(const float value) const { m_Specification->LinearDamping = value; m_Body2D->SetLinearDamping(value); }
		inline void SetAngularDamping(const float value) const { m_Specification->AngularDamping = value; m_Body2D->SetAngularDamping(value); }
		inline void SetGravityScale(const float value) const { m_Specification->GravityScale = value; m_Body2D->SetGravityScale(value); }
		void SetCollisionDetection(const CollisionDetectionType type) const { m_Specification->CollisionDetection = type; m_Body2D->SetBullet(type == CollisionDetectionType::Continuous ? true : false); }
		void SetSleepingMode(const SleepType type) const;
		inline void SetFreezeRotation(const bool flag) const { m_Specification->FreezeRotationZ = flag; m_Body2D->SetFixedRotation(flag); }

		inline void ResetMassData() const { m_Body2D->ResetMassData(); }

	private:
		Ref<Rigidbody2DSpecification> m_Specification{};

		b2Body* m_Body2D;

		friend class BoxCollider2D;
		friend class CircleCollider2D;
	};
}

