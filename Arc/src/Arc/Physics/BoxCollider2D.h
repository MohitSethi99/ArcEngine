#pragma once

#include "Rigidbody2D.h"

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace ArcEngine
{
	class BoxCollider2D
	{
	public:
		BoxCollider2D(Ref<Rigidbody2D>& rigidbody2D, glm::vec2& size, glm::vec2& offset, bool isTrigger);
		~BoxCollider2D() = default;

		void SetSpecification(glm::vec2& size, glm::vec2& offset, bool isTrigger);

	public:
		inline const glm::vec2& GetSize() const { return m_Size; }
		inline glm::vec2& GetOffset() const { return (glm::vec2&)((b2PolygonShape*)m_Fixture->GetShape())->m_centroid; }
		inline bool IsTrigger() const { return m_Fixture->IsSensor(); }

		inline float GetDensity() const { return m_Fixture->GetDensity(); }

	private:
		void CreateFixture(Ref<Rigidbody2D>& rigidbody2D, glm::vec2& size, glm::vec2& offset, bool isTrigger);

	private:
		glm::vec2 m_Size;
		
		Ref<Rigidbody2D> m_Rigidbody2D;
		b2Fixture* m_Fixture;
	};
}

