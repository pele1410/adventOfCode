#include <iostream>

#include <QFile>
#include <QFileInfo>

namespace
{
enum errors_t
{
        MISSING_ARGS = 0,
        FAILED_TO_OPEN = 1,
};
}

int findFirstNumber (QString line_)
{
        auto const index = line_.indexOf (QRegExp ("\\d"));
        if (index < 0)
                return -1;

        return QString (line_.at (index)).toInt ();
}

int findLastNumber (QString line_)
{
        auto const index = line_.lastIndexOf (QRegExp ("\\d"));
        if (index < 0)
                return -1;

        return QString (line_.at (index)).toInt ();
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