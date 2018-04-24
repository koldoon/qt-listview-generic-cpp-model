import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import App 1.0

Window {
    visible: true
    width: 600
    height: 500
    title: qsTr("c++ generic list model example")

    Provider {
        id: provider

        // Provider - is our basic implementation of c++ view controller
        // that exposes some collection-like data of struct items.
        // + itemsModel is used to utilize ListView.model
        // + itemsModel.item(index) is used to get particular item by its index for delegate
    }


    Row {
        id: buttons
        spacing: 20
        padding: 10

        Button {
            text: "add item"
            onClicked: provider.addItem()
        }

        Button {
            text: "add 3"
            onClicked: provider.addItems3()
        }

        Button {
            text: "change center"
            onClicked: provider.changeItem()
        }

        Button {
            text: "remove center"
            onClicked: provider.removeItem()
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Count: " + provider.items.length
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: buttons.implicitHeight + 10
        anchors.bottomMargin: 10
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        cacheBuffer: 2

        // model of "itemsIndex" provides indices only.
        // also it dispatches all necessary signals
        // that allow view to react on indices changes
        model: provider.items

        delegate: DataItemDelegate {
            d: provider.items.item(index)
            anchors.left: parent.left
            anchors.right: parent.right
        }

        removeDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
        }
    }
}
