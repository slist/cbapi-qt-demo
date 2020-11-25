// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QRegExp>

#include "iocsyntaxhighlighter.h"
#include <QRegExp>

IocSyntaxHighlighter::IocSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkGreen);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bprocess_name\\b"),
        QStringLiteral("\\bprocess_hash\\b"),
        QStringLiteral("\\bprocess_product_version\\b"),
        QStringLiteral("\\bprocess_cmdline\\b"),
        QStringLiteral("\\bprocess_publisher\\b"),
        QStringLiteral("\\bprocess_publisher_state\\b"),
        QStringLiteral("\\bprocess_reputation\\b"),
        QStringLiteral("\\bprocess_username\\b"),

        QStringLiteral("\\bchildproc_name\\b"),
        QStringLiteral("\\bchildproc_count\\b"),

        QStringLiteral("\\bmodload_name\\b"),
        QStringLiteral("\\bmodload_count\\b"),
        QStringLiteral("\\bmodload_publisher_state\\b"),

        QStringLiteral("\\bnetconn_domain\\b"),
        QStringLiteral("\\bnetconn_ipv4\\b"),
        QStringLiteral("\\bnetconn_location\\b"),
        QStringLiteral("\\bnetconn_count\\b"),
        QStringLiteral("\\bnetconn_failed\\b"),
        QStringLiteral("\\bnetconn_inbound\\b"),
        QStringLiteral("\\bnetconn_port\\b"),

        QStringLiteral("\\bfilemod_name\\b"),
        QStringLiteral("\\bfilemod_count\\b"),

        QStringLiteral("\\bregmod_name\\b"),
        QStringLiteral("\\bregmod_count\\b"),

        QStringLiteral("\\bdevice_os\\b"),
        QStringLiteral("\\bdevice_group\\b"),
        QStringLiteral("\\bdevice_external_ip\\b"),
        QStringLiteral("\\bdevice_internal_ip\\b"),
        QStringLiteral("\\bdevice_name\\b"),
        QStringLiteral("\\bdevice_timestamp\\b"),

        QStringLiteral("\\bttp\\b"),
        QStringLiteral("\\bparent_name\\b"),
        QStringLiteral("\\bfileless_scriptload_cmdline\\b"),
        QStringLiteral("\\bfileless_scriptload_cmdline_length\\b"),

        QStringLiteral("\\bscriptload_count\\b"),
        QStringLiteral("\\bcrossproc_name\\b"),

        QStringLiteral("\\blegacy\\b"),
        QStringLiteral("\\benriched\\b"),

        QStringLiteral("\\bssensor_action\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules_base.append(rule);
    }

    operatorFormat.setForeground(Qt::darkRed);
    const QString operatorPatterns[] = {
        QStringLiteral("\\bAND\\b"), QStringLiteral("\\bOR\\b"), QStringLiteral("\\bNOT\\b"), QStringLiteral("\\bTO\\b")
    };
    for (const QString &pattern : operatorPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = operatorFormat;
        highlightingRules_base.append(rule);
    }

    valueFormat.setForeground(Qt::darkBlue);
    const QString valuePatterns[] = {
        QStringLiteral("\\bMODIFY_SENSOR\\b"),
        QStringLiteral("\\bFILELESS\\b"),
        QStringLiteral("\\bFILE_SIGNATURE_STATE_NOT_SIGNED\\b"),
        QStringLiteral("\\bFILE_SIGNATURE_STATE_VERIFIED\\b"),
        QStringLiteral("\\bNOT_LISTED\\b"),
        QStringLiteral("\\bUNKNOWN\\b"),
        QStringLiteral("\\bLOW_REPUTATION_SITE\\b"),
        QStringLiteral("\\bSUSPICIOUS_DOMAIN\\b"),
        QStringLiteral("\\bSUSPICIOUS_SITE\\b"),
        QStringLiteral("\\bINJECT_CODE\\b"),
        QStringLiteral("\\bHAS_INJECTED_CODE\\b"),
        QStringLiteral("\\bHAS_MALWARE_CODE\\b"),
        QStringLiteral("\\bCOMPROMISED_PROCESS\\b"),
        QStringLiteral("\\bFIXED_PORT_LISTEN\\b"),
        QStringLiteral("\\bRUN_CMD_SHELL\\b"),
        QStringLiteral("\\bPRIVILEGE_ESCALATE\\b"),
        QStringLiteral("\\bWINDOWS\\b"),
        QStringLiteral("\\bMAC\\b"),
        QStringLiteral("\\bLINUX\\b"),
        QStringLiteral("\\bTERMIATE\\b"),
        QStringLiteral("\\bDENY\\b")
    };
    for (const QString &pattern : valuePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = valueFormat;
        highlightingRules_base.append(rule);
    }

    // TODO Low prio ;-) Better Regexp... https://www.oreilly.com/library/view/regular-expressions-cookbook/9780596802837/ch07s16.html

    IPv4AddressFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");
    rule.format= IPv4AddressFormat;
    highlightingRules_base.append(rule);
    highlightingRules = highlightingRules_base;
}

void IocSyntaxHighlighter::highlightBlock(const QString & text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

void IocSyntaxHighlighter::highlightSearch(const QString & text)
{
    HighlightingRule rule;

    QTextCharFormat highlightSearchFormat;
    highlightSearchFormat.setBackground(Qt::yellow);
    rule.pattern = QRegExp("\\b" + text + "\\b");
    rule.format = highlightSearchFormat;
    highlightingRules = highlightingRules_base;
    highlightingRules.append(rule);
    rehighlight();
}
