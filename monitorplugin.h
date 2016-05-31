/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MonitorPlugin_H
#define MonitorPlugin_H

#include <QObject>
#include<watch.h>
#include <dde-dock/dockconstants.h>
#include "interfaces/dockplugininterface.h"
#include "interfaces/dockpluginproxyinterface.h"
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
class MonitorPlugin : public QObject, public DockPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.deepin.Dock.PluginInterface" FILE "dde-dock-monitor-plugin.json")
    Q_INTERFACES(DockPluginInterface)

public:
    explicit MonitorPlugin(QObject *parent = 0);
    ~MonitorPlugin() Q_DECL_OVERRIDE;

    void init(DockPluginProxyInterface *proxy) Q_DECL_OVERRIDE;
    void changeMode(Dock::DockMode newMode, Dock::DockMode oldMode) Q_DECL_OVERRIDE;
    void invokeMenuItem(QString id, QString itemId, bool checked) Q_DECL_OVERRIDE;
    void setEnabled(const QString &id, bool enable) Q_DECL_OVERRIDE;
    bool configurable(const QString &id) Q_DECL_OVERRIDE;
    bool enabled(const QString &id) Q_DECL_OVERRIDE;
    QPixmap getIcon(QString id) Q_DECL_OVERRIDE;
    QString getPluginName() Q_DECL_OVERRIDE;
    QString getName(QString id) Q_DECL_OVERRIDE;
    QString getTitle(QString id) Q_DECL_OVERRIDE;
    QString getCommand(QString id) Q_DECL_OVERRIDE;
    QString getMenuContent(QString id) Q_DECL_OVERRIDE;
    QStringList ids() Q_DECL_OVERRIDE;
    QWidget *getItem(QString id) Q_DECL_OVERRIDE;
    QWidget *getApplet(QString id) Q_DECL_OVERRIDE;
private slots:
    void onInitTimerTriggered();


private:
    bool showmem();
    bool shownet();
    void setshowmem(bool);
    void setshownet(bool);
    void onEnabledChanged();
    QJsonObject createMenuItem(QString itemId,
                               QString itemName,
                               bool checkable = false,
                               bool checked = false);
    DockPluginProxyInterface *m_proxy;
    watch* monitor;
    QSettings *m_settings = NULL;
    void initSettings()  ;
    bool memshowed,netshowed;
};

#endif // MonitorPlugin_H
