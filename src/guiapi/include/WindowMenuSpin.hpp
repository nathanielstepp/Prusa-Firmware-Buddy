/**
 * @file WindowMenuSpin.hpp
 * @author Radek Vana
 * @brief Spinner menu item, allow to numeric change value
 * @date 2020-11-09
 */

#pragma once

#include "i_window_menu_item.hpp"
#include <i18n.h>
#include <numeric_input_config.hpp>
#include <array>
#include <cstddef>
#include <cstdint>
#include <printers.h>
#include <option/has_touch.h>

/*****************************************************************************/
// IWiSpin
class WiSpin : public IWindowMenuItem {
public:
    static constexpr padding_ui8_t Padding = GuiDefaults::MenuSpinHasUnits ? GuiDefaults::MenuPaddingSpecial : GuiDefaults::MenuPaddingItems;
    static constexpr size_t unit__half_space_padding = 6;
    static constexpr bool show_units = GuiDefaults::MenuSpinHasUnits;

public:
    WiSpin(float value, const NumericInputConfig &config, string_view_utf8 label, const img::Resource *id_icon = nullptr, is_enabled_t enabled = is_enabled_t::yes, is_hidden_t hidden = is_hidden_t::no);

public:
    inline float value() const {
        return value_;
    }

    inline void set_value(float val) {
        value_ = val;
        Change(0);
    }

    /// Deprecated legacy variant of value()
    inline float GetVal() const {
        return value();
    }

    /// Deprecated legacy variant of set_value()
    inline void SetVal(float val) {
        set_value(val);
    }

protected:
    virtual void OnClick() {}

protected:
    static Rect16::Width_t calculateExtensionWidth(const NumericInputConfig &config);

    Rect16 getSpinRect(Rect16 extension_rect) const;
    Rect16 getUnitRect(Rect16 extension_rect) const;

    void click(IWindowMenu &window_menu) override;
    void printExtension(Rect16 extension_rect, color_t color_text, color_t color_back, ropfn raster_op) const override;

    virtual invalidate_t change(int dif) override;
    void update();

#if HAS_TOUCH()
    void touch(IWindowMenu &window_menu, point_ui16_t relative_touch_point) final;
#endif

private:
    const NumericInputConfig &config_;
    std::array<char, 10> text_buffer_;
    float value_;
};
