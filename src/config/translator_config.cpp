/******************************************************************************
 *  Copyright (C) 2013 – 2018 by David Baum <david.baum@naraesk.eu>           *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "translator_config.h"
#include "src/languages.h"
#include "src/config_constants.h"

#include <KRunner/AbstractRunner>
#include <KSharedConfig>

#include <QGridLayout>

K_PLUGIN_CLASS(TranslatorConfig)


TranslatorConfigForm::TranslatorConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

TranslatorConfig::TranslatorConfig(QObject *parent, const KPluginMetaData &args) :
        KCModule(parent, args) {
    m_ui = new TranslatorConfigForm(this->widget());
    QGridLayout *layout = new QGridLayout(this->widget());
    layout->addWidget(m_ui, 0, 0);

    config = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))
                 ->group(QStringLiteral("Runners"))
                 .group(QStringLiteral("krunner_translator"));
    config.config()->reparseConfiguration();

    for (LangCode langCode : Languages::getAllCodes()) {
        m_ui->primaryLanguage->addItem(Languages::fullNameFromCode(langCode), langCode);
        m_ui->secondaryLanguage->addItem(Languages::fullNameFromCode(langCode), langCode);
    }

    // TODO: QCheckBox::stateChanged() is deprecated in newer versions of Qt, but Ubuntu is behind.

    // Languages
    connect(m_ui->primaryLanguage, &QComboBox::currentTextChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->secondaryLanguage, &QComboBox::currentTextChanged, this, &TranslatorConfig::markAsChanged);

    // Translate Shell
    connect(m_ui->googleEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->bingEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);

    // Baidu
    connect(m_ui->baiduEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->baiduAPPID, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->baiduApiKey, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);

    // Youdao
    connect(m_ui->youdaoEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->youdaoAPPID, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->youdaoAppSec, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);

    // Warnings
    connect(m_ui->googleEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::updateWarnings);
    connect(m_ui->bingEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::updateWarnings);
    connect(m_ui->baiduEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::updateWarnings);
    connect(m_ui->youdaoEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::updateWarnings);

    updateWarnings();
}

void TranslatorConfig::load() {
    config.config()->reparseConfiguration();

    // Languages
    QString primaryLangCode   = config.readEntry(Config::PrimaryLanguage::key,   Config::PrimaryLanguage::def);
    QString secondaryLangCode = config.readEntry(Config::SecondaryLanguage::key, Config::SecondaryLanguage::def);

    m_ui->primaryLanguage  ->setCurrentText(Languages::fullNameFromCode(primaryLangCode));
    m_ui->secondaryLanguage->setCurrentText(Languages::fullNameFromCode(secondaryLangCode));

    // Translate Shell
    m_ui->googleEnable->setChecked(config.readEntry(Config::GoogleEnabled::key, Config::GoogleEnabled::def));
    m_ui->bingEnable  ->setChecked(config.readEntry(Config::BingEnabled::key,   Config::BingEnabled::def));

    // Baidu
    m_ui->baiduEnable->setChecked(config.readEntry(Config::BaiduEnabled::key, Config::BaiduEnabled::def));
    m_ui->baiduAPPID    ->setText(config.readEntry(Config::BaiduAppId::key,   Config::BaiduAppId::def));
    m_ui->baiduApiKey   ->setText(config.readEntry(Config::BaiduKey::key,     Config::BaiduKey::def));

    // Youdao
    m_ui->youdaoEnable->setChecked(config.readEntry(Config::YoudaoEnabled::key, Config::YoudaoEnabled::def));
    m_ui->youdaoAPPID    ->setText(config.readEntry(Config::YoudaoAppId::key,   Config::YoudaoAppId::def));
    m_ui->youdaoAppSec   ->setText(config.readEntry(Config::YoudaoSecret::key,  Config::YoudaoSecret::def));

    KCModule::load();
}

void TranslatorConfig::defaults() {
    KCModule::defaults();

    // Languages
    m_ui->primaryLanguage->setCurrentText(Languages::fullNameFromCode(Config::PrimaryLanguage::def));
    m_ui->secondaryLanguage->setCurrentText(Languages::fullNameFromCode(Config::SecondaryLanguage::def));

    // Translate Shell
    m_ui->googleEnable->setChecked(Config::GoogleEnabled::def);
    m_ui->bingEnable->setChecked(Config::BingEnabled::def);

    // Baidu
    m_ui->baiduEnable->setChecked(Config::BaiduEnabled::def);
    m_ui->baiduAPPID->setText(Config::BaiduAppId::def);
    m_ui->baiduApiKey->setText(Config::BaiduKey::def);

    // Youdao
    m_ui->youdaoEnable->setChecked(Config::YoudaoEnabled::def);
    m_ui->youdaoAPPID->setText(Config::YoudaoAppId::def);
    m_ui->youdaoAppSec->setText(Config::YoudaoSecret::def);
}

void TranslatorConfig::save() {
    KCModule::save();

    // Languages
    config.writeEntry(Config::PrimaryLanguage::key,   m_ui->primaryLanguage->currentData().value<LangCode>());
    config.writeEntry(Config::SecondaryLanguage::key, m_ui->secondaryLanguage->currentData().value<LangCode>());

    // Translate Shell
    config.writeEntry(Config::GoogleEnabled::key, m_ui->googleEnable->isChecked());
    config.writeEntry(Config::BingEnabled::key,   m_ui->bingEnable->isChecked());

    // Baidu
    config.writeEntry(Config::BaiduEnabled::key, m_ui->baiduEnable->isChecked());
    config.writeEntry(Config::BaiduAppId::key, m_ui->baiduAPPID->text());
    config.writeEntry(Config::BaiduKey::key, m_ui->baiduApiKey->text());

    // Youdao
    config.writeEntry(Config::YoudaoEnabled::key, m_ui->youdaoEnable->isChecked());
    config.writeEntry(Config::YoudaoAppId::key, m_ui->youdaoAPPID->text());
    config.writeEntry(Config::YoudaoSecret::key, m_ui->youdaoAppSec->text());

    config.config()->sync();
}

void TranslatorConfig::updateWarnings() {
    bool googleEnabled = m_ui->googleEnable->isChecked();
    bool bingEnabled = m_ui->bingEnable->isChecked();
    bool baiduEnabled = m_ui->baiduEnable->isChecked();
    bool youdaoEnabled = m_ui->youdaoEnable->isChecked();

    // Show warning if Bing is enabled
    if (bingEnabled) {
        m_ui->bingWarningReliability->show();
    } else {
        m_ui->bingWarningReliability->hide();
    }

    // Show additional warning if Bing is the only engine that's enabled
    if (bingEnabled && !googleEnabled && !baiduEnabled && !youdaoEnabled) {
        m_ui->bingWarningOnlyEngine->show();
    } else {
        m_ui->bingWarningOnlyEngine->hide();
    }

    // Show error/warning if all the engines are disabled
    if (!googleEnabled && !bingEnabled && !baiduEnabled && !youdaoEnabled) {
        m_ui->noEngineWarning->show();
    } else {
        m_ui->noEngineWarning->hide();
    }
}

#include "translator_config.moc"
