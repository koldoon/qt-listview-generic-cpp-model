import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import App.Provider 1.0

Window {
    visible: true
    width: 500
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
            text: "remove item from center"
            onClicked: provider.removeItem()
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Count: " + provider.itemsModel.length
        }
    }

    ListView {
        anchors.fill: parent
        anchors.topMargin: buttons.implicitHeight + 10
        anchors.bottomMargin: 10
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        cacheBuffer: 2

        // model of "itemsIndex" provides indices only.
        // also it dispatches all necessary signals
        // that allow view to react on indices changes
        model: provider.itemsModel

        delegate: DataItemDelegate {    // delegate must be placed in separate file
            anchors.left: parent.left
            anchors.right: parent.right

            // using exposed method "item()" delegate
            // gets data item instance from the collection
            // inside the provider
            item: provider.itemsModel.item(index)    // "index" can be used only here
        }
    }
}
