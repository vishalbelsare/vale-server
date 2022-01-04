#include "HTTPServer.h"

#include <QDesktopServices>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

/**
 * Set our API headers.
 */
void setHeaders(httplib::Response &res) {
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_header("Allow", "GET, POST, HEAD, OPTIONS");
  res.set_header("Access-Control-Allow-Headers", "*");
  res.set_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, HEAD");
  res.set_header("Access-Control-Expose-Headers", "*");
}

/**
 * Server initialization.
 *
 * See https://errata-ai.github.io/vale-server/api/index.html
 */
HTTPServer::HTTPServer(QString dirpath, QString version, QObject *parent)
    : QObject(parent) {
  this->logger = new TextLogger(this);
  this->mp = new ManagedProcess(this);

  this->vale = new ValeManager(dirpath, this->logger, this->mp);

  _version = version;

  QString data =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "report.json";

  if (!QFile::exists(data)) {
    QFile f(":/config/report.json");
    if (!f.open(QIODevice::ReadOnly)) {
      this->logger->write("Failed to load default report config");
    } else {
      QString json = QString::fromStdString(f.readAll().toStdString());

      QFile results(data);
      results.open(QIODevice::WriteOnly | QIODevice::Truncate);
      results.write(json.toUtf8());
      results.close();
    }
  }

  // Set up our API endpoints:
  svr.Post("/vale", [&](const httplib::Request &req,
                        httplib::Response &res) { onVale(req, res); })
      .Post("/update", [&](const httplib::Request &req,
                           httplib::Response &res) { onUpdate(req, res); })
      .Post("/install", [&](const httplib::Request &req,
                            httplib::Response &res) { onInstall(req, res); })
      .Post("/suggest", [&](const httplib::Request &req,
                            httplib::Response &res) { onSuggest(req, res); })
      .Post("/file", [&](const httplib::Request &req,
                         httplib::Response &res) { onFile(req, res); })
      .Post("/html", [&](const httplib::Request &req,
                         httplib::Response &res) { onHTML(req, res); })
      .Post("/save", [&](const httplib::Request &req,
                         httplib::Response &res) { onSave(req, res); })
      .Post("/open", [&](const httplib::Request &req,
                         httplib::Response &res) { onOpen(req, res); })
      .Post("/run", [&](const httplib::Request &req,
                        httplib::Response &res) { onRun(req, res); })
      .Post("/compile", [&](const httplib::Request &req,
                            httplib::Response &res) { onCompile(req, res); })
      .Post("/tag", [&](const httplib::Request &req,
                            httplib::Response &res) { onTag(req, res); })
      .Post("/upload", [&](const httplib::Request &req,
                        httplib::Response &res) { onUpload(req, res); })

      .Get("/projects", [&](const httplib::Request &req,
                            httplib::Response &res) { onProjects(req, res); })
      .Get("/project", [&](const httplib::Request &req,
                           httplib::Response &res) { onProject(req, res); })
      .Get("/config", [&](const httplib::Request &req,
                          httplib::Response &res) { onConfig(req, res); })
      .Get("/library", [&](const httplib::Request &req,
                           httplib::Response &res) { onLibrary(req, res); })
      .Get("/path", [&](const httplib::Request &req,
                        httplib::Response &res) { onPath(req, res); })
      .Get("/vocab", [&](const httplib::Request &req,
                         httplib::Response &res) { onVocab(req, res); })
      .Get("/newVocab", [&](const httplib::Request &req,
                            httplib::Response &res) { onNewVocab(req, res); })
      .Get("/report", [&](const httplib::Request &req,
                          httplib::Response &res) { onReport(req, res); })
      .Get("/version", [&](const httplib::Request &req,
                           httplib::Response &res) { onVersion(req, res); })
      .Get("/system", [&](const httplib::Request &req, httplib::Response &res) {
        onSystem(req, res);
      });
}

HTTPServer::~HTTPServer() {
  if (svr.is_running()) {
    svr.stop();
  }
  delete this->vale;
  delete this->logger;
}

/**
 * Update the server's active configuration.
 */
void HTTPServer::config(const QString &config) {
  _config = config;

  QString data =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "_vale.ini";

  QFile results(data);
  results.open(QIODevice::WriteOnly | QIODevice::Truncate);
  results.write(_config.toUtf8());
  results.close();

  QSettings settings(data, QSettings::IniFormat);
  stylesPath = settings.value("StylesPath").toString();

  _writable = false;
  QTemporaryFile path(QString("%1/XXXXXX.txt").arg(stylesPath));
  if (path.open()) {
    _writable = true;
  }
}

