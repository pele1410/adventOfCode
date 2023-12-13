#include <iostream>
#include <map>
#include <set>
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
        QRegExp const SYMBOL("(\\*)");

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

        lineNumber_t currentLineNumber = -1;

        std::map<lineNumber_t, potentialPartList_t> potentialParts;
        std::map<lineNumber_t, std::vector<startIndex_t>> symbols;

        while (!f.atEnd())
        {
                QString line = f.readLine();
                line.chop(1);

                qInfo("Line %s", qPrintable(line));

                ++currentLineNumber;

                // Find all symbols first
                int pos = 0;
                while (true)
                {
                        pos = SYMBOL.indexIn(line, pos);
                        if (pos < 0)
                                break;

                        symbols[currentLineNumber].push_back(pos);
                        pos += SYMBOL.matchedLength();
                }

                // Then find all part numbers
                pos = 0;
                while (true)
                {
                        pos = PART_NUMBER.indexIn(line, pos);
                        if (pos < 0)
                                break;

                        potentialPart_t p;
                        // Start index is actually -1 to account for diagnol
                        p.startIndex = pos - 1;
                        // Start index is actually +1 to account for diagnol
                        p.endIndex = p.startIndex + 1 + PART_NUMBER.matchedLength();
                        p.partNumber = PART_NUMBER.cap(1);

                        potentialParts[currentLineNumber].push_back(p);

                        pos += PART_NUMBER.matchedLength();
                }
        }

        auto const totalLines = currentLineNumber;

        // Then try to match things based on gear symbol only
        int sum = 0;
        currentLineNumber = -1;

        auto const isPart = [](potentialPart_t const &p_, startIndex_t symbolPos_) -> int
        {
                if (symbolPos_ >= p_.startIndex && symbolPos_ <= p_.endIndex)
                {
                        return p_.partNumber.toInt();
                }

                return 0;
        };

        auto const checkLine = [&](lineNumber_t lineNumber_, startIndex_t symbolPos_, std::set<int> &maybeParts_)
        {
                auto const parts = potentialParts[lineNumber_];
                for (auto j = 0; j < parts.size(); ++j)
                {
                        int const part = isPart(parts[j], symbolPos_);
                        if (part > 0)
                        {
                                maybeParts_.insert(part);
                        }
                }
        };

        while (currentLineNumber < totalLines)
        {
                ++currentLineNumber;

                if (symbols.count(currentLineNumber) <= 0)
                {
                        continue;
                }

                auto lineSymbols = symbols[currentLineNumber];

                for (int i = 0; i < lineSymbols.size(); ++i)
                {

                        std::set<int> maybeParts;
                        int const symbolPos = lineSymbols[i];

                        // Check against previous line if there is one
                        if (currentLineNumber > 0)
                        {
                                checkLine(currentLineNumber - 1, symbolPos, maybeParts);
                        }

                        // Check against current line
                        {
                                checkLine(currentLineNumber, symbolPos, maybeParts);
                        }

                        // Check against next line if there is one
                        if (currentLineNumber < totalLines)
                        {
                                checkLine(currentLineNumber + 1, symbolPos, maybeParts);
                        }

                        if (maybeParts.size() == 2)
                        {
                                std::string partsString;
                                for (auto i : maybeParts)
                                        partsString += "," + std::to_string(i);

                                qInfo("Parts: %s", partsString.c_str());
                                sum += std::accumulate(maybeParts.begin(), maybeParts.end(), 1, std::multiplies<int>());
                        }
                }
        }
        qInfo("Sum: %d", sum);

        return 0;
}