#!/bin/sh
cp toMOTko.desktop $QPEDIR/apps/Applications/.
cp pics/*.png $QPEDIR/pics/.
if [ ! -d "$QPEDIR/lib/toMOTko" ]; then
    mkdir -p "$QPEDIR/lib/toMOTko"
fi
cp lib/toMOTko/*.conf $QPEDIR/lib/toMOTko/.
cp toMOTko $QPEDIR/bin/.
cp -r help $QPEDIR/.
if [ ! -d "$QPEDIR/help/ja" ]; then
    mkdir -p "$QPEDIR/help/ja"
fi
cp -r help/en/* $QPEDIR/help/ja/.
if [ ! -d "$QPEDIR/i18n" ]; then
    mkdir -p "$QPEDIR/i18n"
fi
if [ ! -d "$QPEDIR/i18n/en" ]; then
    mkdir -p "$QPEDIR/i18n/en"
fi
cp i18n/en/toMOTko.qm $QPEDIR/i18n/en
if [ ! -d "$QPEDIR/i18n/fr" ]; then
    mkdir -p "$QPEDIR/i18n/fr"
fi
cp i18n/fr/toMOTko.qm $QPEDIR/i18n/fr
if [ ! -d "$QPEDIR/i18n/ja" ]; then
    mkdir -p "$QPEDIR/i18n/ja"
fi
cp i18n/ja/toMOTko.qm $QPEDIR/i18n/ja
if [ ! -d "$QPEDIR/i18n/es" ]; then
    mkdir -p "$QPEDIR/i18n/es"
fi
cp i18n/es/toMOTko.qm $QPEDIR/i18n/es
if [ ! -d "$QPEDIR/i18n/zh" ]; then
    mkdir -p "$QPEDIR/i18n/zh"
fi
cp i18n/zh/toMOTko.qm $QPEDIR/i18n/zh
