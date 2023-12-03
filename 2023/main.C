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

constexpr int MAX_RED = 12;
constexpr int MAX_GREEN = 13;
constexpr int MAX_BLUE = 14;

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

                QRegExp gameNumber ("Game (\\d+):");
                QRegExp castNumber ("(\\d+) (\\D+)");

                if (!line.contains (gameNumber))
                {
                        qWarning ("Line does not contain game:\n\t'%s'", qPrintable (line));
                        continue;
                }

                line.remove (0, gameNumber.capturedTexts ().at (0).length ());
                bool badLine = false;
                while (true)
                {
                        qInfo ("Testing line %s", qPrintable (line));
                        int const pos = castNumber.indexIn (line);
                        if (pos < 0)
                                break;

                        auto const value = castNumber.cap (1).toInt ();
                        auto const color = castNumber.cap (2);

                        if (color.contains ("red") && value > MAX_RED)
                        {
                                badLine = true;
                                break;
                        }
                        else if (color.contains ("blue") && value > MAX_BLUE)
                        {
                                badLine = true;
                                break;
                        }
                        else if (color.contains ("green") && value > MAX_GREEN)
                        {
                                badLine = true;
                                break;
                        }

                        line.remove (0, castNumber.capturedTexts ().at (0).length ());
                }

                if (!badLine)
                        sum += gameNumber.cap (1).toInt ();
        }

        qInfo ("Sum: %d", sum);

        return 0;
}