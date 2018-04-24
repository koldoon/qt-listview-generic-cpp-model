import QtQuick 2.9
import QtQuick.Controls 2.2
import App 1.0    // IMPORTANT! DataItem is declared there.

Row {
    // Typed "modelData" property.
    // It is possible to use bi-directional binding with this property
    // since it is declared as a pointer to instance.
    //    property var modelData: {
    //        "id": 2,
    //        "value": 42,
    //        "doubleId": function() { console.log("id doubled") }
    //    }

    property var d

    height: implicitHeight
    spacing: 5
    padding: 5

    Text {
        text: modelData.id
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
            text: modelData.value                    // Using structure properties
            onTextChanged: modelData.value = text    // Changing structure properties
        }
    }

    Button {
        anchors.verticalCenter: parent.verticalCenter
        height: input.implicitHeight
        text: "double id"
        onClicked: modelData.doubleId()      // Executing business logic
    }
}
