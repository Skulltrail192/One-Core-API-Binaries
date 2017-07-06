/*
 * Manifest parser for WUSA
 *
 * Copyright 2015 Michael Müller
 * Copyright 2015 Sebastian Lackner
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
 
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H 
#define __msxml6_h__

#include "windows.h"
#define COBJMACROS
#include "initguid.h"
// #include <msxml2.h>
#include "wusa.h"
#include "msxml6.h"
#include "wine/debug.h"
#include "wine/unicode.h"

WINE_DEFAULT_DEBUG_CHANNEL(wusa);

HRESULT IXMLDOMElement_getAttribute(IXMLDOMElement* This,BSTR p,VARIANT *var);
HRESULT IXMLDOMElement_get_tagName(IXMLDOMElement* This,BSTR *p);
HRESULT IXMLDOMElement_selectSingleNode(IXMLDOMElement* This,BSTR p,IXMLDOMNode **outNode);
HRESULT IXMLDOMNode_QueryInterface(IXMLDOMNode* This,REFIID riid,void **ppvObject);
ULONG IXMLDOMNode_Release(IXMLDOMNode* This);
HRESULT IXMLDOMElement_get_childNodes(IXMLDOMElement* This,IXMLDOMNodeList **outList);
HRESULT IXMLDOMNodeList_nextNode(IXMLDOMNodeList* This,IXMLDOMNode **outNode);
ULONG IXMLDOMElement_Release(IXMLDOMElement* This);
ULONG IXMLDOMNodeList_Release(IXMLDOMNodeList* This);
HRESULT IXMLDOMDocument_load(IXMLDOMDocument* This,VARIANT var1,VARIANT_BOOL *pbool);
HRESULT IXMLDOMDocument_get_documentElement(IXMLDOMDocument* This,IXMLDOMElement **domElement);
ULONG IXMLDOMDocument_Release(IXMLDOMDocument* This);

FORCEINLINE HRESULT IXMLDOMElement_getAttribute(IXMLDOMElement* This,BSTR p,VARIANT *var) {
    return This->lpVtbl->getAttribute(This,p,var);
}

FORCEINLINE HRESULT IXMLDOMElement_get_tagName(IXMLDOMElement* This,BSTR *p) {
    return This->lpVtbl->get_tagName(This,p);
}

FORCEINLINE HRESULT IXMLDOMElement_selectSingleNode(IXMLDOMElement* This,BSTR p,IXMLDOMNode **outNode) {
    return This->lpVtbl->selectSingleNode(This,p,outNode);
}

FORCEINLINE HRESULT IXMLDOMNode_QueryInterface(IXMLDOMNode* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE ULONG IXMLDOMNode_Release(IXMLDOMNode* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IXMLDOMNodeList_nextNode(IXMLDOMNodeList* This,IXMLDOMNode **outNode) {
    return This->lpVtbl->nextNode(This,outNode);
}

FORCEINLINE ULONG IXMLDOMElement_Release(IXMLDOMElement* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE ULONG IXMLDOMDocument_Release(IXMLDOMDocument* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IXMLDOMDocument_get_documentElement(IXMLDOMDocument* This,IXMLDOMElement **domElement) {
    return This->lpVtbl->get_documentElement(This,domElement);
}

FORCEINLINE ULONG IXMLDOMNodeList_Release(IXMLDOMNodeList* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IXMLDOMDocument_load(IXMLDOMDocument* This,VARIANT var1,VARIANT_BOOL *pbool) {
    return This->lpVtbl->load(This,var1,pbool);
}

FORCEINLINE HRESULT IXMLDOMElement_get_childNodes(IXMLDOMElement* This,IXMLDOMNodeList **outList) {
    return This->lpVtbl->get_childNodes(This,outList);
}

static struct dependency_entry *alloc_dependency(void)
{
    struct dependency_entry *entry = heap_alloc_zero(sizeof(*entry));
    if (!entry) WINE_ERR("failed to allocate memory for dependency\n");
    return entry;
}

static struct fileop_entry *alloc_fileop(void)
{
    struct fileop_entry *entry = heap_alloc_zero(sizeof(*entry));
    if (!entry) WINE_ERR("failed to allocate memory for fileop\n");
    return entry;
}

static struct registrykv_entry *alloc_registrykv(void)
{
    struct registrykv_entry *entry = heap_alloc_zero(sizeof(*entry));
    if (!entry) WINE_ERR("failed to allocate memory for registrykv\n");
    return entry;
}

static struct registryop_entry *alloc_registryop(void)
{
    struct registryop_entry *entry = heap_alloc_zero(sizeof(*entry));
    if (!entry) WINE_ERR("failed to allocate memory for registryop\n");
    else
    {
        list_init(&entry->keyvalues);
    }
    return entry;
}

static struct assembly_entry *alloc_assembly(void)
{
    struct assembly_entry *entry = heap_alloc_zero(sizeof(*entry));
    if (!entry) WINE_ERR("failed to allocate memory for assembly\n");
    else
    {
        list_init(&entry->dependencies);
        list_init(&entry->fileops);
        list_init(&entry->registryops);
    }
    return entry;
}

static void clear_identity(struct assembly_identity *entry)
{
    heap_free(entry->name);
    heap_free(entry->version);
    heap_free(entry->architecture);
    heap_free(entry->language);
    heap_free(entry->pubkey_token);
}

void free_dependency(struct dependency_entry *entry)
{
    clear_identity(&entry->identity);
    heap_free(entry);
}

static void free_fileop(struct fileop_entry *entry)
{
    heap_free(entry->source);
    heap_free(entry->target);
    heap_free(entry);
}

static void free_registrykv(struct registrykv_entry *entry)
{
    heap_free(entry->name);
    heap_free(entry->value_type);
    heap_free(entry->value);
    heap_free(entry);
}

static void free_registryop(struct registryop_entry *entry)
{
    struct registrykv_entry *keyvalue, *keyvalue2;

    heap_free(entry->key);

    LIST_FOR_EACH_ENTRY_SAFE(keyvalue, keyvalue2, &entry->keyvalues, struct registrykv_entry, entry)
    {
        list_remove(&keyvalue->entry);
        free_registrykv(keyvalue);
    }

    heap_free(entry);
}

void free_assembly(struct assembly_entry *entry)
{
    struct dependency_entry *dependency, *dependency2;
    struct fileop_entry *fileop, *fileop2;
    struct registryop_entry *registryop, *registryop2;

    heap_free(entry->filename);
    heap_free(entry->displayname);
    clear_identity(&entry->identity);

    LIST_FOR_EACH_ENTRY_SAFE(dependency, dependency2, &entry->dependencies, struct dependency_entry, entry)
    {
        list_remove(&dependency->entry);
        free_dependency(dependency);
    }
    LIST_FOR_EACH_ENTRY_SAFE(fileop, fileop2, &entry->fileops, struct fileop_entry, entry)
    {
        list_remove(&fileop->entry);
        free_fileop(fileop);
    }
    LIST_FOR_EACH_ENTRY_SAFE(registryop, registryop2, &entry->registryops, struct registryop_entry, entry)
    {
        list_remove(&registryop->entry);
        free_registryop(registryop);
    }

    heap_free(entry);
}

static WCHAR *get_xml_attribute(IXMLDOMElement *root, const WCHAR *name)
{
    WCHAR *ret = NULL;
    VARIANT var;
    BSTR bstr;

    if ((bstr = SysAllocString(name)))
    {
        VariantInit(&var);
        if (SUCCEEDED(IXMLDOMElement_getAttribute(root, bstr, &var)))
        {
            ret = (V_VT(&var) == VT_BSTR) ? strdupW(V_BSTR(&var)) : NULL;
            VariantClear(&var);
        }
        SysFreeString(bstr);
    }

    return ret;
}

static BOOL check_xml_tagname(IXMLDOMElement *root, const WCHAR *tagname)
{
    BOOL ret = FALSE;
    BSTR bstr;

    if (SUCCEEDED(IXMLDOMElement_get_tagName(root, &bstr)))
    {
        ret = !strcmpW(bstr, tagname);
        SysFreeString(bstr);
    }

    return ret;
}

static IXMLDOMElement *select_xml_node(IXMLDOMElement *root, const WCHAR *name)
{
    IXMLDOMElement *ret = NULL;
    IXMLDOMNode *node;
    BSTR bstr;

    if ((bstr = SysAllocString(name)))
    {
        if (SUCCEEDED(IXMLDOMElement_selectSingleNode(root, bstr, &node)))
        {
            if (FAILED(IXMLDOMNode_QueryInterface(node, &IID_IXMLDOMElement, (void **)&ret)))
                ret = NULL;
            IXMLDOMNode_Release(node);
        }
        SysFreeString(bstr);
    }

    return ret;
}

static BOOL call_xml_callbacks(IXMLDOMElement *root, BOOL (*func)(IXMLDOMElement *child, WCHAR *tagname, void *context), void *context)
{
    IXMLDOMNodeList *children;
    IXMLDOMElement *child;
    IXMLDOMNode *node;
    BSTR tagname;
    BOOL ret = TRUE;

    if (FAILED(IXMLDOMElement_get_childNodes(root, &children)))
        return FALSE;

    while (ret && IXMLDOMNodeList_nextNode(children, &node) == S_OK)
    {
        if (SUCCEEDED(IXMLDOMNode_QueryInterface(node, &IID_IXMLDOMElement, (void **)&child)))
        {
            if (SUCCEEDED(IXMLDOMElement_get_tagName(child, &tagname)))
            {
                ret = func(child, tagname, context);
                SysFreeString(tagname);
            }
            IXMLDOMElement_Release(child);
        }
        IXMLDOMNode_Release(node);
    }

    IXMLDOMNodeList_Release(children);
    return ret;
}

static IXMLDOMElement *load_xml(const WCHAR *filename)
{
    IXMLDOMDocument *document = NULL;
    IXMLDOMElement *root = NULL;
    VARIANT_BOOL success;
    VARIANT variant;
    BSTR bstr;

    WINE_TRACE("Loading XML from %s\n", debugstr_w(filename));

    if (!(bstr = SysAllocString(filename)))
        return FALSE;

    if (SUCCEEDED(CoCreateInstance(&CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, &IID_IXMLDOMDocument, (void **)&document)))
    {
        VariantInit(&variant);
        V_VT(&variant) = VT_BSTR;
        V_BSTR(&variant) = bstr;

        if (SUCCEEDED(IXMLDOMDocument_load(document, variant, &success)) && success)
        {
            if (FAILED(IXMLDOMDocument_get_documentElement(document, &root)))
                root = NULL;
        }
        IXMLDOMDocument_Release(document);
    }

    SysFreeString(bstr);
    return root;
}

static BOOL read_identity(IXMLDOMElement *root, struct assembly_identity *identity)
{
    static const WCHAR nameW[] = {'n','a','m','e',0};
    static const WCHAR versionW[] = {'v','e','r','s','i','o','n',0};
    static const WCHAR processorArchitectureW[] = {'p','r','o','c','e','s','s','o','r','A','r','c','h','i','t','e','c','t','u','r','e',0};
    static const WCHAR languageW[] = {'l','a','n','g','u','a','g','e',0};
    static const WCHAR publicKeyTokenW[] = {'p','u','b','l','i','c','K','e','y','T','o','k','e','n',0};

    memset(identity, 0, sizeof(*identity));
    if (!(identity->name            = get_xml_attribute(root, nameW))) goto error;
    if (!(identity->version         = get_xml_attribute(root, versionW))) goto error;
    if (!(identity->architecture    = get_xml_attribute(root, processorArchitectureW))) goto error;
    if (!(identity->language        = get_xml_attribute(root, languageW))) goto error;
    if (!(identity->pubkey_token    = get_xml_attribute(root, publicKeyTokenW))) goto error;
    return TRUE;

error:
    clear_identity(identity);
    return FALSE;
}

/* <assembly><dependency><dependentAssembly> */
static BOOL read_dependent_assembly(IXMLDOMElement *root, struct assembly_identity *identity)
{
    static const WCHAR dependencyTypeW[] = {'d','e','p','e','n','d','e','n','c','y','T','y','p','e',0};
    static const WCHAR installW[] = {'i','n','s','t','a','l','l',0};
    static const WCHAR select_assemblyIdentityW[] = {'.','/','/','a','s','s','e','m','b','l','y','I','d','e','n','t','i','t','y',0};
    IXMLDOMElement *child = NULL;
    WCHAR *dependency_type;
    BOOL ret = FALSE;

    if (!(dependency_type = get_xml_attribute(root, dependencyTypeW)))
    {
        WINE_ERR("Failed to get dependency type\n");
        return FALSE;
    }
    if (strcmpW(dependency_type, installW))
    {
        WINE_FIXME("Unimplemented dependency type %s\n", debugstr_w(dependency_type));
        goto error;
    }
    if (!(child = select_xml_node(root, select_assemblyIdentityW)))
    {
        WINE_FIXME("Failed to find assemblyIdentity child node\n");
        goto error;
    }

    ret = read_identity(child, identity);

error:
    if (child) IXMLDOMElement_Release(child);
    heap_free(dependency_type);
    return ret;
}

