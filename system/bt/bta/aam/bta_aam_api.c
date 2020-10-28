#include "bt_target.h"

#include <string.h>
#include "bt_common.h"
#include "bta_sys.h"
#include "bta_aam_api.h"
#include "osi/include/log.h"
#include "stack/include/aam_api.h"


void BTA_AAM_Init()
{
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_AAM_Init (not error)");
    aam_init();
    return;
}

bool BTA_Add_AppDevRecord(const char* app_id, const char* dev_id, int permission_level)
{
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_Add_AppDevRecord (not error) with inputs %s, %s, %d.", app_id, dev_id, permission_level);
    bool status = AAM_Add_AppDev_Rec(app_id, dev_id, permission_level);
    return status;
}

bool BTA_Delete_AppDevRecord(const char* app_id, const char* dev_id)
{
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_Delete_AppDevRecord (not error) with inputs %s, %s.", app_id, dev_id);
    bool status = AAM_Delete_AppDev_Rec(app_id, dev_id);
    return status;
}

bool BTA_Modify_AppDevRecord(const char* app_id, const char* dev_id, int permission_level)
{
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_Modify_AppDevRecord (not error) with inputs %s, %s, %d.", app_id, dev_id, permission_level);
    bool status = AAM_Modify_AppDev_Rec(app_id, dev_id, permission_level);
    return status;
}

int BTA_Check_AppDevID(const char* app_id, const char* dev_id)
{
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_Check_AppDevID (not error) with inputs %s, %s.", app_id, dev_id);
    int status = AAM_ID_Check(app_id, dev_id);
    APPL_TRACE_ERROR("CROSSAPPBLE /system/bt/bta/aam/bta_aam_api.c=>BTA_Check_AppDevID (not error) status=%d", status);
    return status;
}