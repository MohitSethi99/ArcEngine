#pragma once

#include "Rigidbody2D.h"

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

namespace ArcEngine
{
	class CircleCollider2D
	{
	public:
		CircleCollider2D(Ref<Rigidbody2D>& rigidbody2D, float radius, glm::vec2& offset, bool isTrigger);
		~CircleCollider2D() = default;

		void SetSpecification(float radius, glm::vec2& offset, bool isTrigger);

	public:
		inline float GetRadius() const { return ((b2CircleShape*)m_Fixture->GetShape())->m_radius; }
		inline glm::vec2& GetOffset() const { return (glm::vec2&)((b2CircleShape*)m_Fixture->GetShape())->m_p; }
		inline bool IsTrigger() const { return m_Fixture->IsSensor(); }

		inline float GetDensity() const { return m_Fixture->GetDensity(); }

	private:
		void CreateFixture(Ref<Rigidbody2D>& rigidbody2D, float radius, glm::vec2& offset, bool isTrigger);

	private:
		Ref<Rigidbody2D> m_Rigidbody2D;
		b2Fixture* m_Fixture;
	};
}

