#include "GUIElement.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "GUIManager.h"

void GUIElement::applyStyle()
{
	applyBgStyle();
	applyGlyphStyle();
	applyTextStyle();
	mParent->adjustContentSize();
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
	mStyles[state] = style;
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


GUIElement::GUIElement(std::string name, GUIInterface* parent)
{
	this->name = name;
	mPosition = sf::Vector2f(0, 0);
	mIsControl = false;
	mNeedsRedraw = true;

	mCurrentState = GUIState::Normal;
	mStyles.emplace(GUIState::Normal, GUIStyle());
	mStyles.emplace(GUIState::Hover, GUIStyle());
	mStyles.emplace(GUIState::Click, GUIStyle());

	text = "";
	mParent = parent;
	isActive = true;
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
	sf::RenderWindow* window = &manager->getContext().window; //Get window from manager context
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	GUIEvent event;
	event.coords.x = mousePosition.x;
	event.coords.y = mousePosition.y;
	event.interfaceC = name.c_str();
	for (auto& element : mElements)
	{
		if (element.second->isWithin(mousePosition) && element.second->mCurrentState == GUIState::Click)
		{
			event.type = GUIEventType::Release;
			element.second->onRelease();
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			manager->addEvent(event);
		}
	}
	setState(GUIState::Hover);
	mNeedsBackdropRedraw = true;
}

void GUIInterface::onClick(const sf::Vector2i& position)
{
	GUIEvent event;
	event.interfaceC = name.c_str();
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
			manager->addEvent(event);
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
	sf::RenderWindow* window = &manager->getContext().window; //Get window from manager context
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	GUIEvent event;
	event.interfaceC = name.c_str();
	event.coords.x = mousePosition.x;
	event.coords.y = mousePosition.y;
	for (auto& element : mElements)
	{
		if (element.second->isWithin(mousePosition) && element.second->mCurrentState == GUIState::Normal)
		{
			event.type = GUIEventType::Enter;
			element.second->onHover(mousePosition);
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			manager->addEvent(event);
		}
		else if (!element.second->isWithin(mousePosition) && element.second->mCurrentState != GUIState::Normal)
		{
			event.type = GUIEventType::Exit;
			element.second->onLeave();
			event.element = element.second->name.c_str();
			element.second->mIsControl ? mNeedsControlRedraw = true : mNeedsContentRedraw = true;
			manager->addEvent(event);
		}

	}
}

void GUIInterface::adjustContentSize()
{
	float currentSizeX = mStyles[mCurrentState].size.x;
	float currentSizeY = mStyles[mCurrentState].size.y;

	for (auto& a : mElements)
	{
		sf::Vector2f globalPos = a.second->getPosition();
		if (globalPos.x + a.second->mStyles[a.second->mCurrentState].size.x > currentSizeX)
		{
			currentSizeX = globalPos.x + a.second->mStyles[a.second->mCurrentState].size.x;
		}
		if (globalPos.y + a.second->mStyles[a.second->mCurrentState].size.y > currentSizeY)
		{
			currentSizeY = globalPos.y + a.second->mStyles[a.second->mCurrentState].size.y;
		}
	}

	mStyles[mCurrentState].size.x = currentSizeX;
	mStyles[mCurrentState].size.y = currentSizeY;
	mContent->create(currentSizeX, currentSizeY);
	mContentSprite.setTexture(mContent->getTexture());
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

GUIInterface::GUIInterface(GUIManager* mgr, std::string name)
	:GUIElement(name, this)
{
	mNeedsContentRedraw = true;
	mNeedsControlRedraw = true;
	mNeedsBackdropRedraw = true;

	mBackdrop = new sf::RenderTexture();
	mContent = new sf::RenderTexture();
	mControls = new sf::RenderTexture();

	mHorizontalScroll = 0;
	mVerticalScroll = 0;

	manager = mgr;
}

GUIInterface::~GUIInterface()
{
	delete mBackdrop;
	delete mContent;
	delete mControls;
}


void GUIInterface::addButton(std::string label, std::string elementName)
{
	GUIElement* button = manager->constructElement(GUIElementType::Label, elementName, this);
	button->setText(label);
	mElements[elementName] = button;
}


/////////////////////////////////////////////LABEL










void GUILabel::draw(sf::RenderTarget& target)
{
	target.draw(mDrawables.backgroundRect);
	target.draw(mDrawables.text);
}

void GUILabel::update(float dt)
{

}

void GUILabel::onHover(const sf::Vector2i& pos)
{
	setState(GUIState::Hover);
}

void GUILabel::onClick(const sf::Vector2i& pos)
{
	setState(GUIState::Click);
}

void GUILabel::onRelease()
{
	setState(GUIState::Hover);
}

void GUILabel::onLeave()
{
	setState(GUIState::Normal);
}

GUILabel::GUILabel(std::string elementName, GUIInterface* parent)
	:GUIElement(elementName, parent)
{
	GUIStyle buttonStyle;
	buttonStyle.backgroundColor = sf::Color(255, 0, 0, 255);
	buttonStyle.font = "Default";
	buttonStyle.fontSize = 12;
	buttonStyle.mCenterText = true;
	buttonStyle.size = sf::Vector2f(100, 50);
	buttonStyle.textColor = sf::Color(255, 255, 255, 255);
	updateStyle(GUIState::Click, buttonStyle);
	buttonStyle.backgroundColor = sf::Color(255, 0, 255, 255);
	updateStyle(GUIState::Hover, buttonStyle);
	buttonStyle.backgroundColor = sf::Color(255, 255, 0, 255);
	updateStyle(GUIState::Normal, buttonStyle);
}