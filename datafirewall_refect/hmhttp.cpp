#include "hmhttp.h"
#include "hmutils.h"
#include <QtNetwork>


HMHttp::HMHttp(QObject *parent) : QObject(parent)
{


}


QString getCharsetFromReply(const QNetworkReply *reply)
{
	QString charset;
	if (reply == NULL)
		return charset;

	QString contentTypeFromReply = reply->header(QNetworkRequest::ContentTypeHeader).toString();

	if (contentTypeFromReply.isEmpty())
		return charset;

	QStringList typeList = contentTypeFromReply.split(';', QString::SkipEmptyParts, Qt::CaseInsensitive);
	int idx = -1;

	for (QStringList::const_iterator it_type = typeList.begin(); it_type != typeList.end(); ++it_type)
	{
		idx = it_type->indexOf("charset", 0, Qt::CaseInsensitive);
		if (idx >= 0)
		{
			idx = it_type->indexOf("=", idx + 7);
			if (idx >= 0)
			{
				charset = it_type->right(it_type->length() - idx - 1).trimmed();

				break;
			}
		}
	}

	return charset;
}

void filterCertList(QNetworkRequest &networkRequest)
{
	//================[Begin]:filterCertList================//
	QSslConfiguration sslcfg = networkRequest.sslConfiguration();

	sslcfg.setPeerVerifyMode(QSslSocket::VerifyNone);

	QList<QSslCertificate> cacertlist = sslcfg.caCertificates();
	QDateTime curdatetime = QDateTime::currentDateTime();

	for (QList<QSslCertificate>::iterator it_cacert = cacertlist.begin();it_cacert != cacertlist.end();)
	{
		if ((it_cacert->effectiveDate() > curdatetime) || (it_cacert->expiryDate() < curdatetime))
		{
			it_cacert = cacertlist.erase(it_cacert);
		}
		else
		{
			++it_cacert;
		}
	}
	sslcfg.setCaCertificates(cacertlist);

//	if (!m_cert.isNull())
//		sslcfg.setLocalCertificate(m_cert);
//	if (!m_key.isNull())
//		sslcfg.setPrivateKey(m_key);

	networkRequest.setSslConfiguration(sslcfg);
	//================[End]:filterCertList================//
}

QByteArray HMHttp::get(const QString &url, const QVariantMap &form)
{
	QNetworkAccessManager nam;
	QStringList keys;
	QNetworkRequest request;
	QString fullUrl;

	HMUtils::log("http") << "get " << url << HMLog::endl;

	keys = form.keys();
	foreach(QString key, keys)
	{
		if (!fullUrl.isEmpty())
			fullUrl.append('&');
		fullUrl.append(key.toLatin1());
		fullUrl.append('=');
		fullUrl.append(form.value(key).toString().toLatin1());
	}

	fullUrl = url + "?" + fullUrl;

	HMUtils::log("http") << "get req=" << QString(fullUrl) << HMLog::endl;

	request.setUrl(fullUrl);

//	filterCertList(request);

	QNetworkReply *reply = nam.get(request);

	if (reply == NULL)
	{
		HMUtils::log("http") << "get reply is NULL" << HMLog::endl;
		return QByteArray();
	}

	QTime t;
	t.start();
	while (!reply->isFinished())
	{
		if (t.elapsed() > 9000)
		{
			HMUtils::log("http") << "get timeout" << HMLog::endl;
			reply->deleteLater();
			return QByteArray();
		}

		if (reply->error() != QNetworkReply::NoError)
		{
			reply->deleteLater();
			HMUtils::log("http") << "get response error, " << reply->errorString() << HMLog::endl;
			return QByteArray();
		}

		QCoreApplication::processEvents();
	}

	QByteArray bytes = reply->readAll();

	HMUtils::log("http") << "get " << bytes.size() << " bytes received." << HMLog::endl;

	reply->deleteLater();

	return bytes;
}



QByteArray HMHttp::post(const QString &url, const QVariantMap &form, const QString &textCodecName)
{
	QVariantMap headers;

	return post(url, form, headers, textCodecName);

}

QByteArray HMHttp::post(const QString &url, const QVariantMap &form, const QVariantMap &headers, const QString &textCodecName)
{
	QNetworkAccessManager nam;
	QNetworkRequest request;
	QStringList keys;
	QString data;

	HMUtils::log("http") << "post, " << url << HMLog::endl;

	request.setUrl(url);
	request.setRawHeader("Content-type", ("application/x-www-form-urlencoded;charset=" + textCodecName).toLatin1());
	request.setRawHeader("Accept", "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");


	keys = headers.keys();
	foreach(QString key, keys)
	{
		request.setRawHeader(key.toLatin1(), headers.value(key).toString().toLatin1());
	}


	filterCertList(request);

	keys = form.keys();
	foreach(QString key, keys)
	{
		if (!data.isEmpty())
			data.append('&');
		data.append(key);
		data.append('=');
		data.append(form.value(key).toString());
	}

	HMUtils::log("http") << "post, content is: " << data << HMLog::endl;

	QNetworkReply *reply = nam.post(request, QTextCodec::codecForName(textCodecName.toLatin1())->fromUnicode(data).toPercentEncoding("&="));

	if (reply == NULL)
	{
		HMUtils::log("http") << "post, reply is NULL" << HMLog::endl;
		return QByteArray();
	}

	QTime t;
	t.start();
	while (!reply->isFinished())
	{
		if (t.elapsed() > 9000)
		{
			HMUtils::log("http") << "post, timeout" << HMLog::endl;
			reply->deleteLater();
			return QByteArray();
		}

		if (reply->error() != QNetworkReply::NoError)
		{
			reply->deleteLater();
			HMUtils::log("http") << "post, response error, " << reply->errorString() << HMLog::endl;
			return QByteArray();
		}

		QCoreApplication::processEvents();
	}

//	HMUtils::log("http") << "post, response charset is " << getCharsetFromReply(reply) << HMLog::endl;
	QByteArray bytes = reply->readAll();

	HMUtils::log("http") << "post, " << bytes.size() << " bytes received." << HMLog::endl;

	reply->deleteLater();

	return bytes;
}
