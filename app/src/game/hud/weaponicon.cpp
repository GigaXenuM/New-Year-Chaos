#include "weaponicon.h"

#include "items/entity/physicalentity.h"

#include "resources/resourcemanager.h"
#include "weapon/iweapon.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <sstream>

namespace Game
{

WeaponIcon::WeaponIcon(const PhysicalEntity *entity, const sf::Vector2f &size)
    : _entity{ entity },
      _text{ "", ResourseManager::getInstance()->getFont(FontType::DejaVuSansBold) },
      _size{ size }
{
    _text.setFillColor(sf::Color{ 250, 90, 109 });
}

void WeaponIcon::setPosition(const sf::Vector2f pos)
{
    _icon.setOrigin(Util::pointBy(_icon.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _icon.setPosition({ pos.x, pos.y - _icon.getGlobalBounds().height / 2 });
    _text.setOrigin(Util::pointBy(_text.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _text.setPosition({ pos.x, pos.y + _text.getGlobalBounds().height * 1.2f });
}

const sf::Sprite *WeaponIcon::getSprite() const
{
    return &_icon;
}

sf::FloatRect WeaponIcon::globalRect() const
{
    return _icon.getGlobalBounds();
}

sf::FloatRect WeaponIcon::localRect() const
{
    return _icon.getLocalBounds();
}

sf::Vector2f WeaponIcon::position() const
{
    return _icon.getPosition();
}

void WeaponIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    const IWeapon *weapon{ _entity->weapon() };

    _icon.setTexture(ResourseManager::getInstance()->getTextures(TextureType::SnowBall)[2]);
    const sf::Vector2f currentSize{ _icon.getLocalBounds().getSize() };
    const sf::Vector2f scaling{ _size.x / currentSize.x, _size.y / currentSize.y };
    _icon.setScale(scaling);

    const auto reload{ weapon->reload() };
    std::ostringstream stingStream;
    stingStream << std::fixed << std::setprecision(1)
                << (reload.has_value() ? reload.value() : 0.f);
    _text.setString(reload.has_value() ? stingStream.str() : "");

    target.draw(_icon, states);
    target.draw(_text, states);
}

} // namespace Game
