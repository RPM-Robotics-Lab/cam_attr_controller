#pragma once
#include <string>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

// #include "bluefox2_enumerator.h"
#include "Bluefox2Config.h"
#include "bluefox2_setting.h"

#include "lcmtypes++/bot_core/image_t.hpp"

using namespace std;

namespace bluefox2 {


class Bluefox2 {
public:
    explicit Bluefox2(const string &serial);
    ~Bluefox2();

    const string &serial() const { return serial_; }
    string product() const { return dev_->product.readS(); }
    int timeout_ms() const { return timeout_ms_; }
    void set_timeout_ms (int timeout_ms) { timeout_ms_ = timeout_ms;}

    int GetExposeUs() const;
    void SetExposeUs(int &expose_us);
    void SetGainDB(double &gain_db);
    void OpenDevice();
    void RequestSingle() const;

    void Configure(Bluefox2Config &config);
    // void Configure(Boluefox2DynConfig &config);

    // bool GrabImage(sensor_msgs::Image &image_msg);
    bool GrabImage(bot_core::image_t& img_t);
    bool SetExpEnsure();

    void SetMM(int mm) const;
    void SetMaster() const;
    void SetSlave() const;
    void SetFPSandExposeTime(int frameRate_Hz, int exposureTime_us) const;

    bool bluefox2_to_botimg(bot_core::image_t &img_t, const int encoding, const int32_t width, const int32_t height, const int32_t nChannel, const int step, const int bitsPerPixel, const void* data);

    void test_print();
    int64_t idx;

private:
    string AvailableDevice() const;

    bool IsCtmOnDemandSupported() const;

    // Settings
    void SetAoi(int &width, int &height) const;
    void SetIdpf(int &idpf) const;
    void SetCbm(int &cbm) const;

    void SetAgc(bool &auto_gain, double &gain_db) const;
    void SetAec(bool &auto_expose, int &expose_us) const;
    void SetAcs(int &acs, int &des_gray_val) const;

    void SetWbp(int &wbp, double &r_gain, double &g_gain, double &b_gain) const;
    void SetHdr(bool &hdr) const;
    void SetDcfm(int &dcfm) const;
    void SetCpc(int &cpc) const;
    void SetCtm(int &ctm) const;
    void SetCts(int &cts) const;
    void SetCem(int &cem) const;

    void SetCticm(bool &cticme, int &cticmm) const;
    void SetColorTwist(bool &cte, vector<double>& ctr0, vector<double>& ctr1, vector<double>& ctr2) const;
    void SetCtocm(bool &ctocme, int &ctocmm) const;

    void SetLUT(bool &lute, int &lutmode, int &lutimpl, int &lutmapping) const;
    void SetCfm(int &cfm) const;
    void SetCft(int &cft) const;

    // Request
    void FillCaptureQueue(int &n) const;
    void RequestImages(int n) const;

    // setupHRTC
    bool setupHRTC(mvIMPACT::acquire::Device *pDev, int frameRate_Hz, int exposureTime_us) const;

    int timeout_ms_{200};
    string serial_;

    Bluefox2Config config_;
    // Bluefox2DynConfig config_;
    mvIMPACT::acquire::Request *request_{nullptr};
    mvIMPACT::acquire::DeviceManager dev_mgr_;
    mvIMPACT::acquire::Device *dev_{nullptr};
    mvIMPACT::acquire::FunctionInterface *fi_{nullptr};
    // mvIMPACT::acquire::Statistics &stats_{nullptr};
    mvIMPACT::acquire::SettingsBlueFOX *bf_set_{nullptr};
    mvIMPACT::acquire::ImageProcessing *img_proc_{nullptr};
    mvIMPACT::acquire::CameraSettingsBlueFOX *cam_set_{nullptr};
    mvIMPACT::acquire::SystemSettings *sys_set_{nullptr};
    mvIMPACT::acquire::InfoBlueDevice *bf_info_{nullptr};

};

}
 
