#include "verticallayout.h"

#include "item/abstractitem.h"

VerticalLayout::VerticalLayout(RectF rect) : Layout{ rect }
{
}

void VerticalLayout::updateGeometry()
{
    alignCenter();

    Align itemOrigin{ Align::Top };

    if ((alignment() & Align::Left) != Align(0))
    {
        alignLeft();
        itemOrigin |= Align::Left;
    }
    if ((alignment() & Align::Right) != Align(0))
    {
        alignRight();
        itemOrigin |= Align::Right;
    }
    if ((alignment() & Align::Top) != Align(0))
    {
        alignTop();
    }
    if ((alignment() & Align::Bottom) != Align(0))
    {
        alignBottom();
    }

    for (const auto &item : items())
    {
        item->setOrigin(itemOrigin);
        item->setPos(_nextItemPos);
        _nextItemPos.moveY(item->localRect().height() + spacing());
    }
}

void VerticalLayout::alignCenter()
{
    _nextItemPos = rect().center();
    _nextItemPos.moveY(-contentSize().height / 2);
}

void VerticalLayout::alignLeft()
{
    _nextItemPos.moveX(-rect().width() / 2);
}

void VerticalLayout::alignRight()
{
    _nextItemPos.moveX(rect().width() / 2);
}

void VerticalLayout::alignTop()
{
    _nextItemPos.moveY((contentSize().height / 2) - (rect().height() / 2));
}

void VerticalLayout::alignBottom()
{
    _nextItemPos.moveY((rect().height() / 2) - (contentSize().height / 2));
}

SizeF VerticalLayout::contentSize() const
{
    SizeF size{};
    for (const auto &item : items())
    {
        size.width = std::max(size.width, item->globalRect().width());
        size.height += item->globalRect().height();
    }

    size.height += (items().size() - 1) * spacing();

    return size;
}
