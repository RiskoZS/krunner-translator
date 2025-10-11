# Translator for KDE KRunner (Plasma 6)

This is a fork of https://github.com/naraesk/krunner-translator which attempts to bring krunner-translator to Plasma 6, as well as to refine its functionality. See the [just-plasma-6](https://github.com/RiskoZoSlovenska/krunner-translator-ng/tree/just-plasma-6) branch for the minimal set of changes which bring the original plugin to Plasma 6.

This KRunner translates queries using various external services. Currently, the following are supported:
* [Google Translate](https://translate.google.com/) (supports playing translated audio)
* [Bing Translator](https://www.bing.com/translator)
* [youdao](http://fanyi.youdao.com/)
* [Baidu Fanyi](https://fanyi.baidu.com/)


## Current TODO list (PRs may be attempted upstream if possible)
- [ ] Clean up and re-factor existing code
- [ ] Set up CI to automatically build artifacts/packages
- [ ] Allow auto-detecting languages (may change the runner syntax slightly)
- [ ] Add more providers (e.g. LibreTranslate, DeepL, etc.)
- [ ] Other misc. features


## Usage

Languages are identified using their 2-letter ISO 649-1 codes, a full list of which can be found [on Wikipedia](https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes).

### Specify source and target language

Syntax: `<sourcelang>-<targetlang> <searchterm>` \
*en-de house* → will translate *house* into german (*de*)

### Use default source language

Syntax: `<targetlang> <searchterm>`

Default source language: *English (en)* \
Alternative source language: *Spanish (es)*

*de soccer* → *en-de soccer* \
*en hola* → *es-en hola* \
*fr house* → *en-fr house*


## Configuration

To be able to use Youdao and Baidu, an API key is required. You have to obtain a key yourself by following these steps:

### Youdao
1. Register at https://ai.youdao.com/
2. Get an API key and an API secret from Application Manager
3. Copy the key to configuration dialog

### Baidu
1. Register at http://api.fanyi.baidu.com/
2. Get an API key and an API secret from Consoles
3. Copy the key to configuration dialog


## Building from source

1. Clone this repo, `cd` into it, and run `./install.sh`
2. To uninstall, run `./uninstall.sh`

OR, if you prefer to build manually,

1. Install the distro-specific dependencies
2. Clone this repo, `cd` into it, and run
```sh
cmake -B build
cmake --build build --parallel --config Release
sudo cmake --install build --config Release
systemctl --user restart plasma-krunner  # or killall krunner
```
3. To uninstall, run
```sh
sudo cmake --build build --target uninstall
systemctl --user restart plasma-krunner  # or killall krunner
```

<details>
<summary><strong>Ubuntu Dependencies</strong></summary>

Note: Requires Ubuntu 24.10 (Oracular) or newer.

```sh
sudo apt install \
	translate-shell \
	build-essential \
	cmake \
	extra-cmake-modules \
	qt6-base-dev \
	libkf6runner-dev \
	libkf6i18n-dev \
	libkf6coreaddons-dev \
	libkf6config-dev \
	libkf6configwidgets-dev \
	libkf6kcmutils-dev
```

</details>

<details>
<summary><strong>Fedora Dependencies</strong></summary>

```sh
sudo dnf install \
	translate-shell \
	make \
	cmake \
	extra-cmake-modules \
	qt6-qtbase-devel \
	kf6-krunner-devel \
	kf6-ki18n-devel \
	kf6-kcoreaddons-devel \
	kf6-kconfig-devel \
	kf6-kconfigwidgets-devel \
	kf6-kcmutils-devel
```

</details>


Thanks for your feedback and kudos! If you like the runner, please go say thanks to [the upstream](https://github.com/naraesk/krunner-translator).
