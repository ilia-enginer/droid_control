/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtQuick.Controls

ScrollablePage {
    id: page

    Column {
        spacing: 10
        width: parent.width


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Частота отправки команд джойстиком " + "\n" + "(" + mainModel.timer1 + "мс)"

            Component.onCompleted: {
                mainModel.onTimer1Changed.connect(modelChanged)
            }

            function modelChanged(){
                this.text = "Частота отправки команд джойстиком" + "\n" + "(" + value + "мс)"
            }
        }

        Slider {
            id: timer1Slider
            from: 100
            to: 1000
            value: mainModel.timer1
            anchors.horizontalCenter: parent.horizontalCenter
            onMoved: {mainModel.timer1 = value}

            Component.onCompleted: {
                mainModel.onTimer1Changed.connect(modelChanged)
            }

            function modelChanged(){
                this.value = value
            }
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда джойстика" + "\n" + "(" + Math.round(mainModel.joystickAmplitude) + ")"

            Component.onCompleted: {
                mainModel.joystickAmplitudeChanged.connect(modelChanged)
            }

            function modelChanged(){
                this.text = "Амплитуда джойстика" + "\n" + "(" + Math.round(value) + ")"
            }
        }

        Slider {
            id: timer2Slider
            value: mainModel.joystickAmplitude
            from: 1
            to: 100
            anchors.horizontalCenter: parent.horizontalCenter
            onMoved: {mainModel.joystickAmplitude = value}

            Component.onCompleted: {
                mainModel.joystickAmplitudeChanged.connect(modelChanged)
            }

            function modelChanged(){
                this.value = value
            }
        }
    }
}