/**
 * Set the server's base directory to the Vale Dashboard.
 */
void HTTPServer::set_base_dir(const QString &base) {
  svr.set_base_dir(base.toStdString().c_str());
}

/**
 * Stop the server.
 */
void HTTPServer::stop() {
  if (svr.is_running()) {
    svr.stop();
  }
}

void HTTPServer::log(QString msg) { this->logger->write(msg); }

/**
 * Start the server and all installed add-ons.
 */
bool HTTPServer::start(int port) { return svr.listen("127.0.0.1", port); }

/**
 * Check if the server is running.
 */
bool HTTPServer::is_running() { return svr.is_running(); }

/**
 * [GET] /project
 *
 * Get the active project's name.
 */
void HTTPServer::onProject(const httplib::Request & /* req */,
                           httplib::Response &res) {
  setHeaders(res);

  res.set_content("{\"project\": \"" + _project.toStdString() + "\"}",
                  "application/json");
}

/**
 * [POST] /upload?text={<string>}&regex={<string>}
 *
 * Upload the given regex to regex101.
 */
void HTTPServer::onUpload(const httplib::Request &req,
                           httplib::Response &res) {
    setHeaders(res);

    res.status = req.has_param("regex") && req.has_param("text")  ? 200 : 400;
    try {
        if (res.status == 200) {
            QString regex = QString::fromStdString(req.get_param_value("regex"));
            QString text = QString::fromStdString(req.get_param_value("text"));

            QTemporaryFile config(QString("%1/XXXXXX.txt").arg(QDir::tempPath()));
            if (config.open()) {
                QTextStream stream(&config);
                stream << text << Qt::endl;
                stream.flush();
                config.seek(0);

                QString file = config.fileName();
                res.set_content(this->vale->lib({"upload", regex, file}),
                                "application/json");
            } else {
                res.set_content("{\"error\": \"no file\"}", "application/json");
            }
        } else {
            res.set_content("{\"error\": \"missing 'text' or 'regex'\"}",
                            "application/json");
        }
    } catch (const std::exception &e) {
        res.status = 500;
        res.set_content("{\"error\": \"server error\"}", "application/json");
    }
}

/**
 * [GET] /system
 *
 * Get information about Vale Server's environment.
 */
void HTTPServer::onSystem(const httplib::Request & /* req */,
                          httplib::Response &res) {
  setHeaders(res);
  QVariantMap system;

  system.insert("platform", QSysInfo::prettyProductName());
  system.insert("server", _version);
  system.insert("vale", this->vale->getVersion().toString());
  system.insert("asciidoctor", this->mp->runProc("asciidoctor", "", {"-v"}));
  system.insert("rst2html",
                this->mp->runProc("rst2html.py", "", {"--version"}));
  system.insert("dita", this->mp->runProc("dita", "", {"version"}));
  system.insert("xsltproc", this->mp->runProc("xsltproc", "", {"-V"}));

  QJsonDocument doc = QJsonDocument::fromVariant(system);
  QString strJson(doc.toJson(QJsonDocument::Compact));

  res.set_content(strJson.toStdString(), "application/json");
}

/**
 * [GET] /version
 *
 * Get the Vale and Vale Server versions.
 */
void HTTPServer::onVersion(const httplib::Request & /* req */,
                           httplib::Response &res) {
  setHeaders(res);

  res.set_content("{\"server\": \"" + _version.toStdString() + "\"}",
                  "application/json");
}

/**
 * [GET] /vocab?name={<style>}&file={accept|reject}
 *
 * Get the vocab file (either accept.txt or reject.txt) for the given style.
 */
void HTTPServer::onVocab(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("name") && req.has_param("file") ? 200 : 400;
  if (res.status == 200) {
    QString name = QString::fromStdString(req.get_param_value("name"));
    QString file = QString::fromStdString(req.get_param_value("file"));
    res.set_content(vale->lib({"get-vocab", stylesPath, name, file}),
                    "application/json");
  } else if (req.has_param("name")) {
    res.set_content("{\"error\": \"missing 'file'\"}", "application/json");
  } else {
    res.set_content("{\"error\": \"missing 'name'\"}", "application/json");
  }
}

/**
 * [GET] /newVocab?name={<name>}
 *
 * Create a new with the given name.
 */
