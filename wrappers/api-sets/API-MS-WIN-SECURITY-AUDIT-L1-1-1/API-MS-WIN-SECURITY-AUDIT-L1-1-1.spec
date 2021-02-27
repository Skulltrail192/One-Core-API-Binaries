@ stdcall AuditComputeEffectivePolicyBySid(ptr ptr long ptr) advapi32.AuditComputeEffectivePolicyBySid
@ stdcall AuditEnumerateCategories(ptr ptr) advapi32.AuditEnumerateCategories
@ stdcall AuditEnumeratePerUserPolicy(ptr) advapi32.AuditEnumeratePerUserPolicy
@ stdcall AuditEnumerateSubCategories(ptr long ptr ptr) advapi32.AuditEnumerateSubCategories
@ stdcall AuditFree(ptr) advapi32.AuditFree
@ stdcall AuditLookupCategoryNameW(ptr wstr) advapi32.AuditLookupCategoryNameW
@ stdcall AuditLookupSubCategoryNameW(ptr str) advapi32.AuditLookupSubCategoryNameW
@ stub AuditQueryGlobalSaclW
@ stdcall AuditQueryPerUserPolicy(ptr ptr long ptr) advapi32.AuditQueryPerUserPolicy
@ stdcall AuditQuerySecurity(long ptr) advapi32.AuditQuerySecurity
@ stdcall AuditQuerySystemPolicy(ptr long ptr) advapi32.AuditQuerySystemPolicy
@ stub AuditSetGlobalSaclW
@ stdcall AuditSetPerUserPolicy(ptr ptr long) advapi32.AuditSetPerUserPolicy
@ stdcall AuditSetSecurity(ptr long) advapi32.AuditSetSecurity
@ stdcall AuditSetSystemPolicy(ptr long) advapi32.AuditSetSystemPolicy
