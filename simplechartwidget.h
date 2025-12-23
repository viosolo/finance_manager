#ifndef SIMPLECHARTWIDGET_H
#define SIMPLECHARTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QColor>
#include <QPen>
#include <QDate>
#include <vector>
#include <algorithm>

class SimpleChartWidget : public QWidget {
    Q_OBJECT
private:
    std::vector<double> expenses, incomes;
    std::vector<QDate> dates;
    QString title;

public:
    SimpleChartWidget(const std::vector<double>& exp,
                     const std::vector<double>& inc,
                     const std::vector<QDate>& dts,
                     const QString& t, QWidget* parent = nullptr)
        : QWidget(parent), expenses(exp), incomes(inc), dates(dts), title(t) {}

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Фон
        painter.fillRect(rect(), Qt::white);

        if (dates.empty()) return;

        // Заголовок
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(10, 25, title);

        // Границы графика
        int margin = 50;
        QRect chartRect(margin, margin, width() - 2*margin, height() - 2*margin);
        painter.drawRect(chartRect);

        // Максимальное значение для масштабирования
        double maxVal = 0;
        for (double val : expenses) maxVal = std::max(maxVal, val);
        for (double val : incomes) maxVal = std::max(maxVal, val);
        if (maxVal == 0) maxVal = 1;

        // Ось Y
        for (int i = 0; i <= 5; ++i) {
            double value = maxVal * i / 5;
            int y = chartRect.bottom() - (chartRect.height() * i / 5);
            painter.drawText(5, y + 5, QString::number(value, 'f', 0));
        }

        // Ось X
        for (size_t i = 0; i < dates.size(); i += std::max<size_t>(1, dates.size() / 5)) {
            int x = chartRect.left() + (chartRect.width() * i / dates.size());
            painter.drawText(x, chartRect.bottom() + 15, dates[i].toString("dd.MM"));
        }

        // График расходов (красный)
        painter.setPen(QPen(QColor(255, 0, 0), 2));
        drawLine(painter, chartRect, expenses, maxVal);

        // График доходов (зеленый)
        painter.setPen(QPen(QColor(0, 255, 0), 2));
        drawLine(painter, chartRect, incomes, maxVal);

        // Легенда
        painter.setPen(Qt::black);
        painter.drawText(chartRect.right() - 100, chartRect.top() - 10, "🔴 Expenses");
        painter.drawText(chartRect.right() - 100, chartRect.top() + 10, "🟢 Incomes");
    }

private:
    void drawLine(QPainter& painter, const QRect& rect,
                  const std::vector<double>& data, double maxVal) {
        if (data.empty()) return;

        QPainterPath path;
        for (size_t i = 0; i < data.size(); ++i) {
            int x = rect.left() + (rect.width() * i / data.size());
            int y = rect.bottom() - (rect.height() * data[i] / maxVal);

            if (i == 0) path.moveTo(x, y);
            else path.lineTo(x, y);
        }
        painter.drawPath(path);
    }
};

#endif // SIMPLECHARTWIDGET_H
