#ifndef ANDROID_INCLUDE_BT_AAM_H
#define ANDROID_INCLUDE_BT_AAM_H

#include <stdint.h>

__BEGIN_DECLS

/** Represents the standard Bluetooth AAM interface. */
typedef struct {
    void (*aam_init) (void);
    bool (*add_app_dev_record)( const char* app_id, const char* dev_id, int perm_level );
    bool (*delete_app_dev_record)( const char* app_id, const char* dev_id );
    bool (*modify_app_dev_record)( const char* app_id, const char* dev_id, int perm_level );
    int (*check_app_dev_access)( const char* app_id, const char* dev_id );
} btaam_interface_t;

__END_DECLS

#endif /* ANDROID_INCLUDE_BT_AAM_H */
