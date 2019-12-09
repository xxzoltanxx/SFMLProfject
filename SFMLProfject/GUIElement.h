#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
enum class GUIElementType
{
	Label
};

struct GUIStyle
{
	GUIStyle() = default;
	GUIStyle(const std::string& styleFile);
	sf::Vector2f size = sf::Vector2f(0,0);

	sf::Vector2f backgroundPadding = sf::Vector2f(0,0);
	std::string background = "";
	sf::Color backgroundColor = sf::Color(0,0,0,0);

	sf::Vector2f glyphPadding = sf::Vector2f(0, 0);
	std::string glyph = "";
	sf::Color glyphColor = sf::Color(0, 0, 0, 0);

	sf::Vector2f fontPadding = sf::Vector2f(0, 0);
	std::string font = "";
	sf::Color textColor = sf::Color(0, 0, 0, 0);
	unsigned int fontSize = 0;
	bool mCenterText = false;
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
public:
	friend class GUIInterface;
	GUIElement(std::string name, GUIInterface* parent);
	void setActive(bool active) { isActive = active; }
protected:
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void update(float dt) = 0;
	virtual void onHover(const sf::Vector2i& pos) = 0;
	virtual void onClick(const sf::Vector2i& pos) = 0;
	virtual void onRelease() = 0;
	virtual void onLeave() = 0;

	virtual void readIn(std::stringstream& is) = 0;

	friend std::stringstream& operator>>(std::stringstream& is, GUIElement* element)
	{
		element->readIn(is);
		return is;
	}
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

	bool isActive;
};

class GUILabel : public GUIElement
{
public:
	GUILabel(std::string elementName, GUIInterface* parent);
	void draw(sf::RenderTarget& target) override;
	void update(float dt) override;
	void onHover(const sf::Vector2i& pos) override;
	void onClick(const sf::Vector2i& pos) override;
	void onRelease() override;
	void onLeave() override;
	void readIn(std::stringstream& is) override;
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
	const char* interfaceC;
	const char* element;
	union
	{
		ClickCoordinates coords;
	};
};

class GUIManager;

class GUIInterface : public GUIElement
{
	friend class GUIManager;
public:
	void adjustContentSize();
	GUIInterface(GUIManager*, std::string);
	~GUIInterface();
private:

	void addElement(std::stringstream& is);
	void redrawContent();
	void redrawControls();
	void redrawBackdrop();

	void update(float dt) override;
	void draw(sf::RenderTarget& target) override;
	void onHover(const sf::Vector2i& position) override;
	void onClick(const sf::Vector2i& position) override;
	void onRelease() override;
	void onLeave() override;
	void readIn(std::stringstream& is) override;

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
	
	GUIManager* manager;
};