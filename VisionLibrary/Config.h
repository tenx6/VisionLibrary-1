#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "BaseType.h"
#include "VisionType.h"

namespace AOI
{
namespace Vision
{

class Config
{
protected:
    enum class BGR_CHANNEL {
        BLUE,
        GREEN,
        RED,
    };

    Config();

    String           _strRecordDir              = "./Vision/Record/";
    String          _strLogCaseDir              = "./Vision/Logcase/";
    String          _strRecordPrefix            = "";
    String          _strRecordLogPrefix         = "tmplDir.";
    String          _strRecordParamFile         = "param.yml";
    String          _strRecordExt               = ".rcd";
    PR_DEBUG_MODE   _enDebugMode                = PR_DEBUG_MODE::DISABLED;
    Int16           _nDeviceInspChannel         = static_cast<Int16>( BGR_CHANNEL::RED );
    Int32           _nRemoveCCMaxComponent      = 500;
    String          _strOcrCharList             = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    Int32           _nInspBridgeMinThreshold    = 100;
    Int32           _nMinPointsToCalibCamera    = 10;

public:
    static Config*          GetInstance();
    String                  getRecordDir() const;
    String                  getLogCaseDir() const;
    String                  getRecordPreFix() const;
    String                  getRecordExt() const;
    String                  getRecordLogPrefix() const;
    String                  getRecordParamFile() const;
    PR_DEBUG_MODE           getDebugMode() const;
    Int16                   getDeviceInspChannel() const;
    void                    setDebugMode(PR_DEBUG_MODE enDebugMode);
    Int32                   getRemoveCCMaxComponents();
    String                  getOcrCharList() const;
    Int32                   getInspBridgeMinThreshold() const;
    Int32                   getMinPointsToCalibCamera() const;
    void load();
};

#define ConfigInstance Config::GetInstance()

}
}
#endif