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

#include "translator.h"

#include "TranslationResult.h"
#include "languages.h"
#include "config_constants.h"

#include <KConfigGroup>
#include <klocalizedstring.h>

#include <QVariant>
#include <QClipboard>
#include <QApplication>


Translator::Translator(QObject *parent, const KPluginMetaData &pluginMetaData)
        : KRunner::AbstractRunner(parent, pluginMetaData) {

    m_googleProvider = new TranslateShellTranslationProvider(QStringLiteral("Google"), QStringLiteral("google"), true);
    m_bingProvider = new TranslateShellTranslationProvider(QStringLiteral("Bing"), QStringLiteral("bing"), false);

    m_providers.append(m_googleProvider);
    m_providers.append(m_bingProvider);

    addSyntax(i18n("<language code> :q:"), i18n("Translates the word(s) :q: into target language"));
    addSyntax(i18n("<source language>-<target language> :q:"),
              i18n("Translates the word(s) :q: from the source into target language"));

    setMatchRegex(QRegularExpression(QStringLiteral("^(\\w{2,3})(?:\\-(\\w{2,3}))?\\s+?(.+)$")));
}

void Translator::init() {
    reloadConfiguration();
}

void Translator::match(KRunner::RunnerContext &context) {
    // Parse the query
    auto match = matchRegex().match(context.query());
    if (!match.hasMatch()) {
        return;
    }

    QString first_lang = match.captured(1);
    QString second_lang = match.captured(2);
    QString text = match.captured(3);

    // Determine languages
    QString source_lang, target_lang;
    if (!second_lang.isEmpty()) {
        source_lang = match.captured(1);
        target_lang = match.captured(2);
    } else {
        source_lang = (first_lang != m_primaryLanguage) ? m_primaryLanguage : m_secondaryLanguage;
        target_lang = first_lang;
    }

    if (text.isEmpty()
            || !Languages::hasCode(source_lang)
            || !Languages::hasCode(target_lang)
            || !context.isValid()) {
        return;
    }

    // Query providers
    QEventLoop loop;
    int num_pending = 0;

    for (auto provider : m_providers) {
        if (!provider->enabled()) {
            continue;
        }

        TranslationResult *result = provider->translate(text, source_lang, target_lang);
        if (result->complete()) {
            addResult(context, result);
            result->deleteLater(); // TODO: Probably just pass results by value
            continue;
        }

        num_pending += 1;
        connect(result, &TranslationResult::completed, this, [this, result, &context, &num_pending, &loop](){
            addResult(context, result);
            result->deleteLater();

            num_pending -= 1;
            if (num_pending <= 0 && loop.isRunning()) {
                loop.quit();
            }
        });
    }

    if (num_pending > 0) {
        loop.exec(); // Yields until `loop.quit()` is called, effectively waiting for all the results to complete
    }
}

void Translator::addResult(KRunner::RunnerContext &context, TranslationResult *result) {
    if (!context.isValid()) {
        return;
    }

    for (auto &translation : result->translations()) {
        addTranslation(context, translation);
    }
}

void Translator::addTranslation(KRunner::RunnerContext &context, TranslationResult::Translation &translation) {
    if (!context.isValid()) {
        return;
    }

    KRunner::QueryMatch match(this);
    // match.setData(QVariant::fromValue<TranslationResult>(*result)); // TODO: Get this working. Actually, maybe make TranslationResult a subclass of QueryMatch?
    match.setText(translation.translated());
    match.setSubtext(translation.result()->provider()->name());
    match.setRelevance(1);
    match.setIconName(QStringLiteral("applications-education-language"));
    if (!translation.audio().isNull()) {
        match.addAction(KRunner::Action(
            QStringLiteral("play"),
            QStringLiteral("audio-symbolic"),
            i18n("Play audio")
        ));
    }
    match.addAction(KRunner::Action(
        QStringLiteral("copy"),
        QStringLiteral("edit-copy-symbolic"),
        i18n("Copy to Clipboard")
    ));
    context.addMatch(match);
}

void Translator::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) {
    Q_UNUSED(context);

    // TODO Actually handle playing audio
    QApplication::clipboard()->setText(match.text());

    KRunner::Action action = match.selectedAction();
    qWarning(action ? "run ran!" : "no action");

    // if (action != nullptr && action.id() == QStringLiteral("play")) {
    //     // TranslateShellProcess process;
    //     // process.play(match.text());
    // }
}

void Translator::reloadConfiguration() {
    KConfigGroup group = config();

    m_primaryLanguage = group.readEntry(Config::PrimaryLanguage::key, Config::PrimaryLanguage::def);
    m_secondaryLanguage = group.readEntry(Config::SecondaryLanguage::key, Config::SecondaryLanguage::def);

    m_googleProvider->setEnabled(group.readEntry(Config::GoogleEnabled::key, Config::GoogleEnabled::def));
    m_bingProvider->setEnabled(group.readEntry(Config::BingEnabled::key, Config::BingEnabled::def));
}

K_PLUGIN_CLASS_WITH_JSON(Translator, "translator.json")

#include "translator.moc"