/* <assembly><dependency> */
static BOOL read_dependency(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR dependentAssemblyW[] = {'d','e','p','e','n','d','e','n','t','A','s','s','e','m','b','l','y',0};
    struct assembly_entry *assembly = context;
    struct dependency_entry *entry;

    if (strcmpW(tagname, dependentAssemblyW))
    {
        WINE_FIXME("Don't know how to handle dependency tag %s\n", debugstr_w(tagname));
        return FALSE;
    }

    if ((entry = alloc_dependency()))
    {
        if (read_dependent_assembly(child, &entry->identity))
        {
            WINE_TRACE("Found dependency %s\n", debugstr_w(entry->identity.name));
            list_add_tail(&assembly->dependencies, &entry->entry);
            return TRUE;
        }
        free_dependency(entry);
    }

    return FALSE;
}
static BOOL iter_dependency(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_dependency, assembly);
}

/* <assembly><package><update><component> */
/* <assembly><package><update><package> */
static BOOL read_components(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR assemblyIdentityW[] = {'a','s','s','e','m','b','l','y','I','d','e','n','t','i','t','y',0};
    struct assembly_entry *assembly = context;
    struct dependency_entry *entry;

    if (strcmpW(tagname, assemblyIdentityW))
    {
        WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    if ((entry = alloc_dependency()))
    {
        if (read_identity(child, &entry->identity))
        {
            WINE_TRACE("Found identity %s\n", debugstr_w(entry->identity.name));
            list_add_tail(&assembly->dependencies, &entry->entry);
            return TRUE;
        }
        free_dependency(entry);
    }

    return FALSE;
}
static BOOL iter_components(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_components, assembly);
}