void HTTPServer::onNewVocab(const httplib::Request &req,
                            httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("name") ? 200 : 400;
  if (res.status == 200) {
    QString name = QString::fromStdString(req.get_param_value("name"));
    res.set_content(vale->lib({"new-project", stylesPath, name}),
                    "application/json");
  } else {
    res.set_content("{\"error\": \"missing 'name'\"}", "application/json");
  }
}

/**
 * [GET] /path
 *
 * Get the platform-specific, default `StylesPath`.
 *
 * NOTE: We need to use `fromNativeSeparators` to ensure that we always
 *       return valid JSON, even for Windows' paths.
 */
void HTTPServer::onPath(const httplib::Request & /* req */,
                        httplib::Response &res) {
  setHeaders(res);

  QString access = "no";
  if (_writable) {
    access = "yes";
  }

  res.set_content("{\"path\": \"" +
                      QDir::fromNativeSeparators(stylesPath).toStdString() +
                      "\", \"writable\": \"" + access.toStdString() + "\"}",
                  "application/json");
}

/**
 * [GET] /library
 *
 * Get the current styles library from Vale.
 */
void HTTPServer::onLibrary(const httplib::Request & /* req */,
                           httplib::Response &res) {
  setHeaders(res);
  res.set_content(vale->lib({"ls-library", stylesPath}),
                  "application/json");
}

/**
 * [GET] /config
 *
 * Get the current command-line configuration from Vale.
 */
void HTTPServer::onConfig(const httplib::Request & /* req */,
                          httplib::Response &res) {
  setHeaders(res);

  res.set_content(vale->lint({"ls-config"}), "application/json");
}

/**
 * [GET] /projects
 *
 * Get all available projects from Vale.
 */
void HTTPServer::onProjects(const httplib::Request & /* req */,
                            httplib::Response &res) {
  setHeaders(res);
  res.set_content(vale->lib({"ls-projects", stylesPath}),
                  "application/json");
}

/**
 * [POST] /install?name={<style>}&link={<url>}
 *
 * Install the given style through Vale.
 */
