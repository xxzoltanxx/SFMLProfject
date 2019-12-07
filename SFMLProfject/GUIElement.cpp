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
		text.setColor(currentStyle.textColor);
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