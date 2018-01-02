#pragma once
#include <obs-module.h>
#include <string>
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <clocale>
#include <locale>
#include "ccl/ccl.hpp"
#include "layouttype.hpp"
extern "C" {
#include <graphics\image-file.h>
#include "util.h"
}

struct InputKey {
    unsigned char m_key_char;
    bool m_pressed;
    uint16_t texture_u, texture_v;
    uint16_t w, h;

    uint16_t x_offset; // used to center buttons that span over multiple columns
    uint16_t row, column; // On screen location (in pixels for mouse/controller layouts)
};

struct OverlayLayout {
    LayoutType m_type;
    uint8_t m_key_count;
    int8_t m_key_space_v, m_key_space_h;
    uint8_t m_btn_w, m_btn_h;
    uint16_t m_w, m_h;
    uint8_t m_rows, m_cols;
    uint8_t texture_w;
    uint16_t texture_v_space;
    bool m_is_loaded = false;
    std::vector<InputKey> m_keys;
};

struct InputSource
{
    obs_source_t *m_source = nullptr;
    uint32_t cx = 0;
    uint32_t cy = 0;
    std::string m_image_file;
    std::wstring m_layout_file;
    gs_image_file_t *m_image = nullptr;
    OverlayLayout m_layout;

    inline InputSource(obs_source_t *source_, obs_data_t *settings) :
        m_source(source_)
    {
        obs_source_update(m_source, settings);
    }

    inline ~InputSource()
    {
        UnloadOverlayTexure();
        UnloadOverlayLayout();
    }

    void LoadOverlayTexture(void);
    void LoadOverlayLayout(void);
    void UnloadOverlayLayout(void);
    void UnloadOverlayTexure(void);

    void DrawKey(gs_effect_t *effect, InputKey* key, uint16_t x, uint16_t y);
    void CheckKeys(void);

    inline bool io_begins(const std::string line, const std::string prefix)
    {
        if (prefix.size() > line.size()) return false;
        return equal(prefix.begin(), prefix.end(), line.begin());
    }

    inline void io_cut(std::string& line)
    {
        line = line.substr(line.rfind('=') + 1, line.size());
    }

    // Read key order from a string e.g. "A,B,C,0x10"
    unsigned char read_chain(std::string& l)
    {
        std::string temp = l.substr(0, l.find(','));
        l = l.substr(l.find(',') + 1, l.size());

        if (temp.length() < 2) {
            return temp[0];
        }
        else if (temp.find("0x") != std::string::npos)
        {
            return std::stoul(temp, nullptr, 16);
        }
        return 'A';
    }

    uint16_t read_chain_int(std::string& l)
    {
        std::string temp = l.substr(0, l.find(','));
        l = l.substr(l.find(',') + 1, l.size());
        return std::atoi(temp.c_str());
    }

    inline void Update(obs_data_t *settings);
    inline void Tick(float seconds);
    inline void Render(gs_effect_t *effect);
};

// For registering
static obs_properties_t *get_properties_for_overlay(void *data);

void register_overlay_source();