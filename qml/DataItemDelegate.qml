import QtQuick 2.9
import QtQuick.Controls 2.2
import App 1.0    // IMPORTANT! DataItem is declared there.

Row {
    // Typed "item" property.
    // It is possible to use bi-directional binding with this property
    // since it is declared as a pointer to instance.
    // : DataItem
    property DataItem item
    //    property var item: {
    //        "id": 0,
    //        "value": 42,
    //        "doubleId": function() {}
    //    }

    height: implicitHeight
    spacing: 5
    padding: 5

    Text {
        text: item.id
        width: 50
        padding: 10
    }

    Rectangle {
        width: 200
        height: input.implicitHeight
        color: "#D8D8D8"
        border.color: "#B6B5B5"

        TextInput {
            id: input
            anchors.fill: parent
            padding: 10
            text: item.value                    // Using structure properties
            onTextChanged: item.value = text    // Changing structure properties
        }
    }

    Text {
        text: "<- Change Me"
        opacity: 0.5
        anchors.verticalCenter: parent.verticalCenter
    }

    Button {
        anchors.verticalCenter: parent.verticalCenter
        height: input.implicitHeight
        text: "double id"
        onClicked: item.doubleId()      // Executing business logic
    }
}
