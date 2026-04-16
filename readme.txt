
для создания независимого exe
1. win+r
2. cmd 	- вызвать командную строку
3. C:\Qt\6.2.4\mingw_64\bin\windeployqt.exe --qmldir G:\PROGRAMING\PRODGEKT\shar\droid_control-main\pages G:\PROGRAMING\PRODGEKT\shar\Up_level\droid_stick\droid_stick.exe

3.1. первый аргумент путь - путь к windeployqt.exe
3.2. второй аргумент - команда для добавления qml файлов и путь к этим файлам
3.3. третий аргумент - путь к релизному exe файлу созданного приложения

*примечание
1. в переменной PATH должен быть прописан путь к g++
1.1. у меня он C:\Qt\Tools\mingw1120_64\bin