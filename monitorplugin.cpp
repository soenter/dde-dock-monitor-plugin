/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "monitorplugin.h"
#include "interfaces/dockconstants.h"
#include<QTimer>
#include <QCoreApplication>

using namespace Dock;

const QString PLUGIN_ID = "monitor_plugin_id";
static const QString MenuIdShowMem = "id_show_mem:checkbox:show";
static const QString MenuIdShowNet = "id_show_net:checkbox:show";
static const QString MenuIdShowMoritot = "id_open_monitor";


MonitorPlugin::MonitorPlugin(QObject *parent)
    : QObject(parent)
{
    memshowed=true;//默认开启

    initSettings();
  //  m_settings->setValue("enabled", true);
    monitor = new watch(memshowed,netshowed);
    if(memshowed&&netshowed) monitor->setFixedSize(66,28);
    else if(memshowed) monitor->setFixedSize(28,28);
    else if(netshowed) monitor->setFixedSize(36,28);

}
MonitorPlugin::~MonitorPlugin()
{
    delete monitor;
}

QString MonitorPlugin::getPluginName()
{
    return QString(tr("内存网速监测"));
}

void MonitorPlugin::init(DockPluginProxyInterface *proxy)
{
    m_proxy = proxy;
    m_proxy->infoChangedEvent(DockPluginInterface::InfoTypeConfigurable, PLUGIN_ID);
    onEnabledChanged();

   // m_proxy->itemAddedEvent(PLUGIN_ID);
    //changeMode(m_proxy->dockMode(), m_proxy->dockMode());以后可能有用，暂时没对时尚模式做适配。
}


void MonitorPlugin::initSettings()
{
    m_settings = new QSettings("deepin", "dde-dock-monitor-plugin", this);

    if (!QFile::exists(m_settings->fileName())) {
        m_settings->setValue("showMem", true);
        m_settings->setValue("showNet", true);
    }
    else{
        memshowed = m_settings->value("showMem",QVariant(true)).toBool();
        netshowed = m_settings->value("showNet",QVariant(true)).toBool();
    }
}
void MonitorPlugin::changeMode(Dock::DockMode newMode, Dock::DockMode oldMode)
{
    Q_UNUSED(oldMode)
    Q_UNUSED(newMode)

//    switch (newMode)
//    {
//    case Dock::FashionMode:
//        monitor->setFixedSize(73,30);
//       // m_mainWidget->setPixmap(QPixmap(":/icons/icons/fashion.svg"));
//        break;
//    case Dock::ClassicMode:
//    case Dock::EfficientMode:
//        monitor->setFixedSize(73,30);
//        //m_mainWidget->setPixmap(QPixmap(":/icons/icons/normal.svg"));
//        break;
//    }

    //m_proxy->infoChangedEvent(DockPluginInterface::InfoTypeItemSize, PLUGIN_ID);
}

QStringList MonitorPlugin::ids()
{
    return QStringList(PLUGIN_ID);
}

QString MonitorPlugin::getName(QString id)
{
    Q_UNUSED(id)

    return getPluginName();
}

QString MonitorPlugin::getTitle(QString id)
{
    Q_UNUSED(id)

    return QString();
}

QString MonitorPlugin::getCommand(QString id)
{
    Q_UNUSED(id)
    return QString();
}

QWidget *MonitorPlugin::getItem(QString id)
{
    Q_UNUSED(id)

    return monitor;// m_mainWidget;
}

QWidget *MonitorPlugin::getApplet(QString id)
{
    Q_UNUSED(id)

    return nullptr;
}

QString MonitorPlugin::getMenuContent(QString id)
{
    Q_UNUSED(id)
    QJsonObject contentObj;

    QJsonArray items;

//    if (m_mode == Dock::FashionMode) {
//        items.append(createMenuItem(MenuIdSwitchDisplayMode, tr("Switch display mode")));
//    } else {
//        items.append(createMenuItem(MenuIdShowWeek, tr("Show week"), true, showWeek()));
//        items.append(createMenuItem(MenuIdShowDate, tr("Show date"), true, showDate()));
//    }

    items.append(createMenuItem(MenuIdShowMem, tr("显示内存占用"),true,showmem()));
    items.append(createMenuItem(MenuIdShowNet, tr("显示网速"),true,shownet()));
    items.append(createMenuItem(MenuIdShowMoritot, tr("系统监视器")));

    contentObj.insert("items", items);

    return QString(QJsonDocument(contentObj).toJson());
}

void MonitorPlugin::invokeMenuItem(QString id, QString itemId, bool checked)
{
    if (itemId == MenuIdShowMem) {
        setshowmem(checked);
    } else if (itemId == MenuIdShowNet) {
       setshownet(checked);
    } else if (itemId == MenuIdShowMoritot) {
        QProcess::startDetached("gnome-system-monitor");
    }

}

void MonitorPlugin::setEnabled(const QString &id, bool enable)
{
    if (id != PLUGIN_ID)
        return;
    m_settings->setValue("enabled", enable);

    onEnabledChanged();


}

void MonitorPlugin::onEnabledChanged()
{

    m_proxy->itemRemovedEvent(PLUGIN_ID);
    monitor->setParent(NULL);

    if (enabled(PLUGIN_ID)) {
        m_proxy->itemAddedEvent(PLUGIN_ID);
    }

    m_proxy->infoChangedEvent(DockPluginInterface::InfoTypeEnable, PLUGIN_ID);
}

bool MonitorPlugin::configurable(const QString &id)
{
    Q_UNUSED(id)

    return true;
}

bool MonitorPlugin::enabled(const QString &id)
{
    Q_UNUSED(id)

    return m_settings->value("enabled", QVariant(true)).toBool();
}

QPixmap MonitorPlugin::getIcon(QString id)
{
    Q_UNUSED(id);
    QPixmap pix;
    pix.load(":/monitor.png");
    return pix;
}

QJsonObject MonitorPlugin::createMenuItem(QString itemId, QString itemName, bool checkable, bool checked)
{
    QJsonObject itemObj;

    itemObj.insert("itemId", itemId);
    itemObj.insert("itemText", itemName);
    itemObj.insert("itemIcon", "");
    itemObj.insert("itemIconHover", "");
    itemObj.insert("itemIconInactive", "");
    itemObj.insert("itemExtra", "");
    itemObj.insert("isActive", true);
    itemObj.insert("isCheckable", checkable);
    itemObj.insert("checked", checked);
    itemObj.insert("itemSubMenu", QJsonObject());

    return itemObj;
}

bool MonitorPlugin::showmem(){
    return  m_settings->value("showMem").toBool();
}
bool MonitorPlugin::shownet()
{
    return  m_settings->value("showNet").toBool();
}
void MonitorPlugin::setshowmem(bool checked){
    if(m_settings->value("showNet").toBool()==false) return;//另一个不存在，别指望能开启/关闭
    m_settings->setValue("showMem",checked);
    monitor->setmemshow(checked);
    if(checked)  monitor->setFixedSize(66,28);
    else monitor->setFixedSize(36,28);//check=false 证明只有net，设36
    m_proxy->infoChangedEvent(DockPluginInterface::InfoTypeItemSize, PLUGIN_ID);

}
void MonitorPlugin::setshownet(bool checked){
    if(m_settings->value("showMem").toBool()==false) return;
    m_settings->setValue("showNet",checked);
    monitor->setnetshow(checked);
    if(checked)  monitor->setFixedSize(66,28);
    else monitor->setFixedSize(28,28);//check=false 证明只有mem，设28
    m_proxy->infoChangedEvent(DockPluginInterface::InfoTypeItemSize, PLUGIN_ID);
}
