#include "hmutils.h"

#include <QSettings>
#include <QDebug>

#include <string.h>
#include <iostream>
using namespace std;


QMap<QString, HMLog*> HMUtils::logs;

HMUtils::HMUtils(QObject *parent) : QObject(parent)
{

}

HMLog& HMUtils::log(const QString &name)
{
	HMLog *l = HMUtils::logs.value(name);

	if (l == NULL)
	{
		QString logName = name;
		if (!name.endsWith(".log"))
			logName += ".log";

		logName.replace("/", "_");
		logName.replace("\\", "_");
		logName.replace("*", "_");
		logName.replace("?", "_");
		logName.replace(":", "_");
		logName.replace("%", "_");
		logName.replace("$", "_");

		if (!logName.startsWith("_"))
			logName = "_" + logName;

		l = new HMLog(logName);
		HMUtils::logs[name] = l;
	}

	if (!l->isOpen())
	{
		l->open();
	}

    return *l;
}

QByteArray HMUtils::uintToByte(uint number)
{
    QByteArray abyte;
    abyte.resize(4);
    abyte[0] = (uchar)  (0x000000ff & number);
    abyte[1] = (uchar) ((0x0000ff00 & number) >> 8);
    abyte[2] = (uchar) ((0x00ff0000 & number) >> 16);
    abyte[3] = (uchar) ((0xff000000 & number) >> 24);
    return abyte;
}

QByteArray HMUtils::uintToByteBig(uint number)
{
    QByteArray abyte;
    abyte.resize(4);
    abyte[3] = (uchar)  (0x000000ff & number);
    abyte[2] = (uchar) ((0x0000ff00 & number) >> 8);
    abyte[1] = (uchar) ((0x00ff0000 & number) >> 16);
    abyte[0] = (uchar) ((0xff000000 & number) >> 24);
    return abyte;
}

QByteArray HMUtils::ushortToByte(ushort number)
{
    QByteArray abyte;
    abyte.resize(2);
    abyte[0] = (uchar)  (0x00ff & number);
    abyte[1] = (uchar) ((0xff00 & number) >> 8);
    return abyte;
}

QByteArray HMUtils::ushortToByteBig(ushort number)
{
    QByteArray abyte;
    abyte.resize(2);
    abyte[1] = (uchar)  (0x00ff & number);
    abyte[0] = (uchar) ((0xff00 & number) >> 8);
    return abyte;
}

QByteArray HMUtils::int3ToByte(uint number)
{
    QByteArray abyte;
    abyte.resize(3);
    abyte[0] = (uchar)  (0x000000ff & number);
    abyte[1] = (uchar) ((0x0000ff00 & number) >> 8);
    abyte[2] = (uchar) ((0x00ff0000 & number) >> 16);
    return abyte;
}

uint HMUtils::byteArrayToUint(const QByteArray &a)
{
    if(a.length() < 3)
    {
        return 0;
    }
    unsigned char balanceL = a.at(0);
    unsigned char balanceM = a.at(1);
    unsigned char balanceH = a.at(2);

    int balance = 0;
    balance = ((((balance | balanceH) << 8) | balanceM) << 8) | balanceL;
    return balance;
}

uint HMUtils::byteArrayToUint4Byte(const QByteArray &a)
{
    if(a.length() < 4)
    {
        return 0;
    }
    unsigned char data0 = a.at(0);
    unsigned char data1 = a.at(1);
    unsigned char data2 = a.at(2);
    unsigned char data3 = a.at(3);

    int balance = 0;
    balance = (data3 << 24) | (data2 << 16) | (data1 << 8) | data0;

    return balance;
}

