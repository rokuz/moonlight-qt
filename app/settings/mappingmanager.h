#pragma once

#include <QSettings>

#include <SDL_keyboard.h>
#include <SDL_scancode.h>

class SdlGamepadMapping
{
public:
    SdlGamepadMapping() {}

    SdlGamepadMapping(QString string)
    {
        QStringList mapping = string.split(",");
        if (!mapping.isEmpty()) {
            m_Guid = mapping[0];

            string.remove(0, m_Guid.length() + 1);
            m_Mapping = string;
        }
    }

    SdlGamepadMapping(QString guid, QString mapping)
        : m_Guid(guid),
          m_Mapping(mapping)
    {

    }

    bool operator==(const SdlGamepadMapping& other) const
    {
        return m_Guid == other.m_Guid && m_Mapping == other.m_Mapping;
    }

    QString getGuid() const
    {
        return m_Guid;
    }

    QString getMapping() const
    {
        return m_Mapping;
    }

    QString getSdlMappingString() const
    {
        if (m_Guid.isEmpty() || m_Mapping.isEmpty()) {
            return "";
        }
        else {
            return m_Guid + "," + m_Mapping;
        }
    }

private:
    QString m_Guid;
    QString m_Mapping;
};

struct KeyboardMapping
{
    SDL_Scancode m_Scancode = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
    Uint16 m_Mod = 0;

    KeyboardMapping() = default;
    KeyboardMapping(SDL_Scancode scancode, Uint16 mod) : m_Scancode(scancode), m_Mod(mod) {}

    bool operator<(const KeyboardMapping & m) const {
        if (m_Scancode == m.m_Scancode)
            return m_Mod < m.m_Mod;
        return m_Scancode < m.m_Scancode;
    }
};

class MappingManager
{
public:
    MappingManager();

    void addMapping(QString gamepadString);

    void addMapping(SdlGamepadMapping& gamepadMapping);

    void applyMappings();

    void save();

private:
    QMap<QString, SdlGamepadMapping> m_Mappings;
};

enum ProfileKey
{
    Custom = 0,
    DefaultMacToWindows,
};

struct KeyboardMappingProfile
{
  QString m_Name;
  QMap<KeyboardMapping, KeyboardMapping> m_Mappings;

  void setMapping(const KeyboardMapping & from, const KeyboardMapping & to)
  {
      m_Mappings[from] = to;
  }
};

class KeyboardMappingManager
{
public:
    KeyboardMappingManager();

    void reload();

    void setMapping(const KeyboardMapping & from, const KeyboardMapping & to);

    SDL_Keysym remap(SDL_Keysym m);

    void save();

private:
    QMap<ProfileKey, KeyboardMappingProfile *> m_builtInProfiles;
    KeyboardMappingProfile m_CurrentProfile;
};
