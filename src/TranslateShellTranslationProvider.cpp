#include "TranslateShellTranslationProvider.h"
#include <QProcess>

TranslateShellTranslationProvider::TranslateShellTranslationProvider(QString name, QString engine, bool hasAudio)
	: TranslationProvider(name), m_engine(engine), m_hasAudio(hasAudio) {
}

TranslationResult* TranslateShellTranslationProvider::translate(QString text, LangCode source, LangCode dest) {
	auto result = new TranslationResult(this, source, dest, text);
	auto process = new QProcess();
	connect(process, &QProcess::finished, result, [this, result, process](){
		result->addTranslation({ result, QString::fromUtf8(process->readAllStandardOutput()), m_hasAudio });
		process->deleteLater();

		if (m_hasAudio) {
			result->setSourceAudio(true);
		}
		result->finalize();
	});

	QStringList args = {
		QStringLiteral("--brief"),
		QStringLiteral("--engine"), m_engine,
		source + QStringLiteral(":") + dest,
		text,
	};
	process->start(QStringLiteral("trans"), args);

	return result;
}

void TranslateShellTranslationProvider::playSourceAudio(TranslationResult *result) {

}

void TranslateShellTranslationProvider::playTranslatedAudio(TranslationResult *result) {

}
