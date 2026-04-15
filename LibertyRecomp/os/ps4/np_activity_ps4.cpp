/**
 * sceNpActivity stub library for PS4 (OpenOrbis).
 *
 * On real PS4 hardware, these are resolved from libSceNpActivity.sprx.
 * For the recompiler, we provide working implementations that update
 * the in-memory presence status structure and return success.
 */

#ifdef LIBERTY_RECOMP_PS4

#include <orbis/NpActivity.h>
#include <string.h>

extern "C" {

int32_t sceNpActivityInitPresenceStatus(SceNpActivityPresenceStatus* status) {
    if (!status) return -1;
    memset(status, 0, sizeof(*status));
    return 0;
}

int32_t sceNpActivitySetPresenceStatusString(SceNpActivityPresenceStatus* status,
                                              const char* key, const char* value) {
    if (!status || !key || !value) return -1;

    /* Copy the key and value into the status structure. */
    size_t key_len = strlen(key);
    if (key_len >= sizeof(status->presenceKey))
        key_len = sizeof(status->presenceKey) - 1;
    memcpy(status->presenceKey, key, key_len);
    status->presenceKey[key_len] = '\0';

    size_t val_len = strlen(value);
    if (val_len >= sizeof(status->statusString))
        val_len = sizeof(status->statusString) - 1;
    memcpy(status->statusString, value, val_len);
    status->statusString[val_len] = '\0';

    return 0;
}

int32_t sceNpActivitySetPresence(int32_t userId,
                                  const SceNpActivityPresenceStatus* status) {
    /* In a real PS4 environment this would push the presence to PSN.
     * For the recompiler, silently succeed. */
    (void)userId;
    (void)status;
    return 0;
}

}  // extern "C"

#endif /* LIBERTY_RECOMP_PS4 */
