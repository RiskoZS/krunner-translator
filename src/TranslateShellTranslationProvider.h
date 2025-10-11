#pragma once

#include "TranslationProvider.h"
#include <QString>

class TranslateShellTranslationProvider : public TranslationProvider
{
public:
	TranslateShellTranslationProvider(QString name, QString engine, bool hasAudio);

	TranslationResult* translate(QString text, LangCode source, LangCode dest) override;

	void playSourceAudio(TranslationResult *result) override;
    void playTranslatedAudio(TranslationResult *result) override;

private:
	QString m_engine;
	bool m_hasAudio;
};
