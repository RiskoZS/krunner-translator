#include "BaiduTranslationProvider.h"

#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


static const QString ENDPOINT = QStringLiteral("http://fanyi-provider.baidu.com/provider/trans/vip/translate");

QString mapLanguage(QString lang) {
    if (lang == QStringLiteral("ko")) return QStringLiteral("kor");
    if (lang == QStringLiteral("bg")) return QStringLiteral("bul");
    if (lang == QStringLiteral("fi")) return QStringLiteral("fin");
    if (lang == QStringLiteral("sk")) return QStringLiteral("slo");
    if (lang == QStringLiteral("fr")) return QStringLiteral("fra");
    if (lang == QStringLiteral("ar")) return QStringLiteral("ara");
    if (lang == QStringLiteral("et")) return QStringLiteral("est");
    if (lang == QStringLiteral("sv")) return QStringLiteral("swe");
    if (lang == QStringLiteral("ja")) return QStringLiteral("jp");
    if (lang == QStringLiteral("es")) return QStringLiteral("spa");
    if (lang == QStringLiteral("da")) return QStringLiteral("dan");
    if (lang == QStringLiteral("ro")) return QStringLiteral("rom");
    return lang;
}

BaiduTranslationProvider::BaiduTranslationProvider(QString name)
	: TranslationProvider(name) {
}

TranslationResult* BaiduTranslationProvider::translate(QString text, LangCode source, LangCode dest) {
    quint32 salt = QRandomGenerator::global()->generate();

    QString rawSign = m_appId + text + QString::number(salt) + m_key;
	QString sign = QString::fromLatin1(QCryptographicHash::hash(rawSign.toUtf8(), QCryptographicHash::Md5).toHex());

    // rawSign.append(m_appId);
    // rawSign.append(text);
    // rawSign.append(QString::number(salt));
    // rawSign.append(m_key);
    // QByteArray hash = QCryptographicHash::hash(rawSign.toUtf8(), QCryptographicHash::Md5);
    // QString signMD5 = QString::fromLatin1(hash.toHex());

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("appid"), m_appId);
    query.addQueryItem(QStringLiteral("q"), text);
    query.addQueryItem(QStringLiteral("from"), mapLanguage(source));
    query.addQueryItem(QStringLiteral("to"), mapLanguage(dest));
    query.addQueryItem(QStringLiteral("salt"), QString::number(salt));
    query.addQueryItem(QStringLiteral("sign"), sign);

    QUrl url = QUrl(ENDPOINT);
    url.setQuery(query);

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    // QString urlString = QStringLiteral("http://fanyi-provider.baidu.com/provider/trans/vip/translate?")
    //                     .append(QString::fromUtf8(QUrl(query.query(QUrl::FullyEncoded)).toEncoded()));
    // auto request = QNetworkRequest(QUrl(urlString));
    // //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    // request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    auto result = new TranslationResult(this, source, dest, text);
    auto manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [this, result, manager](QNetworkReply *reply){
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (statusCode != 200) {
            result->setTranslated(i18nc("HTTP Error: <status code>", "HTTP Error: %1", statusCode));
            manager->deleteLater();
            result->finalize(false);
            return;
        }

        QJsonObject body = QJsonDocument::fromJson(reply->readAll()).object();

        QJsonValue errorCode = body.value("error_code");
        if (errorCode != QJsonValue::Undefined) {
            result->setTranslated(i18n("(Baidu) Error Code: %1", errorCode.toString()));
            manager->deleteLater();
            result->finalize(false);
            return;
        }

        // CONTINUE: Figure out how to deal with services (like this one) that can return multiple translations
        QList<KRunner::QueryMatch> matches;
        const QJsonArray results = body.find(QStringLiteral("trans_result")).value().toArray();
        float relevance = 1;
        for (const QJsonValue &result: results) {
            KRunner::QueryMatch match(m_runner);
            match.setIconName(QStringLiteral("applications-education-language"));
            match.setText(result.toObject().find(QStringLiteral("dst")).value().toString());
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        m_context.addMatches(matches);

        Q_EMIT finished();
    });
    manager->get(request);

    return result;


    // auto result = new TranslationResult(this, source, dest, text);
	// auto process = new QProcess();
	// connect(process, &QProcess::finished, result, [this, result, process](){
	// 	result->setTranslated(QString::fromUtf8(process->readAllStandardOutput()));
	// 	process->deleteLater();

	// 	if (m_hasAudio) {
	// 		result->setSourceAudio(true);
	// 		result->setTranslatedAudio(true);
	// 	}
	// 	result->finalize();
	// });

	// QStringList args = {
	// 	QStringLiteral("--brief"),
	// 	QStringLiteral("--engine"), m_engine,
	// 	source + QStringLiteral(":") + dest,
	// 	text,
	// };
	// process->start(QStringLiteral("trans"), args);

	// return result;
}


