#include "GUIElement.h"
#include "TextureManager.h"
#include "FontManager.h"

void GUIElement::applyStyle()
{
	applyBgStyle();
	applyGlyphStyle();
	applyTextStyle();
}

void GUIElement::applyGlyphStyle()
{
	sf::Sprite& glyph = mDrawables.glyph;
	if (mStyles[mCurrentState].glyph != "")
	{
		glyph.setTexture(*TextureManager::get()->getResource(mStyles[mCurrentState].glyph));
		glyph.setColor(mStyles[mCurrentState].glyphColor);
		glyph.setPosition(mPosition + mStyles[mCurrentState].glyphPadding);
	}
}

void GUIElement::applyBgStyle()
{
	sf::Sprite& bg = mDrawables.background;
	if (mStyles[mCurrentState].background != "")
	{
		bg.setTexture(*TextureManager::get()->getResource(mStyles[mCurrentState].background));
		bg.setColor(mStyles[mCurrentState].backgroundColor);
		bg.setPosition(mPosition + mStyles[mCurrentState].backgroundPadding);
	}
	sf::RectangleShape& shape = mDrawables.backgroundRect;
	shape.setFillColor(mStyles[mCurrentState].backgroundColor);
	shape.setSize(mStyles[mCurrentState].size);
	shape.setPosition(mPosition);
}

void GUIElement::applyTextStyle()
{
	sf::Text& text = mDrawables.text;
	GUIStyle& currentStyle = mStyles[mCurrentState];
	if (currentStyle.font != "")
	{
		text.setCharacterSize(currentStyle.fontSize);
		text.setFillColor(currentStyle.textColor);
		text.setFont(*FontManager::get()->getResource(currentStyle.font));
		text.setString(this->text);
		if (currentStyle.mCenterText)
		{
			text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
			text.setPosition(currentStyle.size / 2.0f);
		}
		else
		{
			text.setPosition(mPosition + currentStyle.fontPadding);
		}
	}
}

void GUIElement::setPosition(const sf::Vector2f& newPos)
{
	mPosition = newPos;
}

void GUIElement::updateStyle(GUIState state, const GUIStyle& style)
{
	auto& currentlyMappedStyle = mStyles[state];
	if (style.background != "")
	{
		TextureManager::get()->release(currentlyMappedStyle.background);
		TextureManager::get()->request(style.background);
	}
	if (style.glyph != "")
	{
		TextureManager::get()->release(currentlyMappedStyle.glyph);
		TextureManager::get()->request(style.glyph);
	}
	if (style.font != "")
	{
		FontManager::get()->release(currentlyMappedStyle.font);
		FontManager::get()->request(style.font);
	}
	if (state == mCurrentState)
	{
		setRedraw(true);
	}
}

void GUIElement::setState(GUIState state)
{
	if (mCurrentState != state)
	{
		setRedraw(true);
	}
	mCurrentState = state;
}

sf::Vector2f GUIElement::getGlobalPosition() const
{
	sf::Vector2f position = getPosition();
	if (mParent && mParent != this)
	{
		sf::Vector2f parentPosition = mParent->getGlobalPosition();
		position = position + parentPosition;
	}
	return position;
}

bool GUIElement::isWithin(const sf::Vector2i& position)
{
	sf::Vector2f currentPosition = getGlobalPosition();
	sf::Vector2f size = mStyles[mCurrentState].size;
	return (position.x > currentPosition.x && position.x < currentPosition.x + size.x && position.y > currentPosition.y && position.y < currentPosition.y + size.y);
}






////////////////////////////GUIINTERFACE






void GUIInterface::onHover(const sf::Vector2i& position)
{
	setState(GUIState::Hover);
	mNeedsBackdropRedraw = true;
}

void GUIInterface::onLeave()
{
	setState(GUIState::Normal);
	mNeedsBackdropRedraw = true;
}

void GUIInterface::onRelease()
{
	sf::RenderWindow* window; //Get window from manager context
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	GUIEvent event;
	event.coords.x = mousePosition.x;
	event.coords.y = mousePosition.y;
	event.interface = name.c_str();
	for (auto& element : mElements)
	{
		if (!element.second->isWithin(mousePosition) && element.second->mCurrentState == GUIState::Click)
		{
			event.type = GUIEventType::Release;
			element.second->onRelease();
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			//ADD event to queue in GUIMANAGER
		}
	}
	setState(GUIState::Normal);
	mNeedsBackdropRedraw = true;
}

void GUIInterface::onClick(const sf::Vector2i& position)
{
	GUIEvent event;
	event.interface = name.c_str();
	event.coords.x = position.x;
	event.coords.y = position.y;
	for (auto& element : mElements)
	{
		if (element.second->isWithin(position) && element.second->mCurrentState != GUIState::Click)
		{
			event.type = GUIEventType::Click;
			element.second->onClick(position);
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			//ADD event to queue in GUIMANAGER
		}
	}
	setState(GUIState::Click);
	mNeedsBackdropRedraw = true;
}

void GUIInterface::draw(sf::RenderTarget& target)
{
	target.draw(mBackdropSprite);
	target.draw(mContentSprite);
	target.draw(mControlSprite);
}

void GUIInterface::update(float dt)
{
	sf::RenderWindow* window; //Get window from manager context
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	GUIEvent event;
	event.interface = name.c_str();
	event.coords.x = mousePosition.x;
	event.coords.y = mousePosition.y;
	for (auto& element : mElements)
	{
		if (element.second->isWithin(mousePosition) && element.second->mCurrentState != GUIState::Hover)
		{
			event.type = GUIEventType::Enter;
			element.second->onHover(mousePosition);
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			//ADD event to queue in GUIMANAGER
		}
		else if (!element.second->isWithin(mousePosition) && element.second->mCurrentState != GUIState::Normal)
		{
			event.type = GUIEventType::Exit;
			element.second->onLeave();
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			//ADD event to queue in GUIMANAGER
		}

	}
}

void GUIInterface::redrawControls()
{
	mControls->clear(sf::Color(0, 0, 0, 0));
	for (auto& element : mElements)
	{
		if (element.second->mIsControl)
		{
			if (element.second->needsRedraw())
			{
				element.second->applyStyle();
				element.second->draw(*mControls);
				element.second->setRedraw(false);
			}
		}
	}
	mControls->display();
	mNeedsControlRedraw = false;
}

void GUIInterface::redrawBackdrop()
{
	mBackdrop->clear(sf::Color(0, 0, 0, 0));
	if (mStyles[mCurrentState].background != "")
	{
		mBackdrop->draw(mDrawables.background);
	}
	else
	{
		mBackdrop->draw(mDrawables.backgroundRect);
	}
	mNeedsBackdropRedraw = false;
}

void GUIInterface::redrawContent()
{
	mContent->clear(sf::Color(0, 0, 0, 0));
	for (auto& element : mElements)
	{
		if (!element.second->mIsControl)
		{
			if (element.second->needsRedraw())
			{
				element.second->applyStyle();
				element.second->draw(*mContent);
				element.second->setRedraw(false);
			}
		}
	}
	mContent->display();
	mNeedsContentRedraw = false;
}