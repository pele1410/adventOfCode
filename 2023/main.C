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

QString const RED_STRING ("red");
QString const GREEN_STRING ("green");
QString const BLUE_STRING ("blue");

QRegExp const GAME_NUMBER ("Game (\\d+):");
QRegExp const CAST_NUMBER ("(\\d+) (\\D+)");

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

                if (!line.contains (GAME_NUMBER))
                {
                        qWarning ("Line does not contain game:\n\t'%s'", qPrintable (line));
                        continue;
                }

                line.remove (0, GAME_NUMBER.capturedTexts ().at (0).length ());

                std::unordered_map<QString, int> maxes = {
                        {RED_STRING, -std::numeric_limits<int>::max ()},
                        {GREEN_STRING, -std::numeric_limits<int>::max ()},
                        {BLUE_STRING, -std::numeric_limits<int>::max ()},
                };

                auto const updateMax = [&maxes] (int value_, QString color_) {
                        color_.remove (QRegExp ("\\W"));
                        maxes[color_] = value_ > maxes[color_] ? value_ : maxes[color_];
                };

                while (true)
                {
                        int const pos = CAST_NUMBER.indexIn (line);
                        if (pos < 0)
                                break;

                        updateMax (CAST_NUMBER.cap (1).toInt (), CAST_NUMBER.cap (2));

                        line.remove (0, CAST_NUMBER.capturedTexts ().at (0).length ());
                }

                sum += maxes[RED_STRING] * maxes[GREEN_STRING] * maxes[BLUE_STRING];
        }

        qInfo ("Sum: %d", sum);

        return 0;
}