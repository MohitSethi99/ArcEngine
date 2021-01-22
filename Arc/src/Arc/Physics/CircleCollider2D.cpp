#include "arcpch.h"
#include "CircleCollider2D.h"

namespace ArcEngine
{
	CircleCollider2D::CircleCollider2D(Ref<Rigidbody2D>& rigidbody2D, float radius, glm::vec2& offset, bool isTrigger)
	{
		CreateFixture(rigidbody2D, radius, offset, isTrigger);
	}

	void CircleCollider2D::SetSpecification(float radius, glm::vec2& offset, bool isTrigger)
	{
		m_Rigidbody2D->m_Body2D->DestroyFixture(m_Fixture);
		CreateFixture(m_Rigidbody2D, radius, offset, isTrigger);
	}

	void CircleCollider2D::CreateFixture(Ref<Rigidbody2D>& rigidbody2D, float radius, glm::vec2& offset, bool isTrigger)
	{
		m_Rigidbody2D = rigidbody2D;

		b2CircleShape circle;
		circle.m_radius = radius;
		circle.m_p.Set(offset.x, offset.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;

		if (m_Rigidbody2D->GetSpecification()->Type == Rigidbody2dType::Dynamic)
		{
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
		}
		fixtureDef.isSensor = isTrigger;

		m_Fixture = m_Rigidbody2D->m_Body2D->CreateFixture(&fixtureDef);
	}
}
