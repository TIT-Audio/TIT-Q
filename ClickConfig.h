#pragma once
#include <juce_core/juce_core.h>
#include <algorithm>
#include <cmath>
#include "Parameters.h"

namespace titq
{
// Comportamiento configurable de clics / dobles clics en la gráfica. Se guarda en archivos .json de texto plano
// en  Documentos/TIT Q/Presets/  y se carga siempre con valores seguros: si falta un campo o es inválido, se usa el de fábrica.
struct ClickConfig
{
    // --- banda nueva ---
    juce::String newBandType = "Bell";          // cualquiera de los nombres de curva (Bell, Low Shelf, BOOBTEC, AP TIT...)
    double newBandSlope = 12.0;                 // dB/oct (cortes y shelves)
    double newBandQ = 1.0;
    bool   gainFromClick = false;               // true: la banda nueva nace con la ganancia de la altura del clic

    // --- doble clic en la zona de graves / agudos ---
    juce::String bassAction = "HighPass";       // HighPass | LowPass | LowShelf | HighShelf | Bell | None
    double bassMinHz = 20.0, bassMaxHz = 35.0, bassSlope = 12.0;
    juce::String trebleAction = "LowPass";
    double trebleMinHz = 17000.0, trebleMaxHz = 21000.0, trebleSlope = 12.0;

    // --- doble clic sobre una banda existente ---
    juce::String bandDoubleClick = "ToggleOnOff";   // ToggleOnOff | Delete | None

    static juce::StringArray zoneActions()       { return { "HighPass", "LowPass", "LowShelf", "HighShelf", "Bell", "None" }; }
    static juce::StringArray bandClickActions()  { return { "ToggleOnOff", "Delete", "None" }; }

    // nombre de curva (para el parámetro "type") que corresponde a una acción de zona
    static juce::String typeForAction (const juce::String& a)
    {
        if (a == "HighPass")  return "Low Cut";     // High Pass = corta graves
        if (a == "LowPass")   return "High Cut";    // Low Pass = corta agudos
        if (a == "LowShelf")  return "Low Shelf";
        if (a == "HighShelf") return "High Shelf";
        if (a == "Bell")      return "Bell";
        return {};
    }

    // ---------- JSON ----------
    juce::var toVar() const
    {
        auto* nb = new juce::DynamicObject();
        nb->setProperty ("type", newBandType);
        nb->setProperty ("slope", newBandSlope);
        nb->setProperty ("q", newBandQ);
        nb->setProperty ("gainFromClick", gainFromClick);

        auto* bz = new juce::DynamicObject();
        bz->setProperty ("action", bassAction);
        bz->setProperty ("minHz", bassMinHz);
        bz->setProperty ("maxHz", bassMaxHz);
        bz->setProperty ("slope", bassSlope);

        auto* tz = new juce::DynamicObject();
        tz->setProperty ("action", trebleAction);
        tz->setProperty ("minHz", trebleMinHz);
        tz->setProperty ("maxHz", trebleMaxHz);
        tz->setProperty ("slope", trebleSlope);

        auto* root = new juce::DynamicObject();
        root->setProperty ("version", 1);
        root->setProperty ("newBand", juce::var (nb));
        root->setProperty ("bassZone", juce::var (bz));
        root->setProperty ("trebleZone", juce::var (tz));
        root->setProperty ("doubleClickOnBand", bandDoubleClick);
        return juce::var (root);
    }

