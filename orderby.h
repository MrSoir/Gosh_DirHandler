#ifndef ORDERBY_H
#define ORDERBY_H

#include <QString>

//namespace ORDERED_BY
//{
    enum ORDERED_BY{NAME, MOD_DATE, SIZE, TYPE};

    struct Order
    {
    public:
        Order();
        ORDERED_BY ordered_by;
        bool reversedOrdered;

        bool operator==(const Order& order)
        {
            return order.ordered_by      == this->ordered_by &&
                   order.reversedOrdered == this->reversedOrdered;
        }
        bool operator!=(const Order& order)
        {
            return order.ordered_by      != this->ordered_by ||
                   order.reversedOrdered != this->reversedOrdered;
        }

        QString reverse_str = reversedOrdered ? "true" : "false";

        QString toString()
        {
            QString order_str;
            if(this->ordered_by == ORDERED_BY::NAME)
                order_str = "NAME";
            else if(this->ordered_by == ORDERED_BY::MOD_DATE)
                order_str = "MOD_DATE";
            else if(this->ordered_by == ORDERED_BY::SIZE)
                order_str = "SIZE";
            else if(this->ordered_by == ORDERED_BY::TYPE)
                order_str = "TYPE";

            return QString("Order: %1 - reverse: %2").arg(order_str).arg(reverse_str);
        }
    };
//}

#endif // ORDERBY_H