int HMUtils::byteArrayToInt(const QByteArray &a)
{


    if(a.length() < 3)
    {
        return 0;
    }
    char byte = a.at(2);
    int v = 0;
//    qDebug() << a;
    QByteArray bytes = a;

//    bytes.append(a.at(2));
//    bytes.append(a.at(1));
//    bytes.append(a.at(0));

    if (byte &80)
    {
        ((char*)&v)[0] = bytes.at(0);
        ((char*)&v)[1] = bytes.at(1);
        ((char*)&v)[2] = bytes.at(2);
        ((char*)&v)[3] = 0xFF;
    }
    else
    {
        ((char*)&v)[0] = bytes.at(0);
        ((char*)&v)[1] = bytes.at(1);
        ((char*)&v)[2] = bytes.at(2);
        ((char*)&v)[3] = 0x00;
    }
    qDebug() << v;
    return v;
}



ushort HMUtils::byteToUshort(const QByteArray &byte)
{
    if(byte.length() < 2)
    {
        return 0;
//        byte.insert(1,(char)0);
    }
    uchar d = byte.at(0);
    uchar d1 = byte.at(1);
    ushort ret = (d1 << 8) | d;
    return ret;
}

ushort HMUtils::byteToUshortBig(const QByteArray &byte)
{
    if(byte.length() < 2)
    {
        return 0;
    }
    uchar d = byte.at(0);
    uchar d1 = byte.at(1);
    ushort ret = (d << 8) | d1;
    return ret;
}

uchar HMUtils::calcXorSum(const QByteArray &bytes)
{
    unsigned char bcc = 0;
    for(int i = 0;i<bytes.size();i++)
    {
       bcc ^= (unsigned char)bytes.at(i);
    }
    return bcc;
}

int HMUtils::setMachineState(int src, int des)
{
    int state = 0;

    switch (src)
    {
    case 3://emergence
        state = src;
        break;
    case 0://normal
        state = des;
        break;
    case 1://alarm
    {
        if(des == 2)
        {
            state = des;
        }
        else
        {
            state = src;
        }
    }
        break;
    case 2://err
        state = src;
        break;
    default:
        break;
    }

    return state;
}

uint HMUtils::dateTimeToSec(QDateTime &time)
{
    return time.toTime_t();
}

QDateTime HMUtils::secToDateTime(uint sec)
{
    return QDateTime::fromTime_t(sec);
}

QByteArray HMUtils::overturnArray(const QByteArray &bytes)
{
    int len = bytes.length();
    QByteArray tmp;
    tmp.resize(len);
    for(int i=0; i<len; i++)
    {
        tmp[len -1 - i] = bytes[i];
    }
    return tmp;
}

QByteArray HMUtils::stringToArrray(const QString &str)
{
    QByteArray byte = QByteArray::fromHex(str.toLocal8Bit());
    return byte;
}

uchar HMUtils::decToBCD(uchar temp)
{
    return ( (temp/10)*16 + temp%10 ) ;
}

QString HMUtils::byteArrayToString(QByteArray array)
{
     return QString(array.toHex());
}



QVariant
HMUtils::settingValue(
				const QString &name,
				const QString &group,
				const QVariant &defaultValue,
				const QString &filename
				)
{
	QSettings settings(filename, QSettings::IniFormat);

	settings.beginGroup(group);
	QVariant value = settings.value(name, defaultValue);
	settings.endGroup();

	return value;
}


void
HMUtils::setSettingValue(
		const QVariant &value,
		const QString &name,
		const QString &group,
		const QString &filename
		)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.beginGroup(group);
	settings.setValue(name, value);
    settings.endGroup();
}



