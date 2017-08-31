@ stdcall AuthzFreeAuditEvent(ptr) authz.AuthzFreeAuditEvent
@ stdcall AuthzFreeContext(long) authz.AuthzFreeContext
@ stdcall AuthzFreeResourceManager(ptr) authz.AuthzFreeResourceManager
@ stdcall AuthziFreeAuditEventType(ptr) authz.AuthziFreeAuditEventType
@ stdcall AuthziInitializeAuditEvent(long ptr ptr ptr ptr long wstr wstr wstr wstr ptr) authz.AuthziInitializeAuditEvent
@ stdcall AuthziInitializeAuditEventType(long long long long) authz.AuthziInitializeAuditEventType
@ stdcall AuthziInitializeAuditParams(long ptr ptr wstr long) authz.AuthziInitializeAuditParams
@ stdcall AuthziLogAuditEvent(long ptr ptr) authz.AuthziLogAuditEvent
@ stdcall AuthzInitializeContextFromAuthzContext(long ptr ptr long ptr ptr) authz.AuthzInitializeContextFromAuthzContext
@ stdcall AuthzInitializeContextFromSid(long ptr long ptr int64 ptr ptr) authz.AuthzInitializeContextFromSid
@ stdcall AuthzInitializeContextFromToken(long long long ptr int64 ptr ptr) authz.AuthzInitializeContextFromToken
@ stdcall AuthzInitializeResourceManager(long ptr ptr ptr wstr ptr) authz.AuthzInitializeResourceManager
