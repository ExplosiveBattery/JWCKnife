#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T19:50:05
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JWCKnife
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += Sources/main.cpp\
    Sources/Network/educationsystem.cpp \
    Sources/Network/pagelist.cpp \
    Sources/Check/checker.cpp \
    Sources/function.cpp \
#    Sources/Widgets/MVD/classscheduleview.cpp
    Sources/Widgets/AccountItem.cpp \
    Sources/Widgets/LoginWindow.cpp \
    Sources/Widgets/MainWindow.cpp \
    Sources/Widgets/StackedExamRelatedWidget.cpp \
    Sources/Widgets/StackedSelectClassWidget.cpp \
    Sources/Widgets/TitleWidget.cpp \
    Sources/Widgets/MVD/AutoTurnLineDelegate.cpp \
    Sources/Widgets/ProfessionChooseForm.cpp \
    Sources/Widgets/MVD/CourseDelegate.cpp \
    Sources/Widgets/MVD/MaterialDelegate.cpp \
    Sources/Widgets/StackedEvaluationWidget.cpp \
    Sources/Widgets/MVD/EvaluationDelegate.cpp \
    Sources/Widgets/MVD/CheckBoxHeader.cpp \
    Sources/Widgets/CardArrayWidget.cpp \
    Sources/Widgets/TreeViewWidget.cpp \
    Sources/Widgets/MVD/TableToTreeProxyModel.cpp \
    Sources/Widgets/StackedPlanCoursesWidget.cpp \
    Sources/Widgets/StackedGradesWidget.cpp \
    Sources/Widgets/StackedScheduleWidget.cpp


HEADERS  +=Sources/Network/educationsystem.h \
    Sources/Network/pagelist.h \
    Sources/Check/checker.h \
#    Sources/Widgets/MVD/classscheduleview.h
    Sources/Widgets/MVD/AutoTurnLineDelegate.h \
    Sources/Widgets/AccountItem.h \
    Sources/Widgets/LoginWindow.h \
    Sources/Widgets/MainWindow.h \
    Sources/Widgets/StackedExamRelatedWidget.h \
    Sources/Widgets/TitleWidget.h \
    Sources/Widgets/StackedSelectClassWidget.h \
    Sources/Widgets/ProfessionChooseForm.h \
    Sources/Widgets/MVD/CourseDelegate.h \
    Sources/Widgets/MVD/MaterialDelegate.h \
    Sources/Widgets/StackedEvaluationWidget.h \
    Sources/Widgets/MVD/EvaluationDelegate.h \
    Sources/Widgets/MVD/CheckBoxHeader.h \
    Sources/Widgets/CardArrayWidget.h \
    Sources/Widgets/TreeViewWidget.h \
    Sources/Widgets/MVD/TableToTreeProxyModel.h \
    Sources/Widgets/StackedPlanCoursesWidget.h \
    Sources/Widgets/StackedGradesWidget.h \
    Sources/Widgets/StackedScheduleWidget.h




FORMS    += Sources/loginwindow.ui \
    Sources/mainwindow.ui \
    Sources/StackedSelectClassWidget.ui \
    Sources/StackedExamRelatedWidget.ui \
    Sources/ProfessionChooseForm.ui \
    Sources/StackedEvaluationWidget.ui \
    Sources/TreeViewWidget.ui \
    Sources/StackedGradesWidget.ui \
    Sources/StackedScheduleWidget.ui \
    Sources/StackedEvaluationWidget.ui



#RC_FILE += icon.rc



RESOURCES += \
    images.qrc \
    font.qrc