bool
HMUtils::verifySignatureBySha1WithRSA(
                                const QString &signatureData,
                                const QString &publicKey,
                                const QString &orginal
                                )
{
#ifdef OPENSSL
    //签名数据
//    QString tmp = "gRU3kL+YLUFeOyTSbi5femAGloe72K9m22sGz6Ztdo8oeZ1UfpITqlowoMcuXLxhQwsDM+lgBWJESru1mzxAAQPNzchOFdVRQLjDERqLpSQ/onh6prXO1sj5JAQYnle/ZzM+qbixJuba8V0SLUu9I0dsohI94TJfB0QwG1ZVaPc=";
    QByteArray arrEncode = signatureData.toLocal8Bit();

    QByteArray arrBase64DecodedSig = QByteArray::fromBase64(arrEncode);
    string strBase64DecodedSig = arrBase64DecodedSig.toStdString();
    const char * signature = strBase64DecodedSig.c_str();

//    //公钥
//    string strKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC43bVBZRp9yamKOtLojNO4VrOncPQsDa7RWqAyn5cbyJzEnRwO7JzrDUcGiomHz6QwUeFIe8SkSSM86JiiQqo8Z6Dj"
//            "c5fYYzyb6Q80uMRiM3+9KhjM7xaJhdhQzWlPVofaW2Bp9y8ErYjWLaqsy4gizg+A"
//            "56Rny+3wQ6/2XI2lHwIDAQAB";
//    //公钥
//    QString qStringPubKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC43bVBZRp9yamKOtLojNO4VrOncPQsDa7RWqAyn5cbyJzEnRwO7JzrDUcGiomHz6QwUeFIe8SkSSM86JiiQqo8Z6Dj"
//            "c5fYYzyb6Q80uMRiM3+9KhjM7xaJhdhQzWlPVofaW2Bp9y8ErYjWLaqsy4gizg+A"
//            "56Rny+3wQ6/2XI2lHwIDAQAB";


    string strKey;// = publicKey.toStdString();
    int nPublicKeyLen = publicKey.size();      //strPublicKey为base64编码的公钥字符串
    //公钥格式化
    QString tmp;
    for (int i = 0; i < nPublicKeyLen; i+=64)
    {
        if (!tmp.isEmpty())
            tmp += "\n";

        tmp += publicKey.mid(i, 64);
    }

    strKey = "-----BEGIN PUBLIC KEY-----\n";
    strKey += tmp.toStdString();
    strKey += "\n-----END PUBLIC KEY-----\n";

    RSA *rsa;
    BIO *bio = NULL;

	//------------------------
	//hugaopeng 20171205
	char theKey[4096] = {0};
	strcpy(theKey, strKey.c_str());
	//------------------------


	if ((bio = BIO_new_mem_buf(theKey, -1)) == NULL)       //从字符串读取RSA公钥
    {
            return false;
    }

    if((rsa = PEM_read_bio_RSA_PUBKEY(bio,NULL,NULL,NULL))== NULL)// 从bio结构中得到rsa结构
    {
        ERR_load_crypto_strings();
        char errBuf[512];
        ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
        BIO_free_all(bio);
        return 0;
    }

    //未加密数据
//    QString unEncryptedData = "6635020120028247103d39ef768e9a4bf6a42ddb71534417ef70050011509683922987";
//    string strOriginal = "6635020120028247103d39ef768e9a4bf6a42ddb71534417ef70050011509683922987";

    string strOriginal = orginal.toStdString();


//    char *chInAppData = const_cast<char *>(strOriginal.toStdString().c_str());//原始数据，即未加密数据

    EVP_PKEY *evpKey = NULL;
    EVP_MD_CTX *ctx = NULL;
    evpKey = EVP_PKEY_new();

    int result = 0;
    if(evpKey == NULL)
    {
        RSA_free(rsa);
        BIO_free_all(bio);
        return false;
    }

    if((result = EVP_PKEY_set1_RSA(evpKey,rsa)) != 1)
    {
        RSA_free(rsa);
        EVP_PKEY_free(evpKey);
        BIO_free_all(bio);
        return false;
    }

//     EVP_MD_CTX_init(ctx);
    ctx = EVP_MD_CTX_new();

    if(result == 1 && (result = EVP_VerifyInit_ex(ctx, EVP_sha1(), NULL)) != 1)
    {
        return false;
    }

    if(result == 1 && (result = EVP_VerifyUpdate(ctx, strOriginal.c_str(),strOriginal.size())) != 1)
    {
        return false;
    }

    int strBase64DecodedSigSize = strBase64DecodedSig.size();

    if(result == 1 && (result = EVP_VerifyFinal(ctx, (unsigned char *)signature,strBase64DecodedSigSize , evpKey)) != 1)
    {
        return false;
    }

    if(result == 1)
    {
        qDebug()<<"Verify success" << endl;
    }
    else
    {
        ERR_load_crypto_strings();
        char errBuf[512];
        ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
        qDebug()<<"verify failed[" << errBuf<< "]" << endl;
        return false;
    }

    EVP_MD_CTX_free(ctx);
    RSA_free(rsa);
    EVP_PKEY_free(evpKey);
    BIO_free_all(bio);


    return true;
#else
	Q_UNUSED(signatureData);
	Q_UNUSED(publicKey);
	Q_UNUSED(orginal);
	return true;
#endif
}

