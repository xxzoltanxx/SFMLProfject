#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

struct GUIStyle
{
	sf::Vector2f size;

	sf::Vector2f backgroundPadding;
	std::string background;
	sf::Color backgroundColor;

	sf::Vector2f glyphPadding;
	std::string glyph;
	sf::Color glyphColor;

	sf::Vector2f fontPadding;
	std::string font;
	sf::Color textColor;
	unsigned int fontSize;
	bool mCenterText;
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

class GUIInterface;
class GUIElement
{
	friend class GUIInterface;
protected:
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void update(float dt) = 0;
	virtual void onHover(const sf::Vector2i& pos) = 0;
	virtual void onClick(const sf::Vector2i& pos) = 0;
	virtual void onRelease() = 0;
	virtual void onLeave() = 0;

	void setPosition(const sf::Vector2f& newPos);
	sf::Vector2f getPosition() const
	{
		return mPosition;
	}
	sf::Vector2f getGlobalPosition() const;

	bool isControl() const {
		return mIsControl;
	}
	bool isWithin(const sf::Vector2i& position);
	void updateStyle(GUIState state, const GUIStyle& style);
	void setState(GUIState state);
	void setRedraw(bool redraw) { mNeedsRedraw = redraw; }
	bool needsRedraw() const { return mNeedsRedraw; }
	void setText(const std::string& text)
	{
		this->text = text;
	}
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

	std::string text;

	GUIInterface* mParent;
};

enum class GUIEventType
{
	Enter,
	Exit,
	Click,
	Release
};

struct ClickCoordinates
{
	float x, y;
};

struct GUIEvent
{
	GUIEventType type;
	const char* interface;
	const char* element;
	union
	{
		ClickCoordinates coords;
	};
};

class GUIInterface : public GUIElement
{
private:
	void redrawContent();
	void redrawControls();
	void redrawBackdrop();

	void update(float dt) override;
	void draw(sf::RenderTarget& target) override;
	void onHover(const sf::Vector2i& position) override;
	void onClick(const sf::Vector2i& position) override;
	void onRelease() override;
	void onLeave() override;

	std::unordered_map<std::string, GUIElement*> mElements;
	bool mNeedsContentRedraw;
	bool mNeedsControlRedraw;
	bool mNeedsBackdropRedraw;
	sf::RenderTexture* mBackdrop;
	sf::RenderTexture* mContent;
	sf::RenderTexture* mControls;

	sf::Sprite mBackdropSprite;
	sf::Sprite mContentSprite;
	sf::Sprite mControlSprite;

	float mHorizontalScroll;
	float mVerticalScroll;
};