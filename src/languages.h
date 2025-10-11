/******************************************************************************
 *  Copyright (C) 2013 – 2020 by David Baum <david.baum@naraesk.eu>           *
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

#pragma once

#include <klocalizedstring.h>
#include <QMap>

// TODO: Implement the `auto` language code.

/**
 * The type for an understood language code.
 */
typedef QString LangCode;

/**
 * Holds the list of understood language codes and the mapping between codes and
 * proper names.
 */
namespace Languages {
    namespace {
        inline const QMap<LangCode, QString>& getMap() {
            // C++11 guarantees static variables will be initialized only once, and
            // this function should only be called after the runner has initialized,
            // so the calls to `i18n` should be safe as well. Read-only accesses to
            // the map are thread-safe as well, per Qt docs.
            #define LANG(code, name) {QStringLiteral(code), i18n(name)}
            static const QMap<LangCode, QString> map = {
                LANG("af", "Afrikaans"),
                LANG("sq", "Albanian"),
                LANG("am", "Amharic"),
                LANG("ar", "Arabic"),
                LANG("hy", "Armenian"),
                LANG("az", "Azerbaijan"),
                LANG("eu", "Basque"),
                LANG("be", "Belarusian"),
                LANG("bn", "Bengali"),
                LANG("bs", "Bosnian"),
                LANG("bg", "Bulgarian"),
                LANG("my", "Burmese"),
                LANG("ca", "Catalan"),
                LANG("ceb", "Cebuano"),
                LANG("ny", "Chewa"),
                LANG("zh", "Chinese"),
                LANG("co", "Corsican"),
                LANG("hr", "Croatian"),
                LANG("cs", "Czech"),
                LANG("da", "Danish"),
                LANG("nl", "Dutch"),
                LANG("en", "English"),
                LANG("eo", "Esperanto"),
                LANG("et", "Estonian"),
                LANG("fil", "Filipino"),
                LANG("fi", "Finish"),
                LANG("fr", "French"),
                LANG("gl", "Galician"),
                LANG("ka", "Georgian"),
                LANG("de", "German"),
                LANG("el", "Greek"),
                LANG("gu", "Gujarati"),
                LANG("ht", "Haitian Creole"),
                LANG("ha", "Hausa"),
                LANG("haw", "Hawaiian"),
                LANG("he", "Hebrew"),
                LANG("hi", "Hindi"),
                LANG("hmn", "Hmong"),
                LANG("hu", "Hungarian"),
                LANG("is", "Icelandic"),
                LANG("ig", "Igbo"),
                LANG("id", "Indonesian"),
                LANG("ga", "Irish"),
                LANG("it", "Italian"),
                LANG("ja", "Japanese"),
                LANG("jv", "Javanese"),
                LANG("kn", "Kannada"),
                LANG("kk", "Kazakh"),
                LANG("km", "Khmer"),
                LANG("rw", "Kinyarwanda"),
                LANG("ko", "Korean"),
                LANG("ku", "Kurdish"),
                LANG("ky", "Kyrgyz"),
                LANG("lo", "Lao"),
                LANG("la", "Latin"),
                LANG("lv", "Latvian"),
                LANG("lt", "Lithuanian"),
                LANG("lb", "Luxembourgish"),
                LANG("mk", "Macedonian"),
                LANG("mg", "Malagasy"),
                LANG("ms", "Malay"),
                LANG("ml", "Malayalam"),
                LANG("mt", "Maltese"),
                LANG("mi", "Māori"),
                LANG("mr", "Marathi"),
                LANG("mn", "Mongolian"),
                LANG("ne", "Nepali"),
                LANG("no", "Norwegian"),
                LANG("or", "Odia"),
                LANG("ps", "Pashto"),
                LANG("fa", "Persian"),
                LANG("pl", "Polish"),
                LANG("pt", "Portuguese"),
                LANG("pa", "Punjabi"),
                LANG("ro", "Romanian"),
                LANG("ru", "Russian"),
                LANG("sm", "Samoan"),
                LANG("gd", "Scots Gaelic"),
                LANG("sr", "Serbian"),
                LANG("sn", "Shona"),
                LANG("sd", "Sindhi"),
                LANG("si", "Sinhala"),
                LANG("sk", "Slovak"),
                LANG("sl", "Slovenian"),
                LANG("so", "Somali"),
                LANG("st", "Sotho"),
                LANG("es", "Spanish"),
                LANG("su", "Sundanese"),
                LANG("sw", "Swahili"),
                LANG("sv", "Swedish"),
                LANG("tl", "Tagalog"),
                LANG("tg", "Tajik"),
                LANG("ta", "Tamil"),
                LANG("tt", "Tatar"),
                LANG("te", "Telugu"),
                LANG("th", "Thai"),
                LANG("tr", "Turkish"),
                LANG("tk", "Turkmen"),
                LANG("uk", "Ukrainian"),
                LANG("ur", "Urdu"),
                LANG("ug", "Uyghur"),
                LANG("uz", "Uzbek"),
                LANG("vi", "Vietnamese"),
                LANG("cy", "Welsh"),
                LANG("fy", "West Frisian"),
                LANG("xh", "Xhosa"),
                LANG("he", "Yiddish"),
                LANG("yo", "Yoruba"),
                LANG("zu", "Zulu"),
            };
            #undef LANG

            return map;
        }
    }

    /**
     * Returns whether the given string is a valid language code.
     */
    inline bool hasCode(QString code) {
        return getMap().contains(code);
    }

    /**
     * Returns a list of all understood languages codes.
     */
    inline const QList<LangCode> getAllCodes() {
        return getMap().keys();
    }

    /**
     * Returns the proper language name for a given code.
     */
    inline QString nameFromCode(LangCode code) {
        return getMap().value(code);
    }

    /**
     * Returns a formatted, localized string containing both the language name
     * and the language code.
     */
    inline QString fullNameFromCode(LangCode code) {
        return i18nc("<language name> (<language ISO code>)", "%1 (%2)", nameFromCode(code), code);
    }
}
