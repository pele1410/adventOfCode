#include <iostream>
#include <unordered_map>

#include <QFile>
#include <QFileInfo>

namespace
{
enum errors_t
{
        MISSING_ARGS = 0,
        FAILED_TO_OPEN = 1,
};

std::vector<std::pair<QString, int>> const STRING_NUMBERS = {{"one", 1},
                                                             {"two", 2},
                                                             {"three", 3},
                                                             {"four", 4},
                                                             {"five", 5},
                                                             {"six", 6},
                                                             {"seven", 7},
                                                             {"eight", 8},
                                                             {"nine", 9}};

}

int findFirstNumber (QString line_)
{
        int smallestIndex = std::numeric_limits<int>::max ();
        int value;

        auto index = line_.indexOf (QRegExp ("\\d"));
        if (index >= 0 && index < smallestIndex)
        {
                smallestIndex = index;
                value = QString (line_.at (index)).toInt ();
        }

        for (auto const &p : STRING_NUMBERS)
        {
                index = line_.indexOf (p.first);
                if (index >= 0 && index < smallestIndex)
                {
                        smallestIndex = index;
                        value = p.second;
                }
        }

        return value;
}

int findLastNumber (QString line_)
{
        int largestIndex = -std::numeric_limits<int>::max ();
        int value;

        auto index = line_.lastIndexOf (QRegExp ("\\d"));
        if (index >= 0 && index > largestIndex)
        {
                largestIndex = index;
                value = QString (line_.at (index)).toInt ();
        }

        for (auto const &p : STRING_NUMBERS)
        {
                index = line_.lastIndexOf (p.first);
                if (index >= 0 && index > largestIndex)
                {
                        largestIndex = index;
                        value = p.second;
                }
        }

        return value;
}

int main (int argc_, char **argv_)
{
        if (argc_ < 2)
        {
                qCritical ("Missing input file argument");
                return errors_t::MISSING_ARGS;
        }

        QFileInfo fi (argv_[1]);
        QFile f (fi.absoluteFilePath ());

        if (!f.open (QIODevice::ReadOnly))
        {
                qWarning ("Failed to open file '%s'", qPrintable (fi.absoluteFilePath ()));
                return errors_t::FAILED_TO_OPEN;
        }

        int sum = 0;

        while (!f.atEnd ())
        {
                QString line = f.readLine ();
                line.chop (1);

                auto firstNum = findFirstNumber (line);
                auto lastNum = findLastNumber (line);

                if (firstNum < 0 || lastNum < 0)
                {
                        qWarning ("Line does not contain numbers:\n\t'%s'", qPrintable (line));
                        continue;
                }

                auto lineNumber = QString ("%1%2").arg (firstNum).arg (lastNum).toInt ();

                qInfo ("Value of line '%s': %d", qPrintable (line), lineNumber);
                sum += lineNumber;
        }

        qInfo ("Total Sum: %d", sum);

        return 0;
}