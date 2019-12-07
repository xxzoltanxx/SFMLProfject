#include <unordered_map>
#include <vector>
#include <functional>

#include "Game.h"

int main()
{
	Game game;
	game.run();
}

struct GUIStyle
{
	sf::Vector2i size;

	std::string background;
	sf::Color backgroundColor;

	std::string glyph;
	sf::Color glyphColor;

	std::string font;
	sf::Color textColor;
	unsigned int fontSize;
};

enum class GUIState
{
	Normal,
	Hover,
	Click
};

struct GUIDrawables
{
	sf::RectangleShape backgroundRect;
	sf::Sprite background;
	sf::Sprite glyph;
	sf::Text text;
};

class GUIElement
{
	friend class GUIInterface;
protected:
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void update(float dt) = 0;
	virtual void onHover() = 0;
	virtual void onClick(const sf::Vector2f& pos) = 0;
	virtual void onRelese(const sf::Vector2f& pos) = 0;
	virtual void onLeave() = 0;

	void setPosition(const sf::Vector2f& newPos);
	sf::Vector2f getPostition() const
	{
		return mPosition;
	}

	bool isControl() const {
		return mIsControl;
	}
	void updateStyle(GUIState state, const GUIStyle& style);
	void setState(GUIState state);
	void setRedraw(bool redraw);
	bool needsRedraw() const { return mNeedsRedraw; }
	void applyStyle();
	void applyBgStyle();
	void applyGlyphStyle();
	void applyTextStyle();

	std::string name;
	sf::Vector2f mPosition;
	bool mIsControl;
	bool mNeedsRedraw;
	GUIDrawables mDrawables;
	GUIState mCurrentState;
	std::unordered_map<GUIState, GUIStyle> mStyles;
};