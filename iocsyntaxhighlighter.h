// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IOCSYNTAXHIGHLIGHTER_H
#define IOCSYNTAXHIGHLIGHTER_H

#include <QWidget>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegExp>

class IocSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    IocSyntaxHighlighter(QTextDocument *parent = 0);
    void highlightSearch(const QString & text);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules_base;
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat IPv4AddressFormat;
    QTextCharFormat highlightSearchFormat;
};

#endif // IOCSYNTAXHIGHLIGHTER_H
