#include <unistd.h>
#include "bot_util/BotUtils.h"
#include "bluefox2.h"
#include "Bluefox2Config.h"

using namespace std;
using namespace mvIMPACT::acquire;

namespace bluefox2 {

Bluefox2::Bluefox2(const string &serial) : serial_(serial) {
    if(!(dev_ = dev_mgr_.getDeviceBySerial(serial))) {
        throw runtime_error(serial + " not found. " + AvailableDevice());
    }
    idx = 0;
    OpenDevice();
}

Bluefox2::~Bluefox2() {
    if (dev_ && dev_->isOpen()) {
        dev_->close();
    }
}

string Bluefox2::AvailableDevice() const {
    const auto dev_cnt = dev_mgr_.deviceCount();
    string devices = std::to_string(dev_cnt) + " available device(s): ";

    for (decltype(dev_mgr_.deviceCount()) i=0; i < dev_cnt; ++i) {
        devices += dev_mgr_.getDevice(i)->serial.read() + " ";
    }

    return devices;
}

void Bluefox2::OpenDevice() {
    try {
        dev_->open();
        cout << "The device " << dev_->serial.read() << " has been opened." << endl;
    }
    catch (const ImpactAcquireException &e) {
        throw runtime_error(e.what());
    }

    fi_ = new FunctionInterface(dev_);
    // stats_ = new Statistics (dev_);
    bf_set_ = new SettingsBlueFOX(dev_);
    img_proc_ = new ImageProcessing(dev_);
    cam_set_ = new CameraSettingsBlueFOX(dev_);
    sys_set_ = new SystemSettings(dev_);
    bf_info_ = new InfoBlueDevice(dev_);
}

int Bluefox2::GetExposeUs() const {
    if (request_ && request_->isOK()) {
        return request_->infoExposeTime_us.read();
    }
    else {
        return 0;
    }
}

void Bluefox2::SetExposeUs(int &expose_us) {
    bool auto_expose_off = false;
    WriteAndReadProperty(cam_set_->autoExposeControl, auto_expose_off);
    
    WriteAndReadProperty(cam_set_->expose_us, expose_us);
}

void Bluefox2::RequestSingle() const {
    int result = DMR_NO_ERROR;
    result = fi_->imageRequestSingle();
    if (result != DMR_NO_ERROR) {
        cout << serial() << ": Error while requesting image: "
             << ImpactAcquireException::getErrorCodeAsString(result)
             << endl;
    }
}

void Bluefox2::RequestImages(int n) const {
    for (int i=0; i<n; ++i) {
        fi_->imageRequestSingle();
        int requestNr = fi_->imageRequestWaitFor(timeout_ms_);
        fi_->imageRequestUnlock(requestNr);
    }
}

bool Bluefox2::GrabImage(bot_core::image_t& img_t)
{
    // NOTE: A request object is locked for the driver whenever the corresponding
    // wait function returns a valid request object.
    // All requests returned by
    // mvIMPACT::acquire::FunctionInterface::imageRequestWaitFor need to be
    // unlocked no matter which result mvIMPACT::acquire::Request::requestResult
    // contains.
    // http://www.matrix-vision.com/manuals/SDK_CPP/ImageAcquisition_section_capture.html

    int request_nr = INVALID_ID;
    request_nr = fi_->imageRequestWaitFor(timeout_ms_);

    // Check if request is valid
    if (!fi_->isRequestNrValid(request_nr)) {
        // We do not need to unlock here because the request is not valid?
        fi_->imageRequestUnlock(request_nr);
        return false;
    }

    request_ = fi_->getRequest(request_nr);

    // Check if request is ok
    if (!request_->isOK()) {
        // need to unlock here because the request is valid even if it is not ok
        fi_->imageRequestUnlock(request_nr);
        return false;
    }

    int encoding;
    const auto bayer_mosaic_parity = request_->imageBayerMosaicParity.read();
    if (bayer_mosaic_parity != bmpUndefined) {
        // Bayer pattern
        const auto bytes_per_pixel = request_->imageBytesPerPixel.read();
        encoding = BayerPatternToEncoding(bayer_mosaic_parity, bytes_per_pixel);
    }
    else {
        encoding = PixelFormatToEncoding(request_->imagePixelFormat.read());
    }
    // cout << "idpf val: " << request_->imagePixelFormat.read() << endl;
    // int encoding = PixelFormatToEncoding(request_->imagePixelFormat.read());

    bluefox2_to_botimg(img_t, encoding, request_->imageWidth.read(), request_->imageHeight.read(), request_->imageChannelCount.read(), request_->imageLinePitch.read(), request_->imagePixelPitch.read(), request_->imageData.read());
    
    // Release capture request
    fi_->imageRequestUnlock(request_nr);
    return true;

    return true;
}

bool Bluefox2::SetExpEnsure()
{
    // NOTE: A request object is locked for the driver whenever the corresponding
    // wait function returns a valid request object.
    // All requests returned by
    // mvIMPACT::acquire::FunctionInterface::imageRequestWaitFor need to be
    // unlocked no matter which result mvIMPACT::acquire::Request::requestResult
    // contains.
    // http://www.matrix-vision.com/manuals/SDK_CPP/ImageAcquisition_section_capture.html
    
    // request single
    fi_->imageRequestSingle();

    int request_nr = INVALID_ID;
    request_nr = fi_->imageRequestWaitFor(timeout_ms_);

    // Check if request is valid
    if (!fi_->isRequestNrValid(request_nr)) {
        // We do not need to unlock here because the request is not valid?
        fi_->imageRequestUnlock(request_nr);
        return false;
    }

    request_ = fi_->getRequest(request_nr);

    // Check if request is ok
    if (!request_->isOK()) {
        // need to unlock here because the request is valid even if it is not ok
        fi_->imageRequestUnlock(request_nr);
        return false;
    }

    // Release capture request
    fi_->imageRequestUnlock(request_nr);
    return true;
}

void Bluefox2::Configure(Bluefox2Config &config) {
    fi_->imageRequestReset(0, 0);

    // Area of Interset
    SetAoi(config.width, config.height);
    // Pixel Format
    SetIdpf(config.idpf);
    // Binning
    SetCbm(config.cbm);

    // Gain
    SetAgc(config.agc, config.gain_db);
    // Expose
    // Expose mode
    SetCem(config.cem);


    SetAec(config.aec, config.expose_us);
    // Auto Controller
    // SetAcs(config.acs, config.des_gray_val);

    // White Balance
    SetWbp(config.wbp, config.r_gain, config.g_gain, config.b_gain);
    // High Dynamic Range
    SetHdr(config.hdr);
    // Dark Current Filter
    SetDcfm(config.dcfm);
    // Pixel Clock
    SetCpc(config.cpc);
    // Trigger Mode
    SetCtm(config.ctm);
    // Trigger Source
    SetCts(config.cts);
    // Request
    FillCaptureQueue(config.request);

    // Color Twist Input Correction Matrix Mode
    SetCticm(config.cticme, config.cticmm);

    // Color Twist
    SetColorTwist(config.cte, config.ctrrow0, config.ctrrow1, config.ctrrow2);

    // Color Twist Output Correction Matrix Mode
    SetCtocm(config.ctocme, config.ctocmm);

    // LUT
    SetLUT(config.lute, config.lutmode, config.lutimpl, config.lutmapping);

    // Camera Flash Mode
    SetCfm(config.cfm);

    // Camera Flash Type
    SetCft(config.cft);

    // Cache this config
    config_ = config;
}

void Bluefox2::FillCaptureQueue(int &n) const {
    n = min<int>(n, fi_->requestCount() - 1);
    for(int i=0; i<n; ++i) {
        fi_->imageRequestSingle();
    }
}

void Bluefox2::SetAoi(int &width, int &height) const {
    // FIXEM: not implemented
}

void Bluefox2::SetIdpf(int &idpf) const {
    WriteAndReadProperty(bf_set_->imageDestination.pixelFormat, idpf);
}

void Bluefox2::SetCbm(int &cbm) const {
    WriteAndReadProperty(cam_set_->binningMode, cbm);
}

void Bluefox2::SetAgc(bool &auto_gain, double &gain_db) const {
    WriteAndReadProperty(cam_set_->autoGainControl, auto_gain);
    if (!auto_gain) {
        WriteAndReadProperty(cam_set_->gain_dB, gain_db);
    }
}

void Bluefox2::SetAec(bool &auto_exposure, int &expose_us) const {
    if(auto_exposure)   cout << "[Bluefox2]\t set autoexposure" << endl;
    WriteAndReadProperty(cam_set_->autoExposeControl, auto_exposure);
    if (!auto_exposure) {
        WriteAndReadProperty(cam_set_->expose_us, expose_us);
    }
}

// TODO: consider adding auto control limit here
void Bluefox2::SetAcs(int &acs, int &des_gray_val) const {
    if(cam_set_->autoControlParameters.isAvailable()) {
        bool agc = false, aec = false;
        ReadProperty(cam_set_->autoGainControl, agc);
        ReadProperty(cam_set_->autoExposeControl, aec);

        if(agc || aec) {
            if(acs != acs_unavailable) {
                WriteProperty(cam_set_->autoControlParameters.controllerSpeed, acs);
            }
        }
        
        ReadProperty(cam_set_->autoControlParameters.controllerSpeed, acs);
        const auto acp = cam_set_->autoControlParameters;
        WriteAndReadProperty(acp.desiredAverageGreyValue, des_gray_val);
        return;
    }
    acs = acs_unavailable;
}

void Bluefox2::SetWbp(int &wbp, double &r_gain, double &g_gain, double &b_gain) const
{
    // Put white balance as unavailable if it's not a color camera
    if (bf_info_->sensorColorMode.read() <= iscmMono) {
        wbp = wbp_unavailable;
        return;
    }
    
    // Predefined white balance parameters
    if (wbp < wbp_user1) {
        if (wbp > wbp_unavailable) {
            WriteProperty(img_proc_->whiteBalance, wbp);
        }
        ReadProperty(img_proc_->whiteBalance, wbp);
        return;
    }

      // User defined white balance parameters
    if (wbp == wbp_user1) {
        WriteProperty(img_proc_->whiteBalance, wbp);
        auto wbp_set = img_proc_->getWBUserSetting(0);
        WriteAndReadProperty(wbp_set.redGain, r_gain);
        WriteAndReadProperty(wbp_set.greenGain, g_gain);
        WriteAndReadProperty(wbp_set.blueGain, b_gain);
        return;
    }

    // TODO: considering support continuous white balance calibration?
    if (wbp == wbp_calibrate) {
        // Set wbp to user1
        WriteProperty(img_proc_->whiteBalance, wbpUser1);
        // Calibrate next frame
        WriteProperty(img_proc_->whiteBalanceCalibration, wbcmNextFrame);
        // Request one image?
        RequestImages(1);
        // Set config to user1 and update gains
        const auto wbp_set = img_proc_->getWBUserSetting(0);
        ReadProperty(wbp_set.redGain, r_gain);
        ReadProperty(wbp_set.greenGain, g_gain);
        ReadProperty(wbp_set.blueGain, b_gain);
        ReadProperty(img_proc_->whiteBalance, wbp);
    }
}


void Bluefox2::SetHdr(bool &hdr) const {
    auto &hdr_control = cam_set_->getHDRControl();
    if (!hdr_control.isAvailable()) {
        hdr = false;
        return;
    }

    WriteAndReadProperty(hdr_control.HDREnable, hdr);
    if (hdr) {
        // TODO: provide other HDR point?
        WriteProperty(hdr_control.HDRMode, cHDRmFixed0);
    }
}

void Bluefox2::SetDcfm(int &dcfm) const {
    if (dcfm == dcfmCalibrateDarkCurrent) {
        // Special case for calibrate mode
        // Set "OffsetAutoCalibration = Off"
        WriteProperty(cam_set_->offsetAutoCalibration, aocOff);
        // TODO: turn off auto control here?
        // Set filter mode = calibrate
        WriteProperty(img_proc_->darkCurrentFilterMode, dcfmCalibrateDarkCurrent);
        // Read image count, and request some more images
        int img_cnt = img_proc_->darkCurrentFilterCalibrationImageCount.read();
        RequestImages(img_cnt);
        // Then turn on immediately
        WriteProperty(img_proc_->darkCurrentFilterMode, dcfmOn);
        WriteProperty(cam_set_->offsetAutoCalibration, aocOn);
        ReadProperty(img_proc_->darkCurrentFilterMode, dcfm);
    } 
    else {
        WriteAndReadProperty(img_proc_->darkCurrentFilterMode, dcfm);
    }
}

void Bluefox2::SetCpc(int &cpc) const {
    WriteAndReadProperty(cam_set_->pixelClock_KHz, cpc);
}


void Bluefox2::SetCtm(int &ctm) const {
    // Do nothing when set to hard sync
    if (ctm == hard_sync) return;
    WriteAndReadProperty(cam_set_->triggerMode, ctm);
}

void Bluefox2::SetCts(int &cts) const {
    // Do nothing when trigger source is not visible
    if (!cam_set_->triggerSource.isVisible()) {
        cts = cts_unavailable;
        return;
    }
    WriteAndReadProperty(cam_set_->triggerSource, cts);
}

bool Bluefox2::IsCtmOnDemandSupported() const {
    vector<TCameraTriggerMode> values;
    cam_set_->triggerMode.getTranslationDictValues(values);
    return find(values.cbegin(), values.cend(), ctmOnDemand) !=
        values.cend();
}

void Bluefox2::SetMM(int mm) const {
    WriteProperty(img_proc_->mirrorModeGlobal, mm);
}


void Bluefox2::SetMaster() const {
    // Prefer on demand if it's available
    if (IsCtmOnDemandSupported()) {
        cam_set_->triggerMode.write(ctmOnDemand);
    } 
    else {
        cam_set_->triggerMode.write(ctmContinuous);
    }
    cam_set_->flashMode.write(mvIMPACT::acquire::cfmDigout0);
    cam_set_->flashType.write(mvIMPACT::acquire::cftStandard);
    cam_set_->flashToExposeDelay_us.write(0);
    cout << serial() << ": master" << std::endl;
}

void Bluefox2::SetSlave() const {
    cam_set_->triggerMode.write(ctmOnHighLevel);
    cam_set_->triggerSource.write(ctsDigIn0);
    cam_set_->frameDelay_us.write(0);
    cout << serial() << ": slave" << endl;
}

void Bluefox2::SetFPSandExposeTime(int frameRate_Hz, int exposureTime_us) const {
    mvIMPACT::acquire::Connector connector(dev_);
    CameraSettingsBlueFOX cs(dev_);
    // int sensorHeadCount = 1;//connector.videoChannel.read(plMaxValue)+1;

    setupHRTC(dev_, frameRate_Hz, exposureTime_us);
}

void Bluefox2::SetCem(int &cem) const {
    WriteAndReadProperty(cam_set_->exposeMode, cem);
}

void Bluefox2::SetCticm(bool &cticme, int &cticmm) const {
    WriteAndReadProperty(img_proc_->colorTwistInputCorrectionMatrixEnable, cticme);
    if (cticme) {
        WriteAndReadProperty(img_proc_->colorTwistInputCorrectionMatrixMode, cticmm);
    }
}

void Bluefox2::SetColorTwist(bool &cte, vector<double>& ctr0, vector<double>& ctr1, vector<double>& ctr2) const {

    WriteAndReadProperty(img_proc_->colorTwistEnable, cte);

    if (cte) {
        std::vector<double> row( 3, 0. );
        row[0] = ctr0[0];    row[1] = ctr0[1];    row[2] = ctr0[2];
        img_proc_->colorTwistRow0.write( row, cte );

        row[0] = ctr1[0];    row[1] = ctr1[1];    row[2] = ctr1[2];
        img_proc_->colorTwistRow1.write( row, cte );

        row[0] = ctr2[0];    row[1] = ctr2[1];    row[2] = ctr2[2];
        img_proc_->colorTwistRow2.write( row, cte );
    }
}

void Bluefox2::SetCtocm(bool &ctocme, int &ctocmm) const {
    WriteAndReadProperty(img_proc_->colorTwistOutputCorrectionMatrixEnable, ctocme);
    if (ctocme) {
        WriteAndReadProperty(img_proc_->colorTwistOutputCorrectionMatrixMode, ctocmm);
    }
}

void Bluefox2::SetLUT(bool &lute, int &lutmode, int &lutimpl, int &lutmapping) const {
    WriteAndReadProperty(img_proc_->LUTEnable, lute);
    if(lute) {
        WriteAndReadProperty(img_proc_->LUTMode, lutmode);
        WriteAndReadProperty(img_proc_->LUTImplementation, lutimpl);
        WriteAndReadProperty(img_proc_->LUTMappingSoftware, lutmapping);
    }
}

void Bluefox2::SetCfm(int &cfm) const {
    WriteAndReadProperty(cam_set_->flashMode, cfm);
}

void Bluefox2::SetCft(int &cft) const {
    WriteAndReadProperty(cam_set_->flashType, cft);
}

bool Bluefox2::bluefox2_to_botimg(bot_core::image_t &img_t, const int encoding, const int32_t width, const int32_t height, const int32_t nChannel, const int step, const int bitsPerPixel, const void* data)
{
//    img_t.nmetadata = 0;
    // img_t.utime = idx++;
    img_t.utime = bot_util::timestamp_now();
    img_t.width = width;
    img_t.height = height;
    img_t.row_stride = 1 * bitsPerPixel * width;

    img_t.pixelformat = encoding;//PixelFormatToEncoding(pixel_format);

    size_t st0 = (step * height);
    img_t.size = st0;
    img_t.data.resize(st0);
    memcpy(&img_t.data[0], data, st0);

    // std::cout << "utime: " << img_t.utime << std::endl;
    // std::cout << "width: " << img_t.width << std::endl;
    // std::cout << "height: " << img_t.height << std::endl;
    // std::cout << "channel: " << nChannel << std::endl;
    // std::cout << "row stride: " << img_t.row_stride << std::endl;
    // std::cout << "pixelformat: " << img_t.pixelformat << std::endl;
    // std::cout << "size: " << img_t.size << std::endl;
    // std::cout << "size2: " << img_t.data.size() << std::endl;

    return true;
}

bool Bluefox2::setupHRTC(mvIMPACT::acquire::Device *pDev, int frameRate_Hz, int exposureTime_us) const
{
    cout << "Trying to capture at " << frameRate_Hz << " frames per second. Please make sure the device can deliver this frame rate." << endl;

    int frametime_us = static_cast<int>( 1000000.0*( 1.0/static_cast<double>( frameRate_Hz)));

    const int TRIGGER_PULSE_WIDTH_us = 100;

    if( frametime_us < 2 * TRIGGER_PULSE_WIDTH_us )
    {
        cout << "frame rate too high (" << frameRate_Hz << "). Will use 5 Hz." << endl;
        frametime_us = 200000;
    }
    if( exposureTime_us > ( frametime_us - 2 * TRIGGER_PULSE_WIDTH_us ) )
    {
        cout << "exposure time too high(" << exposureTime_us << "). Will use " << frametime_us - 2 * TRIGGER_PULSE_WIDTH_us << " instead" << endl;
        exposureTime_us = frametime_us - 2 * TRIGGER_PULSE_WIDTH_us;
    }    

    {
        cout << "Frame time is " << frametime_us << " usec. " << endl;
        cout << "Exposure time is " << exposureTime_us << " usec. " << endl;
        
        bool aec = false;
        SetAec(aec, exposureTime_us);

        // int cts = bluefox2::cts_RTCtrl;
        // SetCts(cts);

        // int ctm = bluefox2::ctm_on_high_level;
        // SetCtm (ctm);
    }

    mvIMPACT::acquire::IOSubSystemBlueFOX iossbf( pDev );
    // error checks
    if( iossbf.RTCtrProgramCount() == 0 )
    {
        // no HRTC controllers available
        cout << "This device (" << pDev->product.read() << ") doesn't support HRTC" << endl;
        return false;
    }
    mvIMPACT::acquire::RTCtrProgram* pRTCtrlprogram = iossbf.getRTCtrProgram( 0 );
    if( !pRTCtrlprogram )
    {
        // this only should happen if the system is short of memory
        cout << "Error! No valid program. Short of memory?" << endl;
        return false;
    }

    // start of the program
    // we need 5 steps for the program
    pRTCtrlprogram->setProgramSize( 5 );
    // wait a certain amount of time to achieve the desired frequency
    int progStep = 0;
    // int i = 0;
    RTCtrProgramStep* pRTCtrlStep = 0;
    pRTCtrlStep = pRTCtrlprogram->programStep( progStep++ );
    pRTCtrlStep->opCode.write( rtctrlProgWaitClocks );
    pRTCtrlStep->clocks_us.write( frametime_us - exposureTime_us );

    // trigger both sensor heads
    pRTCtrlStep = pRTCtrlprogram->programStep( progStep++ );
    pRTCtrlStep->opCode.write( rtctrlProgTriggerSet );
    // for( i = 0; i < sensorHeadCount; i++ )
    // {
    //     pRTCtrlStep->sensorHeads.write( digioOn, i );
    // }

    // high time for the trigger signal (should not be smaller than 100 us)
    pRTCtrlStep = pRTCtrlprogram->programStep( progStep++ );
    pRTCtrlStep->opCode.write( rtctrlProgWaitClocks );
    pRTCtrlStep->clocks_us.write( exposureTime_us );
    
    // end trigger signal
    pRTCtrlStep = pRTCtrlprogram->programStep( progStep++ );
    pRTCtrlStep->opCode.write( rtctrlProgTriggerReset );
    // for( i = 0; i < sensorHeadCount; i++ )
    // {
    //     pRTCtrlStep->sensorHeads.write( digioOff, i );
    // }

    // restart the program
    pRTCtrlStep = pRTCtrlprogram->programStep( progStep++ );
    pRTCtrlStep->opCode.write( rtctrlProgJumpLoc );
    pRTCtrlStep->address.write( 0 );
    
    // start the program
    pRTCtrlprogram->mode.write( rtctrlModeRun );
    
    // Now this device will deliver synchronous images at exactly the desired frequency
    // when it is constantly feed with image requests and the device can deliver
    // images at this frequency.  

    return true;
}

void Bluefox2::test_print() {
    cout << request_->imageWidth.read() << ", " << request_->imageHeight.read() << endl;
}

}
