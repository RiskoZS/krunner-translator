#include "TranslationProvider.h"

TranslationProvider::TranslationProvider(QString name) {
	m_name = name;
}

QString TranslationProvider::name() {
	return m_name;
}

bool TranslationProvider::enabled() {
	return m_enabled;
}

void TranslationProvider::setEnabled(bool isEnabled) {
	m_enabled = isEnabled;
}
