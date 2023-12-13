#include <algorithm>
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

        // Then try to match things based on gear symbol only
        int sum = 0;
        while (!f.atEnd())
        {
                QString line = f.readLine();
                line.chop(1);

                qInfo("%s", qPrintable(line));

                std::vector<int> winningNumbers;
                std::vector<int> myNumbers;
                std::vector<int> interesection;

                QStringList lineParts = line.split("|");
                if (lineParts.size() != 2)
                {
                        qWarning("Invalid line: %s", qPrintable(line));
                        continue;
                }

                QStringList firstHalf = lineParts[0].split(":");
                if (lineParts.size() != 2)
                {
                        qWarning("Invalid line: %s", qPrintable(line));
                        continue;
                }

                QStringList lineWinningNumbers = firstHalf[1].split(" ");
                QStringList lineMyNumbers = lineParts[1].split(" ");

                for (auto i : lineWinningNumbers)
                {
                        if (i <= 0)
                                continue;

                        winningNumbers.push_back(i.toInt());
                }

                std::sort(winningNumbers.begin(), winningNumbers.end());

                for (auto i : lineMyNumbers)
                {
                        if (i <= 0)
                                continue;

                        myNumbers.push_back(i.toInt());
                }
                std::sort(myNumbers.begin(), myNumbers.end());

                std::set_intersection(winningNumbers.begin(), winningNumbers.end(), myNumbers.begin(), myNumbers.end(),
                                      std::back_inserter(interesection));

                int lineSum = 0;

                for (auto i = 0; i < interesection.size(); ++i)
                {
                        Q_UNUSED(i);

                        if (lineSum == 0)
                        {
                                lineSum = 1;
                        }
                        else
                        {
                                lineSum *= 2;
                        }
                }

                sum += lineSum;
        }
        qInfo("Sum: %d", sum);

        return 0;
}