/* <assembly><package><update> */
static BOOL read_update(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR applicableW[] = {'a','p','p','l','i','c','a','b','l','e',0};
    static const WCHAR componentW[] = {'c','o','m','p','o','n','e','n','t',0};
    static const WCHAR packageW[] = {'p','a','c','k','a','g','e',0};
    struct assembly_entry *assembly = context;

    if (!strcmpW(tagname, componentW))
        return iter_components(child, assembly);
    if (!strcmpW(tagname, packageW))
        return iter_components(child, assembly);
    if (!strcmpW(tagname, applicableW))
        return TRUE;

    WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
    return FALSE;
}
static BOOL iter_update(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_update, assembly);
}

/* <assembly><package> */
static BOOL read_package(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR updateW[] = {'u','p','d','a','t','e',0};
    static const WCHAR parentW[] = {'p','a','r','e','n','t',0};
    struct assembly_entry *assembly = context;

    if (!strcmpW(tagname, updateW))
        return iter_update(child, assembly);
    if (!strcmpW(tagname, parentW))
        return TRUE;

    WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
    return TRUE;
}
static BOOL iter_package(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_package, assembly);
}

/* <assembly><file> */
static BOOL read_file(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    static const WCHAR sourceNameW[] = {'s','o','u','r','c','e','N','a','m','e',0};
    static const WCHAR destinationPathW[] = {'d','e','s','t','i','n','a','t','i','o','n','P','a','t','h',0};
    struct fileop_entry *entry;

    if (!(entry = alloc_fileop()))
        return FALSE;

    if (!(entry->source = get_xml_attribute(root, sourceNameW))) goto error;
    if (!(entry->target = get_xml_attribute(root, destinationPathW))) goto error;

    WINE_TRACE("Found fileop %s -> %s\n", debugstr_w(entry->source), debugstr_w(entry->target));
    list_add_tail(&assembly->fileops, &entry->entry);
    return TRUE;

error:
    free_fileop(entry);
    return FALSE;
}

