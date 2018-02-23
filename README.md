# Generic C++ model for QML ListView

Here is an example of implementation for generic c++ model to use with `ListView` QML component.

Standard way of implementing such a model is to extend `QAbstractItemModel` or `QAbstractListModel` classes for every purpose. Such models are fast and can contain any type of underlying items and any of items' container is possible to use so they can be really cheap in memory consumption but requires lots of boilerplate work to be done. Another disadvantage is that any change in the items must be made with participation of model itself or through it to be able for the `ListView` to know about them.

Lets say we've got collection of `QObject` instances, what then? Such objects support signals and meta-property system, so they can inform the view about changes themselves and we don't need to pass these changes through the list model.

In this case we would like to have some generic type implementing both interfaces:
- for ListView to inform about items adding/removing
- kind of QList to add/remove those items easily

Unfortunately, QObject can not be made generic with standard c++ template system but we still able to use macros.

`list_model.h` and `list_model.cpp` are all you need. Header file contains just one simple macro to define your own model for any ListView model based on QObject items: `DECLARE_LIST_MODEL( NAME, ITEM_TYPE )` which you can basically understand as `class NAME<ITEM_TYPE>`. You can find additional info the header file itself.

Please, follow this project as an example of usage of such a model. Cheers!
