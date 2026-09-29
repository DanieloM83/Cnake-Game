/**
 * @file assets.c
 * @brief Loading and lifetime management of embedded game assets.
 *
 * Asset data is compiled into the executable and exposed through
 * generated/embedded_assets.h. This module owns all raylib resource
 * handles created from that data.
 *
 * The asset subsystem must be initialized after the raylib window has
 * been created. If assets_init() returns false, the caller must call
 * assets_destroy() before shutting down the application.
 */

#include "cnake.h"
#include "generated/embedded_assets.h"

static Sound death_sound;
static Sound move_sound;
static Sound eat_sound;
static Music theme;
static Image icon;

static float theme_volume   = 0.0f;
static float theme_target   = 0.0f;
static bool muted           = false;
static bool audio_available = true;

/**
 * @brief Loads a raylib Sound from embedded WAV data.
 *
 * The temporary Wave object is released after the Sound is created.
 *
 * @param data Pointer to the embedded WAV data.
 * @param size Size of the WAV data in bytes.
 *
 * @return A valid Sound on success, or an invalid Sound on failure.
 */
static Sound load_sound(const unsigned char* data, int size) {
    Wave wave = LoadWaveFromMemory(".wav", data, size);

    if (!IsWaveValid(wave)) return (Sound){0};

    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);

    return sound;
}

/**
 * @brief Initializes all embedded game assets.
 *
 * This function loads sound effects, background music, and the window icon
 * from memory. The caller is responsible for calling assets_destroy() after
 * a successful or partially successful initialization attempt.
 *
 * @return true if every asset was loaded successfully, false otherwise.
 */
bool assets_init(void) {
    if (!IsAudioDeviceReady()) InitAudioDevice();

    if (IsAudioDeviceReady()) {
        death_sound     = load_sound(death_sound_data, death_sound_size);
        move_sound      = load_sound(move_sound_data, move_sound_size);
        eat_sound       = load_sound(eat_sound_data, eat_sound_size);
        theme           = LoadMusicStreamFromMemory(".mp3", theme_music_data, theme_music_size);
        audio_available = IsSoundValid(death_sound) && IsSoundValid(move_sound) &&
                          IsSoundValid(eat_sound) && IsMusicValid(theme);

        if (!audio_available)
            TraceLog(LOG_WARNING, "Audio assets are unavailable; continuing without audio");
    }

    icon = LoadImageFromMemory(".png", window_icon_data, window_icon_size);
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    if (!IsImageValid(icon)) return false;
    SetWindowIcon(icon);

    return true;
}

void assets_toggle_mute(void) {
    if (!audio_available) return;

    muted = !muted;
    SetMusicVolume(theme, muted ? 0.0f : theme_volume);
}

void sound_play_death(void) {
    if (!muted && audio_available && IsSoundValid(death_sound)) PlaySound(death_sound);
}

void sound_play_move(void) {
    if (!muted && audio_available && IsSoundValid(move_sound)) PlaySound(move_sound);
}

void sound_play_eat(void) {
    if (!muted && audio_available && IsSoundValid(eat_sound)) PlaySound(eat_sound);
}

void music_play_theme(void) {
    if (!audio_available) return;

    theme_volume = 0.0f;
    theme_target = 0.35f;

    PlayMusicStream(theme);
}

void music_fade_out(void) {
    if (!audio_available) return;

    theme_target = 0.0f;
}

void music_fade_in(void) {
    if (!audio_available) return;

    theme_target = 0.35f;

    if (!IsMusicStreamPlaying(theme)) {
        SetMusicVolume(theme, theme_volume);
        PlayMusicStream(theme);
    }
}

void music_update_theme(float delta_time) {
    if (!audio_available) return;

    UpdateMusicStream(theme);

    const float delta_volume = ASSETS_THEME_SPEED * delta_time;

    if (theme_volume < theme_target) {
        theme_volume = min_float(theme_volume + delta_volume, theme_target);
    } else if (theme_volume > theme_target) {
        theme_volume = max_float(theme_volume - delta_volume, theme_target);
    }

    SetMusicVolume(theme, muted ? 0.0f : theme_volume);

    if (theme_volume <= 0.0f && theme_target == 0.0f && IsMusicStreamPlaying(theme)) {
        StopMusicStream(theme);
    }
}

/**
 * @brief Releases all resources owned by the asset subsystem.
 *
 * This function is also intended to be called after a partially failed
 * assets_init() call.
 */
void assets_destroy(void) {
    if (IsMusicValid(theme)) {
        StopMusicStream(theme);
        UnloadMusicStream(theme);
    }

    if (IsSoundValid(death_sound)) UnloadSound(death_sound);
    if (IsSoundValid(move_sound)) UnloadSound(move_sound);
    if (IsSoundValid(eat_sound)) UnloadSound(eat_sound);
    if (IsImageValid(icon)) UnloadImage(icon);

    if (IsAudioDeviceReady()) CloseAudioDevice();
}