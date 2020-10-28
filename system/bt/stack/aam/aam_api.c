/*******************************************************************************
 *
 * File containing the functionality for AAM and AAD (access control checks
 * for BLE access.
 * 
 * Intended as proof-of-concept ONLY. Not to be used in production code.
 *
 ******************************************************************************/

#include "aam_api.h"
#define LOG_TAG "CROSSAPPBLE bt_aam"

#include "bt_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <log/log.h>
#include "osi/include/log.h"
#include "osi/include/list.h"


tAAM_CB aam_cb;



void aam_init(void)
{
    LOG_WARN(LOG_TAG, "/system/bt/stack/aam/aam_api.c=>aam_init()");
    memset(&aam_cb, 0, sizeof(tAAM_CB));
    aam_cb.app_dev_rec = list_new(osi_free);
}


/*******************************************************************************
**
** Function         AAM_Add_AppDev_Rec
**
** Description      Add record to AAD for an App-Device pair.
**
** Parameter        app_id : application ID (package name)
**                  permission_level: uint8 AAM permission level.
**
** Returns          True if successfully added. False otherwise.
**
*******************************************************************************/
bool AAM_Add_AppDev_Rec (const char* app_id, const char* dev_id, tAAM_STATUS permission_level)
{
    if (!app_id)
        return false;
    if (!dev_id)
        return false;
    if (app_id[0] == '\0')
        return false;
    if (dev_id[0] == '\0')
        return false;
    if (permission_level < AAM_ERROR)
        return false;
    if (permission_level > AAM_DENY)
        return false;
    
    // First check if the app_id, dev_id is already present in database.
    tAAM_APP_DEV_REC *temp = aam_find_record(app_id, dev_id);
    if (temp != NULL)
    {
        LOG_ERROR(LOG_TAG, "App-Dev ID pair already in database! %s, %s", app_id, dev_id);
        return false;
    }
    
    // Create a new record and add to list.
    tAAM_APP_DEV_REC *p_app_dev_rec = NULL;
    
    p_app_dev_rec = osi_calloc(sizeof(tAAM_APP_DEV_REC));
    LOG_WARN(LOG_TAG, "Storing %s, %s, with permission %d in Database", app_id, dev_id, permission_level);

    bool appended = list_append(aam_cb.app_dev_rec, p_app_dev_rec);
    if (appended == true) {
        LOG_WARN(LOG_TAG, "Successfully added empty record to list");
    } else {
        LOG_WARN(LOG_TAG, "Unable to add empty record to list");
    }
    
    // Fill out the fields within the record.
    memcpy (p_app_dev_rec->app_id, app_id, strlen(app_id)+1);
    LOG_WARN(LOG_TAG, "Successfully added app_id");
    memcpy (p_app_dev_rec->dev_id, dev_id, strlen(dev_id)+1);
    LOG_WARN(LOG_TAG, "Successfully added dev_id");
    p_app_dev_rec->permission_level = permission_level;
    
    // Verification.
    LOG_WARN(LOG_TAG, "Object app_id=%s", p_app_dev_rec->app_id);
    LOG_WARN(LOG_TAG, "Object dev_id=%s", p_app_dev_rec->dev_id);
    LOG_WARN(LOG_TAG, "Object permission_level=%d", p_app_dev_rec->permission_level);

    return true;
}




/*******************************************************************************
**
** Function         AAM_Delete_AppDev_Rec
**
** Description      Add record to AAD for an App-Device pair.
**
** Parameter        app_id : application ID (package name)
**
** Returns          True if successfully deleted. False otherwise.
**
*******************************************************************************/
bool AAM_Delete_AppDev_Rec (const char* app_id, const char* dev_id)
{
    if (!app_id)
        return false;
    if (!dev_id)
        return false;
    if (app_id[0] == '\0')
        return false;
    if (dev_id[0] == '\0')
        return false;

    tAAM_APP_DEV_REC *p_app_dev_rec = aam_find_record(app_id, dev_id);
    if (p_app_dev_rec != NULL)
    {
        list_remove(aam_cb.app_dev_rec, p_app_dev_rec);
        return true;
    }
    
    return false;
}


