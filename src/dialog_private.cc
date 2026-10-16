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

void DialogBox::printWidget(QWidget *widget)
{
    if (widget->isEnabled()) {
        // ORIGINAL: const char *name = widget->objectName().toLocal8Bit().constData();
        QByteArray nameBa = widget->objectName().toLocal8Bit();
        const char *name = nameBa.constData();

        if (name[0]) {
            QWidget *proxyWidget;
            const QMetaObject *metaObj = widget->metaObject();
            int propertyIndex;

            if (metaObj->property(metaObj->indexOfProperty("checkable"))
                .read(widget).toBool()) {
                fprintf(output, "%s=%s\n", name,
                        metaObj->property(metaObj->indexOfProperty("checked"))
                        .read(widget).toBool() ? "1" : "0");
                fflush(output);
                return;
            }

            if ((propertyIndex = metaObj->indexOfProperty("value")) >= 0
                && widgetType(widget) != ProgressBarWidget) {
                fprintf(output, "%s=%d\n", name,
                        metaObj->property(propertyIndex).read(widget).toInt());
                fflush(output);
                return;
            }

            if ((proxyWidget = widget->focusProxy())) {
                QListWidgetItem *item;

                fprintf(output, "%s=", name);
                switch ((unsigned)widgetType(proxyWidget)) {
                case ComboBoxWidget: {
                    // ORIGINAL: ((QComboBox *)proxyWidget)->currentText().toLocal8Bit().constData()
                    QComboBox *cb = qobject_cast<QComboBox *>(proxyWidget);
                    QString cur = cb ? cb->currentText() : QString();
                    QByteArray curBa = cur.toLocal8Bit();
                    fprintf(output, "%s\n", curBa.constData());
                    break;
                }
                case ListBoxWidget: {
                    QListWidget *lw = qobject_cast<QListWidget *>(proxyWidget);
                    item = lw ? lw->currentItem() : nullptr;
                    // ORIGINAL: item ? item->text().toLocal8Bit().constData() : ""
                    QString text = item ? item->text() : QString();
                    QByteArray textBa = text.toLocal8Bit();
                    fprintf(output, "%s\n", textBa.constData());
                    break;
                }
                default: {
                    metaObj = proxyWidget->metaObject();
                    // ORIGINAL: metaObj->property(metaObj->indexOfProperty("text")).read(proxyWidget).toString().toLocal8Bit().constData()
                    QVariant prop = metaObj->property(metaObj->indexOfProperty("text")).read(proxyWidget);
                    QString s = prop.toString();
                    QByteArray sBa = s.toLocal8Bit();
                    fprintf(output, "%s\n", sBa.constData());
                    break;
                }
                }
                fflush(output);
                return;
            }
        }
    }
}
