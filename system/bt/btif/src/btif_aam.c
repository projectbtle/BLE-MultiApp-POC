/*******************************************************************************
 *
 *  Filename:      btif_aam.c
 *
 *  Description:   AAM implementation
 *
 *******************************************************************************/
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/bluetooth.h>
#include <hardware/bt_aam.h>

#include "btif_aam.h"
#include "btif_storage.h"

#include "bta_api.h"
#include "bta_aam_api.h"

#include "osi/include/log.h"


static void aam_init(void)
{
    BTA_AAM_Init();
    return;
}

static bool add_app_dev_record (const char* app_id, const char* dev_id, int perm_level)
{
    LOG_WARN("CROSSAPPBLE", "/system/bt/btif/src/btif_aam.c=>add_app_dev_record %s: App ID=%s, Dev ID=%s, perm_level=%d", __FUNCTION__, app_id, dev_id, perm_level);
    bool status = BTA_Add_AppDevRecord(app_id, dev_id, perm_level);
    int ret;
    
    if (status == TRUE) {
        // Store to NVRAM.
        LOG_WARN("CROSSAPPBLE", "/system/bt/btif/src/btif_aam.c=>add_app_dev_record %s Storing App ID=%s, Dev ID=%s, perm_level=%d to NVRAM", __FUNCTION__, app_id, dev_id, perm_level);
        char app_dev_id[75];
        app_dev_id[0] = '\0';
        strcpy(app_dev_id, app_id);
        strcat(app_dev_id, "-");
        strcat(app_dev_id, dev_id);
        ret = btif_storage_add_aam_app_device(app_dev_id, perm_level);
    }
    return status;
}

static bool delete_app_dev_record (const char* app_id, const char* dev_id)
{
    LOG_WARN("CROSSAPPBLE", "/system/bt/btif/src/btif_aam.c=>delete_app_dev_record %s: App ID=%s, Dev ID=%s", __FUNCTION__, app_id, dev_id);
    bool status = BTA_Delete_AppDevRecord(app_id, dev_id);
    return status;
}

static bool modify_app_dev_record (const char* app_id, const char* dev_id, int perm_level)
{
    LOG_WARN("CROSSAPPBLE", "/system/bt/btif/src/btif_aam.c=>modify_app_dev_record %s: App ID=%s, Dev ID=%s, perm_level=%d", __FUNCTION__, app_id, dev_id, perm_level);
    bool status = BTA_Modify_AppDevRecord(app_id, dev_id, perm_level);
    return status;
}

static int check_app_dev_access (const char* app_id, const char* dev_id)
{
    LOG_WARN("CROSSAPPBLE", "/system/bt/btif/src/btif_aam.c=>check_app_dev_access %s: App ID=%s, Dev ID=%s", __FUNCTION__, app_id, dev_id);
    int status = BTA_Check_AppDevID(app_id, dev_id);
    return status;
}



static const btaam_interface_t btaamInterface = {
    aam_init,
    add_app_dev_record,
    delete_app_dev_record,
    modify_app_dev_record,
    check_app_dev_access
};

const btaam_interface_t *btif_aam_get_interface()
{
    return &btaamInterface;
}