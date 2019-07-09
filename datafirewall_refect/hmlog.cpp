#include "hmlog.h"

#include <QDateTime>
#include <QDebug>

#include <iostream>

#define USE_MUTEX

HMLog::HMLog(QObject *parent) :
	QObject(parent)
{
#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif
    maxFileSize = 1024 * 1024*10;
	isEnableLog = true;
	newLine = true;
	io = NULL;
	flag = HMLog::dec;
}

HMLog::HMLog(const QString &fileName, QObject *parent): QObject(parent)
{
#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif
    maxFileSize = 1024 * 1024 * 10;
	isEnableLog = true;
	newLine = true;
	io = NULL;
	flag = HMLog::dec;

	nameOfFile = fileName;
}

HMLog::~HMLog()
{
	close();
}

bool HMLog::open( const QString &fileName, const QString &prompt, bool enable, int maxSize )
{

	close();

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	maxFileSize = maxSize;

	if ( !fileName.isEmpty() )
		nameOfFile = fileName;

	if ( nameOfFile.isEmpty() )
		return false;

	file.setFileName( nameOfFile );

	if ( !file.open( QIODevice::ReadWrite | QIODevice::Append ) )
		return false;

//	file.seek( file.size() );

	io = dynamic_cast<QIODevice*>( &file );

	Q_ASSERT( io != NULL );

	setPrompt( prompt );

	if ( enable )
		this->enable();
	else
		this->disable();

	newLine = true;

	return true;
}


bool HMLog::open( QIODevice *io, const QString &prompt, bool enable )
{
	close();

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	this->io = io;

	if ( io == NULL )
		return false;

	if ( !io->isWritable() )
		return false;

	setPrompt( prompt );

	if ( enable )
		this->enable();
	else
		this->disable();

	newLine = true;

	return true;
}

void HMLog::close()
{
	if ( !isOpen() )
	{
		return;
	}

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( &file == io )
	{
		file.close();
		io = NULL;
	}
	else
	{
		io->close();
		io = NULL;
	}
}

bool HMLog::isOpen()
{
#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( io == NULL )
		return false;

	return io->isOpen();
}

QString HMLog::fileName()
{
	return nameOfFile;
}

void HMLog::setFileName( const QString &name )
{
	nameOfFile = name;
}

void HMLog::setPrompt( const QString &promptString )
{
	promptStr = promptString;
}

QString HMLog::prompt()
{
	return promptStr;
}

void HMLog::disable()
{
	isEnableLog = false;
}

bool HMLog::isEnable()
{
	return isEnableLog;
}

void HMLog::enable()
{
	isEnableLog = true;
}

void HMLog::checkFileSize()
{

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif
	if ( io != &file )
	{
		return;
	}

	if ( file.size() < maxFileSize )
	{
		return;
	}

	QFile tmpFile( fileName() + ".tmp" );

	tmpFile.open( QIODevice::Truncate | QIODevice::WriteOnly );

	file.seek( file.size() - maxFileSize / 100 );
	file.readLine();

	tmpFile.write( file.readAll() );

#ifdef USE_MUTEX
	locker.unlock();
#endif

	close();

#ifdef USE_MUTEX
	locker.relock();
#endif

	file.remove();

	tmpFile.close();
	tmpFile.rename( fileName() );

#ifdef USE_MUTEX
	locker.unlock();
#endif

	open( fileName(), prompt(), isEnable(), maxFileSize );

}

HMLog& HMLog::operator << ( HMLog::Flag flag )
{
#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( flag == HMLog::endl )
	{
#ifdef USE_MUTEX
	locker.unlock();
#endif
		*this << "\r\n";
		newLine = true;
	}
	else
	{
		this->flag = flag;
	}

	return *this;
}

HMLog& HMLog::operator <<( const QByteArray &bytes )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	if ( bytes.isEmpty() )
		return *this;

	if ( io == NULL )
		return *this;

	QString text;

	for ( int i = 0; i < bytes.size(); i++ )
	{
		unsigned char value = (unsigned char) bytes.at(i);

		text += QString( "%1" ).arg( value, 2, 16, QChar('0') ).toUpper();

		if ( !text.isEmpty() )
			text += " ";
	}

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << text;

	return *this;
}

HMLog& HMLog::operator << ( const QString &str )
{
	if ( !isOpen() )
	{
		return *this;
	}

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
	{
		return *this;
	}

	if ( str.isEmpty() )
	{
		return *this;
	}

	if ( io == NULL )
	{
		return *this;
	}

	QString lineHead;

	if ( newLine )
	{
		lineHead = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + " ";

		if ( !prompt().isEmpty() )
			lineHead += prompt();

		io->write( lineHead.toLocal8Bit() );

		newLine = false;
	}

	io->write( str.toLocal8Bit() );

	if ( io == &file )
	{
		file.flush();
#ifdef USE_MUTEX
	locker.unlock();
#endif
		checkFileSize();
	}

//	newLine = str.endsWith("\n");

//#ifdef QT_DEBUG
//	std::cout << (lineHead + str).toStdString();
//#endif

	return *this;
}

HMLog& HMLog::operator << ( const std::string &str )
{
	*this << str.c_str();

	return *this;
}

HMLog& HMLog::operator << ( const char *str )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	if ( str == NULL )
		return *this;

	QString tmp = str;

#ifdef USE_MUTEX
	locker.unlock();
#endif

	*this << tmp;

	return *this;
}

HMLog& HMLog::operator << ( char *str )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	if ( str == NULL )
		return *this;

	QString tmp = str;

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << tmp;

	return *this;
}

HMLog& HMLog::operator << ( int value )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	QString str;

	if ( flag == HMLog::dec )
		str = QString("%1").arg( value );
	else if ( flag == HMLog::hex )
		str = QString("%1").arg( value, 8,16, QChar('0') );

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << str;

	return *this;
}

HMLog& HMLog::operator << ( unsigned int value )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	QString str;

	if ( flag == HMLog::dec )
		str = QString("%1").arg( value );
	else if ( flag == HMLog::hex )
		str = QString("%1").arg( value, 8,16, QChar('0') );

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << str;

	return *this;
}

HMLog& HMLog::operator << ( long value )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	QString str;

	if ( flag == HMLog::dec )
		str = QString("%1").arg( value );
	else if ( flag == HMLog::hex )
		str = QString("%1").arg( value, 8, 16, QChar('0') );

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << str;

	return *this;
}

HMLog& HMLog::operator << ( unsigned long value )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	QString str;

	if ( flag == HMLog::dec )
		str = QString("%1").arg( value );
	else if ( flag == HMLog::hex )
		str = QString("%1").arg( value, 8, 16, QChar('0') );

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << str;

	return *this;
}

HMLog& HMLog::operator <<( char value )
{
	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif

	if ( !isEnable() )
		return *this;

	QString str;
	str = value;

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << str;

	return *this;
}

HMLog& HMLog::operator <<( unsigned char value )
{

	if ( !isOpen() )
		return *this;

#ifdef USE_MUTEX
	QMutexLocker locker(&mutex);
#endif
	if ( !isEnable() )
		return *this;

#ifdef USE_MUTEX
	locker.unlock();
#endif
	*this << ( (unsigned int) value );

	return *this;
}
