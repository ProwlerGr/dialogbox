/*
 * GUI widgets for shell scripts - dialogbox version 1.0
 *
 * Copyright (C) 2015-2016, 2020 Andriy Martynets <andy.martynets@gmail.com>
 *------------------------------------------------------------------------------
 * This file is part of dialogbox.
 *
 * Dialogbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dialogbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dialogbox. If not, see http://www.gnu.org/licenses/.
 *------------------------------------------------------------------------------
 */

#include "dialogbox.h"

QWidget *DialogBox::findWidget(char *name)
{
    QWidget *widget = nullptr;

    chosenView = nullptr;
    chosenRowFlag = false;

    if (name) {
        int li_row = -1;
        char *li_name = nullptr;

        for (int i = 0; name[i] != 0; i++) {
            if (name[i] == '#') {
                name[i] = 0;
                if (name[i + 1])
                    sscanf(name + i + 1, "%d", &li_row);
                break;
            }
            if (name[i] == ':') {
                name[i] = 0;
                li_name = name + i + 1;
                break;
            }
        }

        if (name[0])
            for (int i = 0, j = pages.count(); i < j; i++) {
                widget = pages.at(i);
                
                // ORIGINAL: if (!strcmp(widget->objectName().toLocal8Bit().constData(), name)) {
                if (QString::fromUtf8(name) == widget->objectName()) {
                    // The widget is page
                    break;
                }
                if ( (widget = findWidgetRecursively(widget->layout(), name)) )
                    break;
            }

        switch ((unsigned)widgetType(widget)) {
        case ListBoxWidget:
            // ORIGINAL: chosenListWidget = widget->focusProxy();
            chosenListWidget = widget->focusProxy();
            chosenView = qobject_cast<ListBox *>(chosenListWidget);
            break;
        case ComboBoxWidget:
            // ORIGINAL: chosenListWidget = widget->focusProxy();
            chosenListWidget = widget->focusProxy();
            if (QComboBox *cb = qobject_cast<QComboBox *>(chosenListWidget))
                chosenView = cb->view();
            else
                chosenView = nullptr;
            break;
        }
        if (chosenView) {
            if (li_row >= 0) {
                chosenRow = li_row;
                chosenRowFlag = true;
            }
            if (li_name) {
                QAbstractItemModel *model = chosenView->model();
                int i;
                int j;
                for (i = 0, j = model->rowCount(); i < j; i++) {
                    // ORIGINAL: if (!strcmp(model->data(model->index(i, 0), Qt::DisplayRole)
                    //                                .toString().toLocal8Bit().constData(),
                    //                                li_name)) {
                    QString cell = model->data(model->index(i, 0), Qt::DisplayRole).toString();
                    if (cell == QString::fromUtf8(li_name)) {
                        chosenRow = i;
                        chosenRowFlag = true;
                        break;
                    }
                }
                if (i == j) {
                    chosenRow = i;
                    chosenRowFlag = true;
                }
            }
        }
    }

    return widget;
}
