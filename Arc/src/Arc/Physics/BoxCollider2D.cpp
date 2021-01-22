#include "arcpch.h"
#include "BoxCollider2D.h"

namespace ArcEngine
{
	BoxCollider2D::BoxCollider2D(Ref<Rigidbody2D>& rigidbody2D, glm::vec2& size, glm::vec2& offset, bool isTrigger)
		: m_Size(size)
	{
		CreateFixture(rigidbody2D, size, offset, isTrigger);
	}

	void BoxCollider2D::SetSpecification(glm::vec2& size, glm::vec2& offset, bool isTrigger)
	{
		m_Rigidbody2D->m_Body2D->DestroyFixture(m_Fixture);
		CreateFixture(m_Rigidbody2D, size, offset, isTrigger);
	}

	void BoxCollider2D::CreateFixture(Ref<Rigidbody2D>& rigidbody2D, glm::vec2& size, glm::vec2& offset, bool isTrigger)
	{
		m_Size = size;
		m_Rigidbody2D = rigidbody2D;

		b2PolygonShape box;
		box.SetAsBox(size.x / 2, size.y / 2, (b2Vec2&)offset, 0.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;

		fixtureDef.density = 1.0f;
		
		if (m_Rigidbody2D->GetSpecification()->Type == Rigidbody2dType::Dynamic)
			fixtureDef.friction = 0.3f;

		fixtureDef.isSensor = isTrigger;

		m_Fixture = m_Rigidbody2D->m_Body2D->CreateFixture(&fixtureDef);
	}
}