QString HMUtils::getM1CardNum(QString &CSN)
{
    QString csnValue = 0;
    QString cn = 0;
    QString cnStr = 0;
    getCSN(CSN,cnStr);
    CSNTransposition(cnStr,csnValue);
    CSNReplace(csnValue,cn);
    return cn;
}

void HMUtils::getCSN(QString &csn, QString &csnum)
{
    bool ok = true;
    uint csnumber = csn.toUInt(&ok,16);
    if(!csnumber)
    {
        csnumber = csn.toUInt();
    }

    QString strCSN = QString::number(csnumber);

    int strNum = 10-strCSN.length();
    QChar ch = '0';

    if(strNum != 0)
    {
        QByteArray arr;
        for(int i=0;i<strNum;i++)
        {
            arr.append(ch);
        }
        csnum = strCSN.insert(0,arr);
    }
    else
    {
        csnum = strCSN;
    }

//    qDebug() << "csnumber:" << csnum << endl;
//    return true;
}

bool HMUtils::CSNTransposition(QString &csnumber, QString &csnumberSetup1)
{
    QString csnumberStrValue;
    transposition(csnumber,0,4,csnumberStrValue);
    transposition(csnumberStrValue,1,3,csnumberStrValue);
    transposition(csnumberStrValue,2,7,csnumberStrValue);
    transposition(csnumberStrValue,5,9,csnumberStrValue);
    transposition(csnumberStrValue,6,8,csnumberStrValue);

    csnumberSetup1 =  csnumberStrValue;
    return true;
}

bool HMUtils::CSNReplace(QString &csnumberSetup1, QString &cn)
{
    QString csValue;
    replace(csnumberSetup1,csValue,'1','2');
    replace(csValue,csValue,'3','4');
    replace(csValue,csValue,'5','6');
    replace(csValue,csValue,'7','8');
    replace(csValue,csValue,'9','0');

    cn = csValue;
    return true;
}

void HMUtils::transposition(QString &csnumberStr, int i, int j, QString &value)
{
    const QChar frist = csnumberStr.at(i);
    const QChar sec = csnumberStr.at(j);

    csnumberStr.replace(i,1,sec);
    value = csnumberStr.replace(j,1,frist);
}

void HMUtils::replace(QString &before, QString &after, QChar head, QChar end)
{

    for(int i=0;i<before.length();i++)
    {
        if(before.at(i) == head)
        {
            before.replace(i,1,end);
        }
        else if(before.at(i) == end)
        {
            before.replace(i,1,head);
        }
    }

    after.swap(before);
}

uint HMUtils::byteToUint(const QByteArray &a, int len)
{
    int i = 0;
    uint sum = 0;

    for(i = 0;i<len;i++)
    {
        char c = 0;
        c = a.at(i);
        c = c << (i*8);
        sum = sum | c;
    }

    return sum;
}

uint HMUtils::byteToUintBig(const QByteArray &a)
{
//    int i = 0;
    uint sum = 0;

//    len = a.length();

//    for(i = 0;i<len;i++)
//    {
//        sum <<= 8;
//        sum |= ((uint)a.at(i) & 0xFF);
//    }

    ((char*)&sum)[0] = a.at(3);
    ((char*)&sum)[1] = a.at(2);
    ((char*)&sum)[2] = a.at(1);
    ((char*)&sum)[3] = a.at(0);

    return sum;
}


