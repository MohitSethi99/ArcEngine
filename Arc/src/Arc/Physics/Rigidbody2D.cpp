#include "arcpch.h"
#include "Rigidbody2D.h"

#include "Physics2D.h"

#include <box2d/b2_fixture.h>

namespace ArcEngine
{
	Rigidbody2D::Rigidbody2D(const glm::vec2& position, const float rotation, const Rigidbody2DSpecification& specification)
	{
		const b2Vec2 pos(position.x, position.y);
		b2BodyDef bodyDef;
		bodyDef.position = pos;

		m_Body2D = Physics2D::GetWorld()->CreateBody(&bodyDef);
		m_Body2D->SetTransform(pos, rotation);

		m_Specification = CreateRef<Rigidbody2DSpecification>(specification);
		SetSpecification(specification);
	}

	void Rigidbody2D::SetSpecification(const Rigidbody2DSpecification& specification)
	{
		SetType(specification.Type);
		SetLinearDamping(specification.LinearDamping);
		SetAngularDamping(specification.AngularDamping);
		SetGravityScale(specification.GravityScale);
		SetCollisionDetection(specification.CollisionDetection);
		SetSleepingMode(specification.SleepingMode);
		SetFreezeRotation(specification.FreezeRotationZ);

		m_Specification = CreateRef<Rigidbody2DSpecification>(specification);
	}

	void Rigidbody2D::SetType(const Rigidbody2dType type) const
	{
		m_Specification->Type = type;
		
		m_Body2D->SetType((b2BodyType)type);

		if (type == Rigidbody2dType::Dynamic)
		{
			uint32_t count = 0;
			b2Fixture* fixture = m_Body2D->GetFixtureList();
			while (fixture != nullptr)
			{
				count++;
				fixture = fixture->GetNext();
			}

			if (count == 0)
				SetMass(1.0f);
		}
	}

	void Rigidbody2D::SetMass(const float value) const
	{
		b2MassData massData;
		m_Body2D->GetMassData(&massData);
		massData.mass = value;
		m_Body2D->SetMassData(&massData);
	}

	void Rigidbody2D::SetSleepingMode(const SleepType type) const
	{
		m_Specification->SleepingMode = type;
		
		switch (type)
		{
			case SleepType::NeverSleep:
			{
				m_Body2D->SetSleepingAllowed(false);
				m_Body2D->SetAwake(true);
				break;
			}
			case SleepType::StartAsleep:
			{
				m_Body2D->SetSleepingAllowed(true);
				m_Body2D->SetAwake(false);
				break;
			}
			case SleepType::StartAwake:
			{
				m_Body2D->SetSleepingAllowed(true);
				m_Body2D->SetAwake(true);
				break;
			}
		}
	}
}
