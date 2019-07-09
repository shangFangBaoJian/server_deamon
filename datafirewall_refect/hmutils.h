#ifndef HMUTILS_H
#define HMUTILS_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "hmlog.h"
#include <QDateTime>

/**
  @brief 全局函数类
  @author hu gao peng
  @date 2016-08-01
*/

/**
 * @brief The IndustryInfo struct 行业数据的数据结构组成解析
 * @author chen jian wen
 * @date 2018-031-17
 */
struct IndustryInfo
{
    //加密算法标志
    char encryptAlgorithmFlagByteSize;
    //密钥索引
    char keyIndexByteSize;
    //商户账户码
    char merchantsAccountNumByteSize;
    //账户号码
    char accountNumByteSize;
    //账户认证码
    char accountCertificatNumByteSize;
    //发行商认证MAC
    char issureMACByteSize;
    //卡类型
    char cardTypeByteSize;
    //发码流水号
    char sendCodeSerialNumByteSize;
    //认证类型
    char certificatTypeByteSize;
    //认证类型MAC
    char certificatTypeMACByteSize;
    //分散因子
    char dispersFactorByteSize;
    //应用认证码
    char appCertificatCodeByteSize;
    //预留字段
    char RFUByteSize;
    //二维码有效时长
    char qrCodeValidDateByteSize;

public:
    IndustryInfo()
    {
        //加密算法标志
        encryptAlgorithmFlagByteSize = 1;
        //密钥索引
        keyIndexByteSize = 1;
        //商户账户码
        merchantsAccountNumByteSize = 16;
        //账户号码
        accountNumByteSize = 8;
        //账户认证码
        accountCertificatNumByteSize = 8;
        //发行商认证MAC
        issureMACByteSize = 4;
        //卡类型
        cardTypeByteSize = 2;
        //发码流水号
        sendCodeSerialNumByteSize = 8;
        //认证类型
        certificatTypeByteSize = 1;
        //认证类型MAC
        certificatTypeMACByteSize = 8;
        //分散因子
        dispersFactorByteSize = 8;
        //应用认证码
        appCertificatCodeByteSize = 8;
        //预留字段
        RFUByteSize = 16;
        //二维码有效时长
        qrCodeValidDateByteSize = 2;
    }
};



class HMUtils : public QObject
{
	Q_OBJECT
public:
	explicit HMUtils(QObject *parent = 0);

signals:

public slots:

public:
	static HMLog& log(const QString &name = "hmapp.log");

    static QByteArray  uintToByte(uint number);
    static QByteArray  uintToByteBig(uint number);
    static QByteArray  ushortToByte(ushort number);
    static QByteArray  ushortToByteBig(ushort number);
    static QByteArray  int3ToByte(uint number);

    static uint byteArrayToUint(const QByteArray& a);
    static int byteArrayToInt(const QByteArray& a);
    static uint byteArrayToUint4Byte(const QByteArray& a);
    static ushort byteToUshort(const QByteArray & byte);
    static ushort byteToUshortBig(const QByteArray &byte);
    static uchar calcXorSum(const QByteArray& byte);

    static int setMachineState(int src, int des);


    //将 1970 1 1 至time转换成秒数
    static uint dateTimeToSec(QDateTime & time);
    static QDateTime secToDateTime(uint sec);
    static QByteArray overturnArray(const QByteArray& bytes);
    static QByteArray stringToArrray(const QString &str);

    /**
         * @brief decToBCD 1字节的十进制转换为bcd码
         * @param temp
         * @return
         */
    static uchar decToBCD(uchar temp);
    static QString byteArrayToString(QByteArray array);

    static
	QVariant
	settingValue(
			const QString &name,
			const QString &group,
			const QVariant &defaultValue = QVariant(),
			const QString &filename = "hmapp.ini"
			);

	static
	void
	setSettingValue(
			const QVariant &value,
			const QString &name,
			const QString &group,
			const QString &filename = "hmapp.ini"
			);


    ///
    /// \brief verifySignature
    /// \param signature 二维码签名数据
    /// \param publicKey 公钥
    /// \param orginal 原始数据，未加密的数据
    /// \return 验证通过返回true,失败返回false
    ///
    static
    bool
    verifySignatureBySha1WithRSA(
                    const QString &signatureData,
                    const QString &publicKey,
                    const QString &orginal
                    );

    //20180124 chenjianwen
    //////////
    /// \brief getM1CardNum 厦门M1卡内号转换卡面号
    /// \param CSN
    /// \return
    ///
    static QString getM1CardNum(QString &CSN);
    static void getCSN(QString &csn,QString &csnum);
    static bool CSNTransposition(QString &csnumber, QString &csnumberSetup1);
    static bool CSNReplace(QString &csnumberSetup1, QString &cn);
    static void transposition(QString &csnumberStr,int i,int j, QString &value);
    static void replace(QString &before, QString &after,QChar head,QChar end);


    /////
    /// \brief byteToUint 20180316 实现任意长度的byte转uint，
    /// \param a
    /// \param len
    /// \return
    ///
    static uint byteToUint(const QByteArray &a,int len);

    static uint byteToUintBig(const QByteArray &a);

private:
	static QMap<QString, HMLog*> logs;
};

#endif // HMUTILS_H