// Baidu::Baidu(KRunner::AbstractRunner *runner, KRunner::RunnerContext &context, const QString &text,
//              const QPair<QString, QString> &language, const QString &appid, const QString &key)
//         : m_runner(runner), m_context(context) {
//     m_manager = new QNetworkAccessManager(this);

//     QRandomGenerator randomGenerator = QRandomGenerator(QTime(0, 0, 0).secsTo(QTime::currentTime()));

//     quint32 salt = randomGenerator.generate();

//     QString sign;
//     sign.append(appid);
//     sign.append(text);
//     sign.append(QString::number(salt));
//     sign.append(key);
//     QByteArray hash = QCryptographicHash::hash(sign.toUtf8(), QCryptographicHash::Md5);
//     QString signMD5 = QString::fromLatin1(hash.toHex());

//     QUrlQuery query;
//     query.addQueryItem(QStringLiteral("appid"), appid);
//     query.addQueryItem(QStringLiteral("q"), text);
//     query.addQueryItem(QStringLiteral("from"), langMapper(language.first));
//     query.addQueryItem(QStringLiteral("to"), langMapper(language.second));
//     query.addQueryItem(QStringLiteral("salt"), QString::number(salt));
//     query.addQueryItem(QStringLiteral("sign"), signMD5);

//     QString urlString = QStringLiteral("http://fanyi-provider.baidu.com/provider/trans/vip/translate?")
//                         .append(QString::fromUtf8(QUrl(query.query(QUrl::FullyEncoded)).toEncoded()));
//     auto request = QNetworkRequest(QUrl(urlString));
//     //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
//     request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

//     m_manager->get(request);
//     connect(m_manager, &QNetworkAccessManager::finished, this, &Baidu::parseResult);
// }

// void Baidu::parseResult(QNetworkReply *reply) {
//     if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
//         Q_EMIT finished();
//         return;
//     }

//     const QString s = QString::fromUtf8(reply->readAll());
//     const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
//     if (jsonObject.contains(QStringLiteral("error_code"))) {
//         KRunner::QueryMatch match(m_runner);
//         match.setIconName(QStringLiteral("dialog-error"));
//         match.setText(i18n("(Baidu) Error code: %1", jsonObject.find(QStringLiteral("error_code")).value().toString()));
//         match.setRelevance(1);
//         m_context.addMatch(match);
//     } else {
//         QList<KRunner::QueryMatch> matches;
//         const QJsonArray results = jsonObject.find(QStringLiteral("trans_result")).value().toArray();
//         float relevance = 1;
//         for (const QJsonValue &result: results) {
//             KRunner::QueryMatch match(m_runner);
//             match.setIconName(QStringLiteral("applications-education-language"));
//             match.setText(result.toObject().find(QStringLiteral("dst")).value().toString());
//             match.setRelevance(relevance);
//             matches.append(match);
//             relevance -= 0.01;
//         }
//         m_context.addMatches(matches);
//     }
//     Q_EMIT finished();
// }

void BaiduTranslationProvider::setAppId(QString appId) {
	m_appId = appId;
}

void BaiduTranslationProvider::setKey(QString key) {
	m_key = key;
}

void BaiduTranslationProvider::playSourceAudio(TranslationResult *result) {

}

void BaiduTranslationProvider::playTranslatedAudio(TranslationResult *result) {

}
