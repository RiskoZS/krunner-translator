#pragma once

#include "TranslationResult.h"
#include "languages.h"

#include <KConfigGroup>
#include <QObject>

class TranslationProvider : public QObject
{
	Q_OBJECT

public:
	TranslationProvider(QString name);

	QString name();
	bool enabled();

	void setEnabled(bool isEnabled);

	virtual TranslationResult* translate(QString text, LangCode source, LangCode dest) = 0;

	virtual void playSourceAudio(TranslationResult *result) = 0;
	virtual void playTranslatedAudio(TranslationResult *result) = 0;

private:
	QString m_name;
	bool m_enabled = false;
};
