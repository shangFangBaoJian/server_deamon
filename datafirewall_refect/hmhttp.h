#ifndef HMHTTP_H
#define HMHTTP_H

#include <QObject>


class HMHttp : public QObject
{
	Q_OBJECT
public:
	explicit HMHttp(QObject *parent = 0);

signals:

public slots:

public:
	QByteArray get(const QString &url, const QVariantMap &form);

	QByteArray post(const QString &url, const QVariantMap &form, const QString &textCodecName = "UTF-8");
	QByteArray post(const QString &url, const QVariantMap &form, const QVariantMap &headers, const QString &textCodecName = "UTF-8");

};

#endif // HMHTTP_H