/*******************************************************************************
**
** Function         AAM_Modify_AppDev_Rec
**
** Description      Modify record to AAD for an App-Device pair.
**
** Parameter        app_id : application ID (package name)
**                  permission_level: uint8 AAM permission level.
**
** Returns          True if successfully added. False otherwise.
**
*******************************************************************************/
bool AAM_Modify_AppDev_Rec (const char* app_id, const char* dev_id, tAAM_STATUS permission_level)
{
    if (!app_id)
        return false;
    if (!dev_id)
        return false;
    if (app_id[0] == '\0')
        return false;
    if (dev_id[0] == '\0')
        return false;
    if (permission_level < AAM_ERROR)
        return false;
    if (permission_level > AAM_DENY)
        return false;
    
    // First check if the app_id, dev_id is already present in database.
    tAAM_APP_DEV_REC *p_app_dev_rec = aam_find_record(app_id, dev_id);
    if (p_app_dev_rec != NULL)
    {
        LOG_WARN(LOG_TAG, "AAM_Modify_AppDev_Rec: App-Dev ID pair (%s, %s) already in database. Existing permission level is %d. Modifying to %d", app_id, dev_id, p_app_dev_rec->permission_level, permission_level);
        p_app_dev_rec->permission_level = permission_level;
        return true;
    } else {
        LOG_WARN(LOG_TAG, "AAM_Modify_AppDev_Rec: App-Dev ID pair (%s, %s) not in database. Adding new.", app_id, dev_id);
        bool ret = AAM_Add_AppDev_Rec(app_id, dev_id, permission_level);
        return ret;
    }
    
    return false;
}


/*******************************************************************************
**
** Function         AAD_ID_Check
**
** Description      BTA GATTC calls this function, to determine whether
**                  an application should be allowed access to a particular
**                  BLE device.
**
** Parameter        app_id : application ID (package name)
**
** Returns          tAAM_STATUS: permission level stored for app-dev pair.
**
*******************************************************************************/
tAAM_STATUS AAM_ID_Check(const char* app_id, const char* dev_id)
{
    if (!app_id)
        return AAM_ERROR;
    if (!dev_id)
        return AAM_ERROR;
    if (app_id[0] == '\0')
        return AAM_ERROR;
    if (dev_id[0] == '\0')
        return AAM_ERROR;
    
    LOG_WARN(LOG_TAG, "CROSSAPPBLE /system/bt/stack/aam/aam_api.c=>AAM_ID_Check for app_id=%s, dev_id=%s", app_id, dev_id);
    tAAM_APP_DEV_REC *p_app_dev_rec = aam_find_record(app_id, dev_id);
    if (p_app_dev_rec == NULL)
    {
        return AAM_USERAUTH_REQD;
    }
    
    tAAM_STATUS permission_level = p_app_dev_rec->permission_level;
    return permission_level;
}



/*******************************************************************************
**
** Function         AAM_Find_Record
**
** Description      Find the rcord associated with a particular app-dev pair.
**
** Parameter        app_id : application ID (package name)
**
** Returns          tAAM_APP_DEV_REC: AAM record.
**
*******************************************************************************/
tAAM_APP_DEV_REC *aam_find_record (const char* app_id, const char* dev_id)
{
    if (!app_id)
        return NULL;
    if (!dev_id)
        return NULL;
    if (app_id[0] == '\0')
        return NULL;
    if (dev_id[0] == '\0')
        return NULL;

    LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record for app_id=%s, dev_id=%s", app_id, dev_id);
    
    if (aam_cb.app_dev_rec == NULL) {
        LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record. List is NULL. Returning NULL");
        return NULL;
    }
    
    if (list_length(aam_cb.app_dev_rec) == 0) {
        LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record. List is empty. Returning NULL");
        return NULL;
    }
    
    for (const list_node_t *node = list_begin(aam_cb.app_dev_rec); node != list_end(aam_cb.app_dev_rec); node = list_next(node)) 
    {
        tAAM_APP_DEV_REC *list_element = list_node(node);
        
        LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record comparing %s with %s with length=%d", list_element->app_id, app_id, strlen(app_id));
        if (strncmp(list_element->app_id, app_id, strlen(app_id)) != 0) {
            LOG_WARN(LOG_TAG, "CROSSAPPBLE no app ID match, dev_id is %s and permission_level is %d for this record. Check next record, if exists.", list_element->dev_id, list_element->permission_level);
            continue;
        }
        
        LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record comparing %s with %s with length=%d", list_element->dev_id, dev_id, strlen(dev_id));
        if (strncmp (list_element->dev_id, dev_id, strlen(dev_id)) != 0) {
            LOG_WARN(LOG_TAG, "CROSSAPPBLE no dev ID match.  Check next record, if exists.");
            continue;
        }
        
        LOG_WARN(LOG_TAG, "CROSSAPPBLE aam_find_record for app_id=%s, dev_id=%s found a record", app_id, dev_id);
        return list_element;
    }
    
    return NULL;
}

void concat_app_dev_id(const char* app_id, const char* dev_id, char* app_dev_id)
{
    app_dev_id[0] = '\0';
    strcpy(app_dev_id, app_id);
    strcat(app_dev_id, "-");
    strcat(app_dev_id, dev_id);
    LOG_WARN(LOG_TAG, "CROSSAPPBLE concat_app_dev_id for app_id=%s, dev_id=%s results in %s", app_id, dev_id, app_dev_id);
}
