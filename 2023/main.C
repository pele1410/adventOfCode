#include <iostream>
#include <map>
#include <vector>

#include <QFile>
#include <QFileInfo>
#include <QString>

namespace
{
        enum errors_t
        {
                MISSING_ARGS = 0,
                FAILED_TO_OPEN = 1,
        };

        using lineNumber_t = int;
        using startIndex_t = int;

        struct potentialPart_t
        {
                startIndex_t startIndex;
                startIndex_t endIndex;
                QString partNumber;
        };

        using potentialPartList_t = std::vector<potentialPart_t>;

        QRegExp const PART_NUMBER("(\\d+)");
        QRegExp const SYMBOL("([^\\.\\d])");

}

int main(int argc_, char **argv_)
{
        if (argc_ < 2)
        {
                qCritical("Missing input file argument");
                return errors_t::MISSING_ARGS;
        }

        QFileInfo fi(argv_[1]);
        QFile f(fi.absoluteFilePath());

        if (!f.open(QIODevice::ReadOnly))
        {
                qWarning("Failed to open file '%s'", qPrintable(fi.absoluteFilePath()));
                return errors_t::FAILED_TO_OPEN;
        }

        int sum = 0;
        lineNumber_t currentLineNumber = -1;

        std::map<lineNumber_t, potentialPartList_t> potentialParts;
        std::map<lineNumber_t, std::vector<startIndex_t>> symbols;

        while (!f.atEnd())
        {
                QString line = f.readLine();
                line.chop(1);

                qInfo("Line %s", qPrintable(line));

                ++currentLineNumber;

                auto symbolLine = line;
                auto partLine = line;

                auto const isPart = [&sum](potentialPart_t const &p_, startIndex_t symbol_) -> bool
                {
                        qDebug("Comparing symbol_ %d to start/end %d/%d", symbol_, p_.startIndex, p_.endIndex);
                        if (symbol_ >= p_.startIndex && symbol_ <= p_.endIndex)
                        {
                                qInfo("Found a part %s", (qPrintable(p_.partNumber)));
                                sum += p_.partNumber.toInt();
                                return true;
                        }

                        return false;
                };

                auto const matchPartAgainstSymbols = [&isPart](potentialPart_t const &p_, std::vector<startIndex_t> const &symbols_) -> bool
                {
                        for (int i = 0; i < symbols_.size(); ++i)
                        {
                                if (isPart(p_, symbols_[i]))
                                {
                                        return true;
                                }
                        }

                        return false;
                };

                auto const matchSymbolAgainstParts = [&isPart](potentialPartList_t &parts_, startIndex_t symbol_)
                {
                        auto iter = parts_.begin();
                        while (iter != parts_.end())
                        {
                                if (isPart(*iter, symbol_))
                                {
                                        iter = parts_.erase(iter);
                                }
                                else
                                {
                                        ++iter;
                                }
                        }
                };

                // Find all symbols first
                startIndex_t lineOffset = 0;
                while (true)
                {
                        int const pos = SYMBOL.indexIn(symbolLine);
                        if (pos < 0)
                                break;

                        auto const actualPos = pos + lineOffset;

                        symbols[currentLineNumber].push_back(actualPos);
                        symbolLine.remove(pos, SYMBOL.capturedTexts().at(0).length());

                        lineOffset += SYMBOL.capturedTexts().at(0).length();

                        // Check part against previous line
                        if (currentLineNumber > 0)
                        {
                                matchSymbolAgainstParts(potentialParts[currentLineNumber - 1], actualPos);
                        }

                        // Check part against current line
                        matchSymbolAgainstParts(potentialParts[currentLineNumber], actualPos);
                }

                // Then find all part numbers
                lineOffset = 0;
                while (true)
                {
                        int const pos = PART_NUMBER.indexIn(partLine);
                        if (pos < 0)
                                break;

                        partLine.remove(pos, PART_NUMBER.capturedTexts().at(0).length());

                        potentialPart_t p;
                        // Start index is actually -1 to account for diagnol
                        p.startIndex = pos - 1 + lineOffset;
                        // Start index is actually +1 to account for diagnol
                        p.endIndex = p.startIndex + 1 + PART_NUMBER.capturedTexts().at(0).length();
                        p.partNumber = PART_NUMBER.cap(1);

                        lineOffset += PART_NUMBER.capturedTexts().at(0).length();

                        bool found = false;
                        // Check part against previous line
                        if (currentLineNumber > 0 && matchPartAgainstSymbols(p, symbols[currentLineNumber - 1]))
                        {
                                found = true;
                        }

                        // Check part against current line
                        if (matchPartAgainstSymbols(p, symbols[currentLineNumber]))
                        {
                                found = true;
                        }

                        if (!found)
                        {
                                potentialParts[currentLineNumber].push_back(p);
                        }
                }
        }
        qInfo("Sum: %d", sum);

        return 0;
}