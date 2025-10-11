#pragma once

namespace Config {
    #define CONFIG_ENTRY(name, k, v)                      \
        namespace name {                                  \
            static const QString key = QStringLiteral(k); \
            static const auto def = (v);                  \
        }

    CONFIG_ENTRY(PrimaryLanguage,   "primaryLanguage",   QStringLiteral("en"));
    CONFIG_ENTRY(SecondaryLanguage, "secondaryLanguage", QStringLiteral("es"));

    CONFIG_ENTRY(GoogleEnabled, "googleEnable", true);
    CONFIG_ENTRY(BingEnabled,   "bingEnable",  false);

    CONFIG_ENTRY(BaiduEnabled, "baiduEnable", false);
    CONFIG_ENTRY(BaiduAppId,   "baiduAPPID",  QStringLiteral(""));
    CONFIG_ENTRY(BaiduKey,     "baiduAPIKey", QStringLiteral(""));

    CONFIG_ENTRY(YoudaoEnabled, "youdaoEnable", false);
    CONFIG_ENTRY(YoudaoAppId,   "youdaoAPPID",  QStringLiteral(""));
    CONFIG_ENTRY(YoudaoSecret,  "youdaoAPPSec", QStringLiteral(""));

    #undef CONFIG_ENTRY
}
