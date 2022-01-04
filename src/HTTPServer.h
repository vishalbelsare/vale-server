#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <httplib.h>

#include <QObject>
#include <QString>
#include <QStringList>

#include "TextLogger.h"
#include "ValeManager.h"
#include "ManagedProcess.h"

class HTTPServer : public QObject {
 public:
  HTTPServer(QString dirpath, QString version, QObject *parent);
  ~HTTPServer() Q_DECL_OVERRIDE;

  bool start(int port);
  bool is_running();

  void stop();
  void log(QString msg);

  void set_base_dir(const QString &base);
  void config(const QString &config);

  void project(QString project) { _project = project; }
  void mode(QString mode) { _mode = mode; }

  QString stylesPath;

  ValeManager *vale;

 private:
  // Endpoints [POST]:
  void onVale(const httplib::Request &req, httplib::Response &res);
  void onFile(const httplib::Request &req, httplib::Response &res);
  void onHTML(const httplib::Request &req, httplib::Response &res);
  void onUpdate(const httplib::Request &req, httplib::Response &res);
  void onInstall(const httplib::Request &req, httplib::Response &res);
  void onSuggest(const httplib::Request &req, httplib::Response &res);
  void onTest(const httplib::Request &req, httplib::Response &res);
  void onSave(const httplib::Request &req, httplib::Response &res);
  void onOpen(const httplib::Request &req, httplib::Response &res);
  void onRun(const httplib::Request &req, httplib::Response &res);
  void onCompile(const httplib::Request &req, httplib::Response &res);
  void onTag(const httplib::Request &req, httplib::Response &res);
  void onUpload(const httplib::Request &req, httplib::Response &res);

  // Endpoints [GET]:
  void onProjects(const httplib::Request &req, httplib::Response &res);
  void onProject(const httplib::Request &req, httplib::Response &res);
  void onConfig(const httplib::Request &req, httplib::Response &res);
  void onLibrary(const httplib::Request &req, httplib::Response &res);
  void onPath(const httplib::Request &req, httplib::Response &res);
  void onVocab(const httplib::Request &req, httplib::Response &res);
  void onAddons(const httplib::Request &req, httplib::Response &res);
  void onReport(const httplib::Request &req, httplib::Response &res);
  void onVersion(const httplib::Request &req, httplib::Response &res);
  void onNewVocab(const httplib::Request &req, httplib::Response &res);
  void onSystem(const httplib::Request &req, httplib::Response &res);

  httplib::Server svr;
  TextLogger *logger;
  ManagedProcess *mp;

  QString _config;
  QString _project;
  QString _mode;
  QString _version;

  bool _writable;
};

#endif
