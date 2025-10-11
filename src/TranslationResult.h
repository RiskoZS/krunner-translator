#pragma once

#include "languages.h"
#include <QString>
#include <QVariant>

class TranslationProvider;

class TranslationResult : public QObject {
	Q_OBJECT

public:
	class Translation {
	public:
		Translation(TranslationResult *result, QString translated, QVariant audio);
		Translation(TranslationResult *result, QString translated);

		TranslationResult *result();
		QString translated();
		QVariant audio();

	private:
		TranslationResult *m_result;
		QString m_translated;
		QVariant m_audio;
	};

	TranslationResult(TranslationProvider *provider, LangCode source, LangCode target, QString original);

	bool complete();
	QString error();
	TranslationProvider* provider();

	LangCode sourceLanguage();
	LangCode targetLanguage();
	QString original();
	QVariant sourceAudio();
	QList<Translation> translations();

	void addTranslation(Translation translation);
	void setError(QString error);
	void setSourceAudio(QVariant sourceAudio);
	void finalize();

private:
	bool m_complete = false;
    QString m_error;
    TranslationProvider *m_provider;
	LangCode m_sourceLanguage;
	LangCode m_targetLanguage;
	QString m_original;
	QVariant m_sourceAudio;
	QList<Translation> m_translations;

Q_SIGNALS:
	void completed();
};

Q_DECLARE_METATYPE(TranslationResult)
