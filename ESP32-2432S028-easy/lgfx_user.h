#ifndef __LGFX_USER__
#define __LGFX_USER__

#include <LovyanGFX.hpp>
#include <Arduino.h>

// INTENDED TO BE USED WITH ESP32-2432S028 2.8inch ALIExpress boards (sold in 2024)

class LGFX : public lgfx::LGFX_Device {

  lgfx::Panel_ST7789    _panel_instance;
  lgfx::Bus_SPI         _bus_instance;
  lgfx::Light_PWM       _light_instance;
  lgfx::Touch_XPT2046   _touch_instance;

public:
  LGFX(void) {
    { 
      auto cfg = _bus_instance.config();
      cfg.spi_host      = SPI2_HOST;
      cfg.spi_mode      =         0;
      cfg.freq_write    =  40000000;
      cfg.freq_read     =  10000000;
      cfg.spi_3wire     =     false;
      cfg.use_lock      =      true;
      cfg.dma_channel   =         1;
      cfg.pin_sclk      =        14;
      cfg.pin_mosi      =        13;
      cfg.pin_miso      =        12;
      cfg.pin_dc        =         2;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs           =    15;
      cfg.pin_rst          =    -1;
      cfg.pin_busy         =    -1;
      cfg.panel_width      =   240;
      cfg.panel_height     =   320;
      cfg.offset_x         =     0;
      cfg.offset_y         =     0;
      cfg.offset_rotation  =     0;
      cfg.dummy_read_pixel =     8;
      cfg.dummy_read_bits  =     1;
      cfg.readable         =  true;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       = false;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl          =    21;
      cfg.invert          = false;
      cfg.freq            = 44100;
      cfg.pwm_channel     =     7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min           =       300;
      cfg.x_max           =      3900;
      cfg.y_min           =      3700;
      cfg.y_max           =       200;
      cfg.pin_int         =        36;
      cfg.bus_shared      =     false;
      cfg.offset_rotation =         0;
      cfg.spi_host        = SPI3_HOST;
      cfg.freq            =   1000000;
      cfg.pin_sclk        =        25;
      cfg.pin_mosi        =        32;
      cfg.pin_miso        =        39;
      cfg.pin_cs          =        33;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }

  void calibrate() {
    if (width() < height()) LGFX::setRotation(getRotation() ^ 1);
    LGFX::setTextDatum(textdatum_t::middle_center);
    LGFX::drawString("Please calibrate", width()>>1, height() >> 1);
    LGFX::setTextDatum(textdatum_t::top_left);

    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (LGFX::isEPD()) std::swap(fg, bg);
    LGFX::calibrateTouch(nullptr, fg, bg, std::max(width(), height()) >> 3);
  }

};

#endif