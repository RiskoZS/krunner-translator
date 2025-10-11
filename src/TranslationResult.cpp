#include "TranslationResult.h"


TranslationResult::Translation::Translation(TranslationResult *result, QString translated, QVariant audio)
	: m_result(result), m_translated(translated), m_audio(audio) {
}

TranslationResult::Translation::Translation(TranslationResult *result, QString translated)
	: m_result(result), m_translated(translated) {
}

QString TranslationResult::Translation::translated() {
	return m_translated;
}

QVariant TranslationResult::Translation::audio() {
	return m_audio;
}

QVariant TranslationResult::Translation::audio() {
	return m_audio;
}


TranslationResult::TranslationResult(TranslationProvider *provider, LangCode sourceLang, LangCode destLang, QString original) {
	m_provider = provider;
	m_original = original;
	m_sourceLanguage = sourceLang;
	m_targetLanguage = destLang;
}

bool TranslationResult::complete() {
	return m_complete;
}

QString TranslationResult::error() {
	return m_error;
}

TranslationProvider* TranslationResult::provider() {
	return m_provider;
}

LangCode TranslationResult::sourceLanguage() {
	return m_sourceLanguage;
}

LangCode TranslationResult::targetLanguage() {
	return m_targetLanguage;
}

QString TranslationResult::original() {
	return m_original;
}

QVariant TranslationResult::sourceAudio() {
	return m_sourceAudio;
}

QList<TranslationResult::Translation> TranslationResult::translations() {
    return m_translations;
}

void TranslationResult::addTranslation(Translation translation) {
	m_translations.append(translation);
}

void TranslationResult::setSourceAudio(QVariant data) {
	m_sourceAudio = data;
}

void TranslationResult::setError(QString error) {
	m_error = error;
}

void TranslationResult::finalize() {
	m_complete = true;
	Q_EMIT completed();
}
