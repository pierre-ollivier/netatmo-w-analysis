#include "CustomItemDelegate.h"

void CustomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    opt.state &= ~QStyle::State_HasFocus; // Remove focus indication

    if (opt.state & QStyle::State_Selected) {
        opt.palette.setColor(QPalette::Highlight, Qt::red);
    }

    QItemDelegate::paint(painter, opt, index);
}
