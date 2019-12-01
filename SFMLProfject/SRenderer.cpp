#include "SRenderer.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "CDrawable.h"
#include "CPosition.h"

SRenderer::SRenderer(SystemManager* mgr, System sysId)
	:SBase(mgr, System::Renderer)
{
	requirementFlags.push_back((1 << (int)ComponentType::Position) | (1 << (int)ComponentType::Sprite));
	requirementFlags.push_back((1 << (int)ComponentType::Position) | (1 << (int)ComponentType::SpriteSheet));
	//TODO Subscribe to messages
}

void SRenderer::handleEvent(unsigned int entityId, SystemEvent event)
{
	//TODO
}

void SRenderer::draw(sf::RenderTarget& target)
{
	EntityManager* entManager = manager->getEntityManager();
	for (auto& entity : mEntities)
	{
		CPosition* position = entManager->getComponent<CPosition>(entity, ComponentType::Position);
		if (CSpriteSheet * sheet = entManager->getComponent<CSpriteSheet>(entity, ComponentType::SpriteSheet))
		{
			sheet->setPosition(position->getPosition());
			sheet->draw(target);
		}
		else if (CSprite * sprite = entManager->getComponent<CSprite>(entity, ComponentType::Sprite))
		{
			sprite->setPosition(position->getPosition());
			sprite->draw(target);
		}
	}
}

void SRenderer::update(float dt)
{
	EntityManager* entManager = manager->getEntityManager();
	for (auto& entity : mEntities)
	{
		if (CSpriteSheet * sheet = entManager->getComponent<CSpriteSheet>(entity, ComponentType::SpriteSheet))
		{
			sheet->update(dt);
		}
	}
}