    static ClickConfig fromVar (const juce::var& v)
    {
        ClickConfig c;                                  // valores de fábrica
        if (! v.isObject()) return c;

        const auto nb = v.getProperty ("newBand", juce::var());
        c.newBandType     = pickEnum (nb.getProperty ("type", juce::var()), typeNames(), c.newBandType);
        c.newBandSlope    = num (nb.getProperty ("slope", juce::var()), 0.0, 96.0, c.newBandSlope);
        c.newBandQ        = num (nb.getProperty ("q", juce::var()), 0.025, 40.0, c.newBandQ);
        c.gainFromClick   = flag (nb.getProperty ("gainFromClick", juce::var()), c.gainFromClick);

        const auto bz = v.getProperty ("bassZone", juce::var());
        c.bassAction  = pickEnum (bz.getProperty ("action", juce::var()), zoneActions(), c.bassAction);
        c.bassMinHz   = num (bz.getProperty ("minHz", juce::var()), 10.0, 500.0, c.bassMinHz);
        c.bassMaxHz   = num (bz.getProperty ("maxHz", juce::var()), 10.0, 500.0, c.bassMaxHz);
        c.bassSlope   = num (bz.getProperty ("slope", juce::var()), 0.0, 96.0, c.bassSlope);

        const auto tz = v.getProperty ("trebleZone", juce::var());
        c.trebleAction = pickEnum (tz.getProperty ("action", juce::var()), zoneActions(), c.trebleAction);
        c.trebleMinHz  = num (tz.getProperty ("minHz", juce::var()), 1000.0, 21000.0, c.trebleMinHz);
        c.trebleMaxHz  = num (tz.getProperty ("maxHz", juce::var()), 1000.0, 21000.0, c.trebleMaxHz);
        c.trebleSlope  = num (tz.getProperty ("slope", juce::var()), 0.0, 96.0, c.trebleSlope);

        c.bandDoubleClick = pickEnum (v.getProperty ("doubleClickOnBand", juce::var()), bandClickActions(), c.bandDoubleClick);

        // coherencia de los rangos de frecuencia
        if (c.bassMinHz >= c.bassMaxHz)     { c.bassMinHz = 20.0;      c.bassMaxHz = 35.0; }
        if (c.trebleMinHz >= c.trebleMaxHz) { c.trebleMinHz = 17000.0; c.trebleMaxHz = 21000.0; }
        return c;
    }

    // ---------- carpeta y archivos ----------
    static juce::File presetsFolder()
    {
        return juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
                   .getChildFile ("TIT Q").getChildFile ("Presets");
    }

    static juce::Array<juce::File> listPresets()
    {
        juce::Array<juce::File> out;
        for (auto& f : presetsFolder().findChildFiles (juce::File::findFiles, false, "*.json"))
            if (! f.getFileName().startsWithChar ('_'))      // los archivos que empiezan por "_" son internos (_default.json)
                out.add (f);
        std::sort (out.begin(), out.end(), [] (const juce::File& a, const juce::File& b)
                   { return a.getFileNameWithoutExtension().compareIgnoreCase (b.getFileNameWithoutExtension()) < 0; });
        return out;
    }

    static juce::File fileFor (const juce::String& name)
    {
        return presetsFolder().getChildFile (juce::File::createLegalFileName (name.trim()) + ".json");
    }

    static ClickConfig load (const juce::File& f)
    {
        if (! f.existsAsFile()) return {};
        return fromVar (juce::JSON::parse (f));              // JSON roto o incompleto -> valores de fábrica en lo que falte
    }

    static bool save (const juce::File& f, const ClickConfig& c)
    {
        f.getParentDirectory().createDirectory();
        return f.replaceWithText (juce::JSON::toString (c.toVar()));
    }

    // el preset marcado como Default se recuerda en _default.json
    static juce::String defaultName()
    {
        const auto v = juce::JSON::parse (presetsFolder().getChildFile ("_default.json"));
        return v.getProperty ("default", juce::var()).toString();
    }

    static void setDefaultName (const juce::String& name)
    {
        auto* o = new juce::DynamicObject();
        o->setProperty ("default", name);
        presetsFolder().getChildFile ("_default.json").replaceWithText (juce::JSON::toString (juce::var (o)));
    }

    // Arranque: carga el preset Default. Si no hay carpeta/archivos, crea Default.json con los valores de fábrica.
    // Nunca falla: ante cualquier problema devuelve los valores de fábrica.
    static ClickConfig loadDefault (juce::String& nameOut)
    {
        nameOut = "Default";
        const auto folder = presetsFolder();
        folder.createDirectory();

        if (listPresets().isEmpty())
        {
            save (folder.getChildFile ("Default.json"), ClickConfig());
            setDefaultName ("Default");
        }

        juce::String name = defaultName();
        if (name.isEmpty() || ! fileFor (name).existsAsFile())
        {
            const auto all = listPresets();
            if (all.isEmpty()) return {};
            name = all.getFirst().getFileNameWithoutExtension();
        }
        nameOut = name;
        return load (fileFor (name));
    }

private:
    static juce::String pickEnum (const juce::var& v, const juce::StringArray& allowed, const juce::String& def)
    {
        const auto s = v.toString().trim();
        for (auto& a : allowed) if (a.equalsIgnoreCase (s)) return a;
        return def;
    }
    static double num (const juce::var& v, double lo, double hi, double def)
    {
        if (! (v.isDouble() || v.isInt() || v.isInt64())) return def;
        const double d = (double) v;
        return std::isfinite (d) ? juce::jlimit (lo, hi, d) : def;
    }
    static bool flag (const juce::var& v, bool def)
    {
        return (v.isBool() || v.isInt() || v.isInt64()) ? (bool) v : def;
    }
};
} // namespace titq
