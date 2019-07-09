#ifndef HMLOG_H
#define HMLOG_H


#include <string>
#include <QObject>
#include <QString>
#include <QFile>
#include <QMutexLocker>

class HMLog : public QObject
{
	Q_OBJECT
public:
	explicit HMLog(QObject *parent = 0);

	HMLog( const QString &fileName, QObject *parent = 0 );

	virtual ~HMLog();

signals:

public slots:

public:
	enum Flag
	{
		dec,
		hex,
		endl
	};
public:
	virtual
	bool
	open(
		const QString &fileName = QString(),
		const QString &prompt = QString(),
		bool enable = true,
		int maxSize = 1024 * 1024  * 10
		);

	virtual
	bool
	open(
		QIODevice *io,
		const QString &prompt = QString(),
		bool enable = true
		);

	virtual void close();
	virtual bool isOpen();

	virtual QString fileName();
	virtual void setFileName( const QString &name );

	/**
	  @brief 设置每行起始的提示符
	*/
	virtual void setPrompt( const QString &promptString );
	virtual QString prompt();

	virtual void disable();
	virtual void enable();

	virtual bool isEnable();

public:
	virtual HMLog& operator <<( HMLog::Flag flag );
	virtual HMLog& operator <<( const QByteArray &bytes );
	virtual HMLog& operator <<( const QString &str );
	virtual HMLog& operator <<( const std::string &str );
	virtual HMLog& operator <<( const char *str );
	virtual HMLog& operator <<( char *str );
	virtual HMLog& operator <<( int value );
	virtual HMLog& operator <<( unsigned int value );
	virtual HMLog& operator <<( long value );
	virtual HMLog& operator <<( unsigned long value );
	virtual HMLog& operator <<( char value );
	virtual HMLog& operator <<( unsigned char value );

protected:
	virtual void checkFileSize();

private:
	QFile file;
	QIODevice *io;
	bool isEnableLog;
	QString promptStr;
	QString nameOfFile;
	bool newLine;
	HMLog::Flag flag;
	int maxFileSize;
	QMutex mutex;
};

#endif // HMLOG_H
