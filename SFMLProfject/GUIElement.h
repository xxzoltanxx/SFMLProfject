#pragma once

struct Style
{
	sf::Vector2f size;
	sf::Color backgroundColor;
	std::string backgroundTexture;
	sf::Color bakgroundTextureColor;

	std::string font;
	sf::Color fontColor;
	sf::Vector2f textPadding;

	std::string glyph;
	sf::Color glyphColor;
	sf::Vector2f glyphPadding;
};

struct GUIDrawables
{
	sf::RectangleShape rectangle;
	sf::Sprite background;
	sf::Text text;
	sf::Sprite glyph;
};

enum class GUIState
{
	Normal,
	Hover,
	Clicked
};
class GUIElement
{
	friend class GUIInterface;
public:
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void update(float dt) = 0;
	virtual void onClick(const sf::Vector2i& mousePos) = 0;
	virtual void onRelease(const sf::Vector2i& mousePos) = 0;
	virtual void onHover() = 0;
	virtual void onExit() = 0;
private:
	std::string name;
	bool needsRedrawing() const { return needsRedraw; }
	void applyStyle();
	void setRedraw(bool redraw);
	void setStyle(GUIState state);
	void updateStyle(GUIState state, const Style& style);
	void applyBackgroundStyle();
	void applyGlyphStyle();
	void applyTextStyle();
	GUIDrawables drawables;
	GUIState currentState;
	bool needsRedraw;
	std::unordered_map<GUIState, Style> styles;

	sf::Vector2f mPosition;
};

#include "TextureManager.h"
#include "FontManager.h"

void GUIElement::applyStyle()
{
	applyBackgroundStyle();
	applyGlyphStyle();
	applyTextStyle();
}

void GUIElement::updateStyle(GUIState state, const Style& style)
{
	if (styles[state].backgroundTexture != "")
	{
		TextureManager::get()->request(style.backgroundTexture);
		TextureManager::get()->release(styles[state].backgroundTexture);
	}
	if (styles[state].glyph != "")
	{
		TextureManager::get()->request(style.glyph);
		TextureManager::get()->release(styles[state].glyph);
	}
	if (styles[state].font != "")
	{
		FontManager::get()->request(style.font);
		FontManager::get()->release(styles[state].font);
	}
	styles[state] = style;
	if (currentState == state)
	{
		applyStyle();
		setRedraw(true);
	}
}

void GUIElement::setRedraw(bool redraw)
{
	needsRedraw = redraw;
}

void GUIElement::setStyle(GUIState state)
{
	if (currentState != state)
	{
		currentState = state;
		applyStyle();
		setRedraw(true);
	}
}

void GUIElement::applyTextStyle()
{
	Style& currentStyle = styles[currentState];
	if (currentStyle.font != "")
	{
		sf::Font* resource = FontManager::get()->getResource(currentStyle.font);
		drawables.text.setFont(*resource);
		drawables.text.setPosition(mPosition + currentStyle.textPadding);
	}
}

void GUIElement::applyGlyphStyle()
{
	Style& currentStyle = styles[currentState];
	if (currentStyle.glyph != "")
	{
		sf::Texture* resource = TextureManager::get()->getResource(currentStyle.glyph);
		drawables.glyph.setTexture(*resource);
		drawables.glyph.setPosition(mPosition + currentStyle.glyphPadding);
	}
}

void GUIElement::applyBackgroundStyle()
{
	Style& currentStyle = styles[currentState];
	if (currentStyle.backgroundTexture != "")
	{
		sf::Texture* resource = TextureManager::get()->getResource(currentStyle.backgroundTexture);
		drawables.background.setTexture(*resource);
		drawables.rectangle.setSize(currentStyle.size);
		drawables.background.setPosition(mPosition);
	}
}