/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{

}

void Highlighter::set_bash( paths path_bucket ) {

  HighlightingRule rule;
  highlightingRules.clear();
  // expressions
  keywordFormat.setForeground(Qt::red);
  keywordFormat.setFontItalic(true);
  rule.pattern = QRegExp("\\(\\(.+\\)\\)");
  rule.format = keywordFormat;
  highlightingRules.append(rule);


  // bash words
  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setFontWeight(QFont::Bold);
  keywordFormat.setFontItalic(false);
  QStringList keywordPatterns;
  keywordPatterns << "\\bif\\b" << "\\bthen\\b" << "\\bfi\\b"
                  << "\\bfor\\b" << "\\bdo\\b" << "\\bdone\\b"
                  << "\\bunset\\b" << "\\bsed\\b" << "\\bls\\b"
                  << "\\brm\\b" << "\\bmv\\b" << "\\bcp\\b"
                  << "\\bexit\\b" << "\\\\$" << "\\bgrep\\b"
                  << "\\bcd\\b" << "\\bawk\\b"  << "\\becho\\b"
                  << "\\bsleep\\b"<< "\\bcut\\b" << "\\bsort\\b";
  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
    }
  //  executables
  keywordPatterns.clear();
  keywordFormat.setForeground(Qt::darkBlue);
  keywordFormat.setFontWeight(QFont::Bold);
  keywordFormat.setFontItalic(false);
  std::vector <std::string> exes = path_bucket.list_paths();
  for(unsigned int i = 0; i < exes.size(); i++ )
    keywordPatterns.push_back(QString::fromStdString("\\b"+exes.at(i)+"\\b"));

  QString pipepath = QString::fromStdString(path_bucket.get_path_pipeliner());
  rule.pattern = QRegExp(QRegExp::escape(pipepath) + "\\s" + ".+\\s");
  rule.pattern.setMinimal(true);
  rule.format = keywordFormat;
  highlightingRules.append(rule);

  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
    }
  // bash variables
  keywordPatterns.clear();
  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setFontWeight(QFont::Normal);
  keywordFormat.setFontItalic(true);
  keywordPatterns << "\\$[^\\(]\\S+(\\b|/)+"
                  << "\\$\\{.+\\}";
  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      rule.pattern.setMinimal(true);
      highlightingRules.append(rule);
    }
  // comments
  keywordPatterns.clear();
  keywordFormat.setForeground(Qt::darkGreen);
  keywordFormat.setFontWeight(QFont::Normal);
  keywordFormat.setFontItalic(false);

  keywordPatterns << "[^\\\\]\\#.{0,}";
  keywordPatterns << "^#.{0,}";
  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      rule.pattern.setMinimal(false);
      highlightingRules.append(rule);
    }


}

void Highlighter::apply(){
  HighlightingRule rule;

  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);

  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
    }

// add comments and call outs
  // comments
  keywordPatterns.clear();
  keywordFormat.setForeground(Qt::darkGreen);
  keywordFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);

  keywordFormat.setFontWeight(QFont::Normal);
  keywordFormat.setFontItalic(false);
  keywordPatterns << "[^\\\\]?\\#.+";

  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      rule.pattern.setMinimal(false);
      highlightingRules.append(rule);
    }

  // call outs
  keywordPatterns.clear();
  keywordFormat.setForeground(Qt::red);
  keywordFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);

  keywordFormat.setFontWeight(QFont::Normal);
  keywordFormat.setFontItalic(false);
  keywordPatterns << "[^\\\\]?\\#.+\\!";

  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      rule.pattern.setMinimal(false);
      highlightingRules.append(rule);
    }



}


void Highlighter::highlightBlock(const QString &text)
{
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
