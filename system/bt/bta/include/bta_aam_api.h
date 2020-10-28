#ifndef BTA_AAM_API_H
#define BTA_AAM_API_H


extern void BTA_AAM_Init(void);

extern bool BTA_Add_AppDevRecord(const char* app_id, const char* dev_id, int permission_level);

extern bool BTA_Delete_AppDevRecord(const char* app_id, const char* dev_id);

extern bool BTA_Modify_AppDevRecord(const char* app_id, const char* dev_id, int permission_level);

extern int BTA_Check_AppDevID(const char* app_id, const char* dev_id);






#endif /* BTA_AAM_API_H */