#include "verticallayout.h"

#include "item/ilayoutitem.h"
#include "util/geometryoperation.h"

VerticalLayout::VerticalLayout(const sf::FloatRect &rect) : Layout{ rect }
{
}

void VerticalLayout::updateGeometry()
{
    alignCenter();

    Util::EnumFlag<Align> itemOrigin;

    if (alignment().test(Align::Left))
    {
        alignLeft();
        itemOrigin.set(Align::Left);
    }
    if (alignment().test(Align::Right))
    {
        alignRight();
        itemOrigin.set(Align::Right);
    }
    if (alignment().test(Align::Top))
    {
        alignTop();
        itemOrigin.set(Align::Top);
    }
    if (alignment().test(Align::Bottom))
    {
        alignBottom();
        itemOrigin.set(Align::Bottom);
    }

    for (const auto &item : items())
    {
        item->setOrigin(itemOrigin);
        item->setPosition(_nextItemPos);
        _nextItemPos.y += item->localRect().height + spacing();
    }
}

void VerticalLayout::alignCenter()
{
    const auto rect{ this->rect() };
    _nextItemPos = Util::pointBy(rect, { Align::Bottom, Align::Left, Align::Right, Align::Top });
    _nextItemPos.y -= contentSize().y / 2;
}

void VerticalLayout::alignLeft()
{
    _nextItemPos.x -= rect().width / 2;
}

void VerticalLayout::alignRight()
{
    _nextItemPos.x += rect().width / 2;
}

void VerticalLayout::alignTop()
{
    _nextItemPos.y += (contentSize().y / 2) - (rect().height / 2);
}

void VerticalLayout::alignBottom()
{
    _nextItemPos.y += (rect().height / 2) - (contentSize().y / 2);
}

sf::Vector2f VerticalLayout::contentSize() const
{
    sf::Vector2f size{};
    for (const auto &item : items())
    {
        size.x = std::max(size.x, item->globalRect().width);
        size.y += item->globalRect().height;
    }

    size.y += (items().size() - 1) * spacing();

    return size;
}
