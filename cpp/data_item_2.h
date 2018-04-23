#pragma once

#include <QObject>

namespace app {
    class DataItem2 : public QObject {
        Q_OBJECT
        Q_PROPERTY( int dat MEMBER dat CONSTANT )

    public:
        int dat;
    };
}