void HTTPServer::onInstall(const httplib::Request &req,
                           httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("name") && req.has_param("link") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString name = QString::fromStdString(req.get_param_value("name"));
      QString link = QString::fromStdString(req.get_param_value("link"));

      res.set_content(vale->lint({"install", name, link}), "application/json");
    } else {
      res.set_content("{\"error\": \"missing 'name' or 'link'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST] /run?code={<string>}&text={<string>}
 *
 * Run the given rule on the provided text.
 */
void HTTPServer::onRun(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("code") && req.has_param("text") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString code = QString::fromStdString(req.get_param_value("code"));
      QString text = QString::fromStdString(req.get_param_value("text"));

      QTemporaryFile codef(QString("%1/XXXXXX.yml").arg(QDir::tempPath()));
      QTemporaryFile textf(QString("%1/XXXXXX.md").arg(QDir::tempPath()));

      if (codef.open() && textf.open()) {
        QTextStream stream1(&codef);
        stream1 << code << Qt::endl;
        stream1.flush();
        codef.seek(0);

        QTextStream stream2(&textf);
        stream2 << text << Qt::endl;
        stream2.flush();
        textf.seek(0);

        QString codep = codef.fileName();
        QString textp = textf.fileName();

        std::string s = vale->command({"--output=JSON", "run", codep, textp});
        res.set_content(s, "application/json");
      } else {
        res.set_content("{\"error\": \"no file\"}", "application/json");
      }
    } else {
      res.set_content("{\"error\": \"missing 'text' or 'code'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST] /compile?code={<string>}
 *
 * Compile the given rule and report any errors.
 */
void HTTPServer::onCompile(const httplib::Request &req,
                           httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("code") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString code = QString::fromStdString(req.get_param_value("code"));
      QTemporaryFile config(QString("%1/XXXXXX.yml").arg(QDir::tempPath()));
      if (config.open()) {
        QTextStream stream(&config);
        stream << code << Qt::endl;
        stream.flush();
        config.seek(0);

        QString file = config.fileName();

        std::string s = vale->command({"--output=JSON", "compile", file});
        res.set_content(s, "application/json");
      } else {
        res.set_content("{\"error\": \"no file\"}", "application/json");
      }
    } else {
      res.set_content("{\"error\": \"missing 'text' or 'format'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST] /tag?text={<string>}&lang={<string>}
 *
 * Return part-of-speech tags for the provided text.
 */
void HTTPServer::onTag(const httplib::Request &req,
                           httplib::Response &res) {
    setHeaders(res);

    res.status = req.has_param("text") && req.has_param("lang") ? 200 : 400;
    try {
        if (res.status == 200) {
            QString text = QString::fromStdString(req.get_param_value("text"));
            QString lang = QString::fromStdString(req.get_param_value("lang"));
            QString endp = QString::fromStdString(req.get_param_value("endp"));

            QTemporaryFile config(QString("%1/XXXXXX.yml").arg(QDir::tempPath()));
            if (config.open()) {
                QTextStream stream(&config);
                stream << text << Qt::endl;
                stream.flush();
                config.seek(0);

                QString file = config.fileName();
                std::string s = vale->command({"--output=JSON", "tag", file, lang, endp});
                res.set_content(s, "application/json");
            } else {
                res.set_content("{\"error\": \"no file\"}", "application/json");
            }
        } else {
            res.set_content("{\"error\": \"missing 'text'\"}",
                            "application/json");
        }
    } catch (const std::exception &e) {
        res.status = 500;
        res.set_content("{\"error\": \"server error\"}", "application/json");
    }
}

/**
 * [POST] /suggest?alert={<alert>}
 *
 * Suggest possible fixes for the given alert.
 */
void HTTPServer::onSuggest(const httplib::Request &req,
                           httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("alert") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString alert = QString::fromStdString(req.get_param_value("alert"));
      res.set_content(vale->command({"suggest", alert}), "application/json");
    } else {
      res.set_content("{\"error\": \"missing 'alert'\"}", "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST] /update?path={<file>}&text={<string>}
 *
 * Update the given Vocab file through Vale.
 *
 * TODO: Re-name to `/vocab`?
 */
void HTTPServer::onUpdate(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("path") && req.has_param("text") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString rule = QString::fromStdString(req.get_param_value("path"));
      QString text = QString::fromStdString(req.get_param_value("text"));
      res.set_content(vale->lib({"update-vocab", stylesPath, rule, text}),
                      "application/json");
    } else {
      res.set_content("{\"error\": \"missing 'path' or 'text'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST]
 *
 * /vale?file={<path>}
 *
 * Lint the given file with Vale.
 */
void HTTPServer::onFile(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("file") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString file = QString::fromStdString(req.get_param_value("file"));
      QString json = "";

      QString ini = _config;
      if (_mode != "Server" && req.has_param("path")) {
        QString path = QString::fromStdString(req.get_param_value("path"));
        if (_mode == "Compatibility") {
          // Compatibility
          json = QString::fromStdString(
              vale->lint({"--output=JSON", "--mode-compat", file}, path));
        } else {
          // Command Line
          json = QString::fromStdString(
              vale->lint({"--output=JSON", file}, path, true));
        }
      } else {
        // Server
        json = QString::fromStdString(
            vale->lint({"--output=JSON", file}, "", false));
      }

      if (req.has_param("raw")) {
        res.set_content(json.toStdString(), "application/json");
      } else {
        QString data =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
            QDir::separator() + "results.json";

        QFile results(data);
        results.open(QIODevice::WriteOnly | QIODevice::Truncate);
        results.write(json.toUtf8());
        results.close();

        res.set_content("{\"path\": \"" +
                            QDir::fromNativeSeparators(data).toStdString() +
                            "\"}",
                        "application/json");
      }
    } else {
      res.set_content("{\"error\": \"missing 'file'\"}", "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST]
 *
 * Open the given file.
 */
void HTTPServer::onOpen(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("path") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString path = QString::fromStdString(req.get_param_value("path"));
      QDesktopServices::openUrl(QUrl::fromLocalFile(path));
      res.set_content("{\"status\": \"opened file\"}", "application/json");
    } else {
      res.set_content("{\"error\": \"missing 'path'\"}", "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST]
 *
 * Save the given report configuration.
 */
void HTTPServer::onSave(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("report") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString json = QString::fromStdString(req.get_param_value("report"));

      QString data =
          QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
          QDir::separator() + "report.json";

      QFile results(data);
      results.open(QIODevice::WriteOnly | QIODevice::Truncate);
      results.write(json.toUtf8());
      results.close();

      res.set_content("{\"path\": \"" +
                          QDir::fromNativeSeparators(data).toStdString() +
                          "\"}",
                      "application/json");
    } else {
      res.set_content("{\"error\": \"missing 'report'\"}", "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [GET]
 *
 * Retrieve the given report configuration.
 */
void HTTPServer::onReport(const httplib::Request & /* req */,
                          httplib::Response &res) {
  setHeaders(res);

  res.status = 200;
  try {
    QString data =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
        QDir::separator() + "report.json";

    QString saved = "";

    // Load our default config file.
    QFile f(data);
    if (!f.open(QIODevice::ReadOnly)) {
      this->logger->write("Failed to read report config");
    } else {
      saved = QString::fromStdString(f.readAll().toStdString());
    }

    res.set_content(saved.toStdString(), "application/json");
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST]
 *
 * /vale?format={.md|.rst|.adoc|.html}&config={<string>}&text={<string>}
 *
 * Lint the given text according to its given format and config (optional)
 * with Vale.
 */
void HTTPServer::onVale(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("format") && req.has_param("text") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString text = QString::fromStdString(req.get_param_value("text"));
      QString tfmt = QString::fromStdString(req.get_param_value("format"));

      QString ini = _config;
      if (req.has_param("config")) {
        ini = QString::fromStdString(req.get_param_value("config"));
      }

      // NOTE: We need this to avoid creating the file in `./`, which we don't
      // have write access to!
      //
      // See https://git.io/JemyQ.
      QTemporaryFile config(QString("%1/XXXXXX" + tfmt).arg(QDir::tempPath()));
      if (config.open()) {
        QTextStream stream(&config);
        stream << text << Qt::endl;
        stream.flush();
        config.seek(0);

        QString file = config.fileName();
        if (req.has_param("path") && _mode != "Server") {
          QString path = QString::fromStdString(req.get_param_value("path"));
          if (_mode == "Compatibility") {
            // Compatibility
            res.set_content(
                vale->lint({"--output=JSON", "--mode-compat", file}, path),
                "application/json");
          } else {
            // Command Line
            res.set_content(vale->lint({"--output=JSON", file}, path, true),
                            "application/json");
          }
        } else {
          // Server
          res.set_content(vale->lint({"--output=JSON", file}, "", false),
                          "application/json");
        }
      } else {
        res.set_content("{\"error\": \"no file\"}", "application/json");
      }
    } else {
      res.set_content("{\"error\": \"missing 'text' or 'format'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}

/**
 * [POST]
 *
 * /vale?format={.md|.rst|.adoc|.html}&config={<string>}&text={<string>}
 *
 * Lint the given text according to its given format and config (optional)
 * with Vale.
 */
void HTTPServer::onHTML(const httplib::Request &req, httplib::Response &res) {
  setHeaders(res);

  res.status = req.has_param("html") && req.has_param("text") ? 200 : 400;
  try {
    if (res.status == 200) {
      QString text = QString::fromStdString(req.get_param_value("text"));
      QString html = QString::fromStdString(req.get_param_value("html"));

      QString ini = _config;
      if (req.has_param("config")) {
        ini = QString::fromStdString(req.get_param_value("config"));
      }

      QTemporaryFile t1(QString("%1/XXXXXX.html").arg(QDir::tempPath()));
      QTemporaryFile t2(QString("%1/XXXXXX.html").arg(QDir::tempPath()));
      if (t1.open() && t2.open()) {
        QTextStream s1(&t1);
        s1 << text << Qt::endl;
        s1.flush();
        t1.seek(0);

        QTextStream s2(&t2);
        s2 << html << Qt::endl;
        s2.flush();
        t2.seek(0);

        QString f1 = t1.fileName();
        QString f2 = t2.fileName();
        if (req.has_param("path") && _mode != "Server") {
          QString path = QString::fromStdString(req.get_param_value("path"));
          if (_mode == "Compatibility") {
            // Compatibility
            res.set_content(vale->lint({"--output=JSON", "--built=" + f2,
                                        "--mode-compat", f1},
                                       path),
                            "application/json");
          } else {
            // Command Line
            res.set_content(
                vale->lint({"--output=JSON", "--built=" + f2, f1}, path, true),
                "application/json");
          }
        } else {
          // Server
          res.set_content(
              vale->lint({"--output=JSON", "--built=" + f2, f1}, "", false),
              "application/json");
        }
      } else {
        res.set_content("{\"error\": \"no file\"}", "application/json");
      }
    } else {
      res.set_content("{\"error\": \"missing 'text' or 'html'\"}",
                      "application/json");
    }
  } catch (const std::exception &e) {
    res.status = 500;
    res.set_content("{\"error\": \"server error\"}", "application/json");
  }
}