/* <assembly><registryKeys><registryKey> */
static BOOL read_registry_key(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR securityDescriptorW[] = {'s','e','c','u','r','i','t','y','D','e','s','c','r','i','p','t','o','r',0};
    static const WCHAR systemProtectionW[] = {'s','y','s','t','e','m','P','r','o','t','e','c','t','i','o','n',0};
    static const WCHAR registryValueW[] = {'r','e','g','i','s','t','r','y','V','a','l','u','e',0};
    static const WCHAR nameW[] = {'n','a','m','e',0};
    static const WCHAR valueTypeW[] = {'v','a','l','u','e','T','y','p','e',0};
    static const WCHAR valueW[] = {'v','a','l','u','e',0};
    struct registryop_entry *registryop = context;
    struct registrykv_entry *entry;

    if (!strcmpW(tagname, securityDescriptorW)) return TRUE;
    if (!strcmpW(tagname, systemProtectionW)) return TRUE;
    if (strcmpW(tagname, registryValueW))
    {
        WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    if (!(entry = alloc_registrykv()))
        return FALSE;

    if (!(entry->value_type = get_xml_attribute(child, valueTypeW))) goto error;
    entry->name = get_xml_attribute(child, nameW);      /* optional */
    entry->value = get_xml_attribute(child, valueW);    /* optional */

    WINE_TRACE("Found registry %s -> %s\n", debugstr_w(entry->name), debugstr_w(entry->value));
    list_add_tail(&registryop->keyvalues, &entry->entry);
    return TRUE;

error:
    free_registrykv(entry);
    return FALSE;
}
static BOOL iter_registry_key(IXMLDOMElement *root, struct registryop_entry *registryop)
{
    return call_xml_callbacks(root, read_registry_key, registryop);
}

/* <assembly><registryKeys> */
static BOOL read_registry_keys(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR registryKeyW[] = {'r','e','g','i','s','t','r','y','K','e','y',0};
    static const WCHAR keyNameW[] = {'k','e','y','N','a','m','e',0};
    struct assembly_entry *assembly = context;
    struct registryop_entry *entry;
    WCHAR *keyname;

    if (strcmpW(tagname, registryKeyW))
    {
        WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    if (!(keyname = get_xml_attribute(child, keyNameW)))
    {
        WINE_FIXME("RegistryKey tag doesn't specify keyName\n");
        return FALSE;
    }

    if ((entry = alloc_registryop()))
    {
        list_init(&entry->keyvalues);
        if (iter_registry_key(child, entry))
        {
            entry->key = keyname;
            WINE_TRACE("Found registryop %s\n", debugstr_w(entry->key));
            list_add_tail(&assembly->registryops, &entry->entry);
            return TRUE;
        }
        free_registryop(entry);
    }

    heap_free(keyname);
    return FALSE;
}
static BOOL iter_registry_keys(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_registry_keys, assembly);
}

/* <assembly> */
static BOOL read_assembly(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR assemblyIdentityW[] = {'a','s','s','e','m','b','l','y','I','d','e','n','t','i','t','y',0};
    static const WCHAR dependencyW[] = {'d','e','p','e','n','d','e','n','c','y',0};
    static const WCHAR packageW[] = {'p','a','c','k','a','g','e',0};
    static const WCHAR fileW[] = {'f','i','l','e',0};
    static const WCHAR registryKeysW[] = {'r','e','g','i','s','t','r','y','K','e','y','s',0};
    static const WCHAR trustInfoW[] = {'t','r','u','s','t','I','n','f','o',0};
    static const WCHAR configurationW[] = {'c','o','n','f','i','g','u','r','a','t','i','o','n',0};
    static const WCHAR deploymentW[] = {'d','e','p','l','o','y','m','e','n','t',0};
    struct assembly_entry *assembly = context;

    if (!strcmpW(tagname, assemblyIdentityW) && !assembly->identity.name)
        return read_identity(child, &assembly->identity);
    if (!strcmpW(tagname, dependencyW))
        return iter_dependency(child, assembly);
    if (!strcmpW(tagname, packageW))
        return iter_package(child, assembly);
    if (!strcmpW(tagname, fileW))
        return read_file(child, assembly);
    if (!strcmpW(tagname, registryKeysW))
        return iter_registry_keys(child, assembly);
    if (!strcmpW(tagname, trustInfoW))
        return TRUE;
    if (!strcmpW(tagname, configurationW))
        return TRUE;
    if (!strcmpW(tagname, deploymentW))
        return TRUE;

    WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
    return TRUE;
}
static BOOL iter_assembly(IXMLDOMElement *root, struct assembly_entry *assembly)
{
    return call_xml_callbacks(root, read_assembly, assembly);
}

struct assembly_entry *load_manifest(const WCHAR *filename)
{
    static const WCHAR assemblyW[] = {'a','s','s','e','m','b','l','y',0};
    static const WCHAR displaynameW[] = {'d','i','s','p','l','a','y','N','a','m','e',0};
    struct assembly_entry *entry = NULL;
    IXMLDOMElement *root = NULL;

    WINE_TRACE("Loading manifest %s\n", debugstr_w(filename));

    if (!(root = load_xml(filename))) return NULL;
    if (!check_xml_tagname(root, assemblyW))
    {
        WINE_FIXME("Didn't find assembly root node?\n");
        goto done;
    }

    if ((entry = alloc_assembly()))
    {
        entry->filename = strdupW(filename);
        entry->displayname = get_xml_attribute(root, displaynameW);
        if (iter_assembly(root, entry)) goto done;
        free_assembly(entry);
        entry = NULL;
    }

done:
    IXMLDOMElement_Release(root);
    return entry;
}

/* <unattend><servicing><package> */
static BOOL read_update_package(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR sourceW[] = {'s','o','u','r','c','e',0};
    static const WCHAR assemblyIdentityW[] = {'a','s','s','e','m','b','l','y','I','d','e','n','t','i','t','y',0};
    struct dependency_entry *entry;
    struct list *update_list = context;

    if (!strcmpW(tagname, sourceW)) return TRUE;
    if (strcmpW(tagname, assemblyIdentityW))
    {
        WINE_TRACE("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    if ((entry = alloc_dependency()))
    {
        if (read_identity(child, &entry->identity))
        {
            WINE_TRACE("Found update %s\n", debugstr_w(entry->identity.name));
            list_add_tail(update_list, &entry->entry);
            return TRUE;
        }
        free_dependency(entry);
    }

    return FALSE;
}
static BOOL iter_update_package(IXMLDOMElement *root, struct list *update_list)
{
    return call_xml_callbacks(root, read_update_package, update_list);
}

/* <unattend><servicing> */
static BOOL read_servicing(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR packageW[] = {'p','a','c','k','a','g','e',0};
    static const WCHAR installW[] = {'i','n','s','t','a','l','l',0};
    static const WCHAR actionW[] = {'a','c','t','i','o','n',0};
    struct list *update_list = context;
    WCHAR *action;
    BOOL ret = TRUE;

    if (strcmpW(tagname, packageW))
    {
        WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    if (!(action = get_xml_attribute(child, actionW)))
    {
        WINE_FIXME("Servicing tag doesn't specify action\n");
        return FALSE;
    }

    if (!strcmpW(action, installW))
        ret = iter_update_package(child, update_list);
    else
        WINE_FIXME("action %s not supported\n", debugstr_w(action));

    heap_free(action);
    return ret;
}
static BOOL iter_servicing(IXMLDOMElement *root, struct list *update_list)
{
    return call_xml_callbacks(root, read_servicing, update_list);
}

/* <unattend> */
static BOOL read_unattend(IXMLDOMElement *child, WCHAR *tagname, void *context)
{
    static const WCHAR servicingW[] = {'s','e','r','v','i','c','i','n','g',0};
    struct list *update_list = context;

    if (strcmpW(tagname, servicingW))
    {
        WINE_FIXME("Ignoring unexpected tag %s\n", debugstr_w(tagname));
        return TRUE;
    }

    return iter_servicing(child, update_list);

}
static BOOL iter_unattend(IXMLDOMElement *root, struct list *update_list)
{
    return call_xml_callbacks(root, read_unattend, update_list);
}

BOOL load_update(const WCHAR *filename, struct list *update_list)
{
    static const WCHAR unattendW[] = {'u','n','a','t','t','e','n','d',0};
    IXMLDOMElement *root = NULL;
    BOOL ret = FALSE;

    WINE_TRACE("Reading update %s\n", debugstr_w(filename));

    if (!(root = load_xml(filename))) return FALSE;
    if (!check_xml_tagname(root, unattendW))
    {
        WINE_FIXME("Didn't find unattend root node?\n");
        goto done;
    }

    ret = iter_unattend(root, update_list);

done:
    IXMLDOMElement_Release(root);
    return ret;
}

BOOL queue_update(struct assembly_entry *assembly, struct list *update_list)
{
    struct dependency_entry *entry;

    if (!(entry = alloc_dependency()))
        return FALSE;

    if (!(entry->identity.name          = strdupW(assembly->identity.name))) goto error;
    if (!(entry->identity.version       = strdupW(assembly->identity.version))) goto error;
    if (!(entry->identity.architecture  = strdupW(assembly->identity.architecture))) goto error;
    if (!(entry->identity.language      = strdupW(assembly->identity.language))) goto error;
    if (!(entry->identity.pubkey_token  = strdupW(assembly->identity.pubkey_token))) goto error;

    WINE_TRACE("Queued update %s\n", debugstr_w(entry->identity.name));
    list_add_tail(update_list, &entry->entry);
    return TRUE;

error:
    free_dependency(entry);
    return FALSE;
}
