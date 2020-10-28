#ifndef AAM_API_H
#define AAM_API_H

#include "bt_target.h"
#include "osi/include/list.h"

#define  AAM_ERROR                      -1
#define  AAM_OK                         0                                    
#define  AAM_USERAUTH_REQD              1                          
#define  AAM_DENY                       2                            
typedef INT8 tAAM_STATUS;


typedef struct
{
    char app_id[55];
    char dev_id[18];
    tAAM_STATUS             permission_level;
} tAAM_APP_DEV_REC;


typedef struct
{
    list_t                  *app_dev_rec;   /* list of tAAM_APP_DEV_REC */
} tAAM_CB;


extern tAAM_CB aam_cb;




/*******************************************************************************
**
** Function         aam_init
**
** Description      Initialise AAM.
**
*******************************************************************************/
extern void aam_init(void);


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
extern  bool AAM_Add_AppDev_Rec (const char* app_id, const char* dev_id, tAAM_STATUS permission_level);



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
extern  bool AAM_Delete_AppDev_Rec (const char* app_id, const char* dev_id);



/*******************************************************************************
**
** Function         AAM_Modify_AppDev_Rec
**
** Description      Add record to AAD for an App-Device pair.
**
** Parameter        app_id : application ID (package name)
**                  permission_level: New uint8 AAM permission level.
**
** Returns          True if successfully modified. False otherwise.
**
*******************************************************************************/
extern  bool AAM_Modify_AppDev_Rec (const char* app_id, const char* dev_id, tAAM_STATUS permission_level);




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
extern  tAAM_STATUS AAM_ID_Check (const char* app_id, const char* dev_id);




/*******************************************************************************
**
** Function         aam_find_record
**
** Description      Find the rcord associated with a particular app-dev pair.
**
** Parameter        app_id : application ID (package name)
**
** Returns          tAAM_APP_DEV_REC: AAM record.
**
*******************************************************************************/
tAAM_APP_DEV_REC *aam_find_record (const char* app_id, const char* dev_id);


void concat_app_dev_id(const char* app_id, const char* dev_id, char* app_dev_id);
char** str_split(char* a_str, const char* a_delim);

#endif  /* AAM_API_H */